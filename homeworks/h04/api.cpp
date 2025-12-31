#include <stdexcept>
#include <cctype>
#include <string>
#include <vector>
#include "api.h"
#include <fstream>

void api::process_arguments(std::vector<std::string>& args) {
    size_t i = 0;
    while (i < args.size() && args[i].size() > 1 && args[i][0] == '-') {
        std::string opt = std::move(args[i]);

        if (opt.rfind("-i", 0) == 0) {
            if (opt.size() > 2) {
                input_file_ = opt.substr(2);
            } else {
                if (++i >= args.size()) { //není název filu
                    throw std::runtime_error("Incorrect arguments 2");
                }

                input_file_ = args[i];
            }
        }
        else if (opt.rfind("-o", 0) == 0) {
            if (opt.size() > 2) {
                output_file_ = opt.substr(2);
            } else {
                if (++i >= args.size()) { //není název filu
                    throw std::runtime_error("Incorrect arguments 3");
                }

                output_file_ = args[i];
            }
        }
        else if (opt.rfind("-s", 0) == 0) {
            if (opt.size() > 2) {
                separator_ = opt[2];
            } else {
                if (++i >= args.size()) { //není separator
                    throw std::runtime_error("Incorrect arguments 4");
                }

                if (args[i].empty()) { //neni separator
                    throw std::runtime_error("Incorrect argument 5");
                }

                separator_ = args[i][0];
            }
        }
        else { //neco necekaného
            throw std::runtime_error("Incorrect arguments 6");
        }
        ++i;
    }

    while (i < args.size()) {
        const std::string &arg = args[i];

        if (arg.size() < 2) { //příliž kratké typy
            throw std::runtime_error("Incorrect arguments 7");
        }


        char tchar = arg[0];
        Type t;
        if (tchar == 'S')       t = Type::STRING;
        else if (tchar == 'N')  t = Type::INT;
        else { //nedefinovaný typ
            throw std::runtime_error("Incorrect arguments 8");
        }


        std::string colstr = arg.substr(1);
        for (char c : colstr)
            if (!std::isdigit((unsigned char)c)) { //nejsou pouze cisla
                throw std::runtime_error("Incorrect arguments 9");
            }


        int col = std::stoi(colstr);
        if (col <= 0) { //nejsou žádna čísla
            throw std::runtime_error("Incorrect arguments 10");
        }


        order_.push_back(col);
        map_[col] = t;

        ++i;
    }
}

void api::read_input(){
    Table table(std::move(order_), std::move(map_), separator_);
    read_lines_and_fill(table);
    table.sort_table();
    write_output(table);
}


void api::read_lines_and_fill(Table& table) {
    if (!input_file_.empty()) {
        std::ifstream fin(input_file_);
        if (!fin.is_open()) {//soubor nejde otevrit nebo neni
            throw std::runtime_error("Cannot open input file: " + input_file_);
        }
        read_stream(fin, table);
    } else {
        read_stream(std::cin, table);
    }
}
void api::read_stream(std::istream& in, Table& table) {
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty())
            continue;
        table.add_radek(line);
    }
}
void api::write_output(const Table& table) {
    if (!output_file_.empty()) {
        std::ofstream fout(output_file_);
        if (!fout.is_open()) { //soubor nejde otevrit nebo neni
            throw std::runtime_error("Cannot open output file: " + output_file_);
        }
        table.print_table(fout);
    } else {
        table.print_table(std::cout);
    }
}