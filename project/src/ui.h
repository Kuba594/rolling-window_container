#ifndef PROJECT_UI_H
#define PROJECT_UI_H
#include <filesystem>
#include <memory>
#include <string>
#include <vector>
#include "data_loader.h"
#include "incremental_stats.h"

//class responsible for Interactive Interface
class UI {
public:
    void run();

private:
    //settings that can be changed
    std::string input_path_;
    std::size_t window_size_ = 30;
    std::string output_dir_ = "results";
    std::string output_prefix_ = "stats";

    //inside info, current values
    std::unique_ptr<CsvReader> reader_;
    std::unique_ptr<IncrementalStats<double>> engine_;
    std::vector<std::string> headers_;
    std::vector<std::vector<double>> daily_means_;
    std::vector<std::vector<double>> daily_vars_;
    std::size_t rows_processed_ = 0;
    bool eof_reached_ = false;

    //helper commands
    void cmd_help();
    void cmd_status();
    void cmd_load(const std::vector<std::string>&);
    void cmd_window(const std::vector<std::string>&);
    void cmd_output_dir(const std::vector<std::string>&);
    void cmd_prefix(const std::vector<std::string>&);
    void cmd_step(const std::vector<std::string>&);
    void cmd_run();
    void cmd_reset();
    void cmd_print(const std::vector<std::string>&);
    void cmd_save(const std::vector<std::string>&);

    //rest of helping methods
    void ensure_engine();
    bool process_one_row();
    void print_means() const;
    std::vector<std::string> tokenize(const std::string& line);
    std::string ask(const std::string& prompt, const std::string& current);
    std::size_t ask_size(const std::string& prompt, std::size_t current);
    void print_matrix(const std::string& title, const Matrix<double>& m, const std::vector<std::string>& labels);
    void write_time_series(const std::string& path, const std::vector<std::string>& asset_names, const std::vector<std::vector<double>>& rows);
};


#endif //PROJECT_UI_H
