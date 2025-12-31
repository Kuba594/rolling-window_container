#ifndef CPP_HOMEWORKS_API_H
#define CPP_HOMEWORKS_API_H
#include <iostream>
#include <unordered_map>
#include "Table.h"

enum TYPE_OF_ARGUMENT {INPUT_FILE,OUTPUT_FILE,SEPARATOR,TYPE};

class api {
public:
    void process_arguments(std::vector<std::string>& args);
    void read_input();
    void read_lines_and_fill(Table& table);
    void write_output(const Table& table);

private:
    void read_stream(std::istream& in, Table& table);
    std::string input_file_;
    std::string output_file_;
    char separator_ = ' ';
    std::vector<int> order_;
    std::unordered_map<int,Type> map_;
};


#endif //CPP_HOMEWORKS_API_H