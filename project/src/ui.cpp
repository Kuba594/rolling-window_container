#include "ui.h"

namespace fs = std::filesystem;

//Main loop that allows user to interact with enviroment
void UI::run() {
    std::cout << "-----------------------------------------\n"
              << "  Rolling Covariance Engine \n"
              << "-----------------------------------------\n"
              << "Write 'help' for commands, 'quit' to exit.\n\n";

    std::string line;
    while (true) {
        std::cout << "> " << std::flush;
        if (!std::getline(std::cin, line)) { std::cout << "\n"; break; }

        auto tokens = tokenize(line);
        if (tokens.empty()) continue;

        const std::string& cmd = tokens[0];
        try {
            if (cmd == "help" || cmd == "?") cmd_help();
            else if (cmd == "status" || cmd == "info") cmd_status();
            else if (cmd == "load") cmd_load(tokens);
            else if (cmd == "window") cmd_window(tokens);
            else if (cmd == "output-dir" || cmd == "dir") cmd_output_dir(tokens);
            else if (cmd == "prefix") cmd_prefix(tokens);
            else if (cmd == "step" || cmd == "next") cmd_step(tokens);
            else if (cmd == "run") cmd_run();
            else if (cmd == "reset") cmd_reset();
            else if (cmd == "print" || cmd == "show") cmd_print(tokens);
            else if (cmd == "save") cmd_save(tokens);
            else if (cmd == "quit" || cmd == "exit" || cmd == "q") break;
            else std::cout << "Unknown command: " << cmd
                           << "  (type 'help')\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
    std::cout << "Bye!\n";
}

//Command implementations

//help implementation, prints possible commands with what it is for
//inside <> means that values will be stored in settings, where [] won't be
void UI::cmd_help() {
    std::cout <<
        "Commands:\n"
        "  load <path>                  Open a CSV file (resets the engine)\n"
        "  window <N>                   Set the rolling window size (resets the engine)\n"
        "  output-dir <path>            Set output directory (default 'results')\n"
        "  prefix <name>                Set output file prefix (default 'stats')\n"
        "  step [N]                     Read next N rows (default 1) and update engine\n"
        "  run                          Read all remaining rows and update engine\n"
        "  print [means|cov|corr|all]   Print current stats (default 'all')\n"
        "  save [matrices|daily|all]    Save outputs to disk (default 'matrices')\n"
        "  status                       Show current state\n"
        "  reset                        Forget loaded data and engine state\n"
        "  help                         Show this help\n"
        "  quit                         Exit\n";
}

//method that prints current status
void UI::cmd_status() {
    std::cout << "Input file:   " << (input_path_.empty() ? "(none)" : input_path_) << "\n"
              << "Window size:  " << window_size_ << "\n"
              << "Output dir:   " << output_dir_ << "\n"
              << "Output prefix:" << output_prefix_ << "\n";
    if (engine_) {
        std::cout << "Engine:      " << engine_->cols() << "/" << engine_->capacity()
                  << " observations with " << engine_->rows() << " assets\n"
                  << "Rows read:    " << rows_processed_ << (eof_reached_ ? "  [EOF]" : "") << "\n";
    } else {
        std::cout << "Engine:      not initialized (load a file and step/run)\n";
    }
}

//method that loads input csv from provided path using CsvReader class
void UI::cmd_load(const std::vector<std::string>& tokens) {
    std::string path = (tokens.size() >= 2) ? tokens[1] : ask("Input CSV path", input_path_);
    if (path.empty()) { std::cout << "Cancelled.\n"; return; }
    input_path_ = path;

    reader_ = std::make_unique<CsvReader>(input_path_);
    headers_ = reader_->headers();
    if (headers_.empty()) {
        std::cout << "File has no header row.\n";
    } else {
        std::cout << "Loaded " << headers_.size() << " columns: ";
        for (std::size_t i = 0; i < headers_.size(); ++i) {
            std::cout << headers_[i];
            if (i + 1 < headers_.size()) {
                std::cout << ", ";
            }
            else {
                std::cout << "\n";
            }
        }
    }
    engine_.reset();
    daily_means_.clear();
    daily_vars_.clear();
    rows_processed_ = 0;
    eof_reached_ = false;
}

//sets size of window, if is set, then the engine restarts to work with new window
void UI::cmd_window(const std::vector<std::string>& tokens) {
    window_size_ = (tokens.size() >= 2) ? std::stoul(tokens[1]) : ask_size("Window size", window_size_);
    if (engine_) {
        std::cout << "Engine reset to use window " << window_size_ << ".\n";
        engine_.reset();
        daily_means_.clear();
        daily_vars_.clear();
        rows_processed_ = 0;
        if (!input_path_.empty()) {
            reader_ = std::make_unique<CsvReader>(input_path_);
            headers_ = reader_->headers();
            eof_reached_ = false;
        }
    } else {
        std::cout << "Window size set to " << window_size_ << ".\n";
    }
}

//Changes ouptut directory
void UI::cmd_output_dir(const std::vector<std::string>& tokens) {
    output_dir_ = (tokens.size() >= 2) ? tokens[1] : ask("Output directory", output_dir_);
    std::cout << "Output directory set to '" << output_dir_ << "'.\n";
}

//Method that changes the prefix of saved data
void UI::cmd_prefix(const std::vector<std::string>& tokens) {
    output_prefix_ = (tokens.size() >= 2) ? tokens[1] : ask("Output prefix", output_prefix_);
    std::cout << "Output prefix set to '" << output_prefix_ << "'.\n";
}

//load #of steps observation of data, default is 1
void UI::cmd_step(const std::vector<std::string>& tokens) {
    if (!reader_) { std::cout << "Load a file first.\n"; return; }
    if (eof_reached_) { std::cout << "Already at end of file.\n"; return; }
    ensure_engine();

    std::size_t n = 1;
    if (tokens.size() >= 2) n = std::stoul(tokens[1]);

    std::size_t did = 0;
    for (std::size_t i = 0; i < n; ++i) {
        if (!process_one_row()) break;
        ++did;
    }
    std::cout << "Read " << did << " row(s).  Engine now: " << engine_->cols() << "/" << engine_->capacity() << (eof_reached_ ? "  [EOF]" : "") << "\n";

    if (engine_->cols() >= 2) print_means();
}

//process the rest of rows in file
void UI::cmd_run() {
    if (!reader_) { std::cout << "Load a file first.\n"; return; }
    ensure_engine();
    std::size_t did = 0;
    while (process_one_row()) ++did;
    std::cout << "Processed " << did << " row(s).  Engine now: " << engine_->cols() << "/" << engine_->capacity() << "  [EOF]\n";
}

//method that resets the engine, meaning you must load file again
void UI::cmd_reset() {
    reader_.reset();
    engine_.reset();
    headers_.clear();
    daily_means_.clear();
    daily_vars_.clear();
    rows_processed_ = 0;
    eof_reached_ = false;
    input_path_.clear();
    std::cout << "State cleared. Settings stayed the same\n";
}

//method that prints current statistics. User can choose which statistics.
void UI::cmd_print(const std::vector<std::string>& tokens) {
    if (!engine_ || engine_->cols() == 0) {
        std::cout << "No data. Load and step/run first.\n";
        return;
    }
    std::string what = (tokens.size() >= 2) ? tokens[1] : "all";

    if (what == "means" || what == "all") print_means();
    if (engine_->cols() < 2 && (what == "cov" || what == "corr" || what == "all")) {
        std::cout << "(need >= 2 observations for cov/corr — currently "  << engine_->cols() << ")\n";
        return;
    }
    if (what == "cov"  || what == "all")
        print_matrix("Covariance",  engine_->covariance_matrix(),  headers_);
    if (what == "corr" || what == "all")
        print_matrix("Correlation", engine_->correlation_matrix(), headers_);
}

//method responsible for creating ouput directory and calling helper method to save
//by default us saves just final correlation and covariance matrix, but you can set daily to save daily means and variances
void UI::cmd_save(const std::vector<std::string>& tokens) {
    if (!engine_ || engine_->cols() < 2) {
        std::cout << "Engine has insufficient data to save (need >= 2 obs).\n";
        return;
    }

    std::string what = (tokens.size() >= 2) ? tokens[1] : "matrices";
    fs::path out_dir = output_dir_;
    fs::create_directories(out_dir);

    if (what == "matrices" || what == "all") {
        fs::path c = out_dir / (output_prefix_ + "_cov.csv");
        fs::path r = out_dir / (output_prefix_ + "_corr.csv");
        write_matrix(c.string(),  engine_->covariance_matrix(),  headers_);
        write_matrix(r.string(), engine_->correlation_matrix(), headers_);
        std::cout << "Saved " << c.string() << "\n" << "      " << r.string() << "\n";
    }

    if (what == "daily" || what == "all") {
        if (daily_means_.empty()) {
            std::cout << "(no daily series to save — they're collected during 'step'/'run')\n";
        } else {
            fs::path m = out_dir / (output_prefix_ + "_means_daily.csv");
            fs::path v = out_dir / (output_prefix_ + "_variances_daily.csv");
            write_time_series(m.string(), headers_, daily_means_);
            write_time_series(v.string(), headers_, daily_vars_);
            std::cout << "Saved " << m.string() << "  (" << daily_means_.size() << " rows)\n" << "      " << v.string() << "  (" << daily_vars_.size()  << " rows)\n";
        }
    }
}

//Rest of helper methods
//creates IncrementalStats that computes all statistics
void UI::ensure_engine() {
    if (!engine_) {
        if (headers_.empty())
            throw std::runtime_error("file has no header — can't size the engine");
        engine_ = std::make_unique<IncrementalStats<double>>(headers_.size(), window_size_);
    }
}

//method that reads one line of file, and stores daily results which can be saved later (means and variacnes)
bool UI::process_one_row() {
    std::vector<double> row;
    if (!reader_->next(row)) {
        eof_reached_ = true;
        return false;
    }
    engine_->push_column(row);
    ++rows_processed_;

    daily_means_.push_back(engine_->mean_vector());
    if (engine_->cols() >= 2) {
        std::vector<double> v(headers_.size());
        for (std::size_t i = 0; i < headers_.size(); ++i)
            v[i] = engine_->variance(i);
        daily_vars_.push_back(std::move(v));
    }
    return true;
}

//prints current means
void UI::print_means() const {
    std::cout << "Means (over " << engine_->cols() << " obs):\n";
    for (std::size_t i = 0; i < headers_.size(); ++i)
        std::cout << "  "  << headers_[i] << " = " << std::fixed << std::setprecision(6) << engine_->mean(i) << "\n";
}

std::vector<std::string> UI::tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string t;
    while (ss >> t) tokens.push_back(t);
    return tokens;
}

//method asks user which value it wants for prompt value. If they don't write anything previous values is kept
std::string UI::ask(const std::string& prompt, const std::string& current) {
    std::cout << prompt;
    if (!current.empty()) std::cout << " [" << current << "]";
    std::cout << ": ";
    std::string line;
    std::getline(std::cin, line);
    return line.empty() ? current : line;
}

//mathod that asks user the size, this method uses ask method.
std::size_t UI::ask_size(const std::string& prompt, std::size_t current) {
    std::string s = ask(prompt, std::to_string(current));
    try { return std::stoul(s); }
    catch (...) { return current; }
}

//method that prints matrix to cout.
void UI::print_matrix(const std::string& title, const Matrix<double>& m, const std::vector<std::string>& labels) {
    std::cout << "\n" << title << ":\n          ";
    for (auto&& name : labels) std::cout << std::setw(10) << name;
    std::cout << "\n";
    for (std::size_t i = 0; i < m.rows(); ++i) {
        std::cout << std::setw(8) << labels[i] << "  ";
        for (std::size_t j = 0; j < m.cols(); ++j)
            std::cout << std::setw(10) << std::fixed << std::setprecision(6) << m(i, j);
        std::cout << "\n";
    }
}

void UI::write_time_series(const std::string& path, const std::vector<std::string>& asset_names, const std::vector<std::vector<double>>& rows) {
    std::ofstream f(path);
    if (!f.is_open())
        throw std::runtime_error("cannot open " + path + " for writing");
    f << "day";
    for (auto&& name : asset_names) f << ',' << name;
    f << '\n';
    for (std::size_t t = 0; t < rows.size(); ++t) {
        f << (t + 1);
        for (auto v : rows[t]) f << ',' << v;
        f << '\n';
    }
}

