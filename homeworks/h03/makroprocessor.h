#ifndef CPP_HOMEWORKS_MAKROPROCESSOR_H
#define CPP_HOMEWORKS_MAKROPROCESSOR_H
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <memory>

constexpr char EOL = '\n';
constexpr char HASHTAG = '#';
constexpr char SPACE = ' ';
constexpr char EROOR_OUTPUT[] = "Error";
using ptr = std::shared_ptr<std::string>;
using map_value = std::vector<ptr>;
using makro_map = std::unordered_map<std::string, map_value>;


enum READING_MODE {
    READING_WORD, READING_MAKRONAME, READING_MAKRODEFINITION, ERROR, READING_WHITESPACE
};
class makroprocessor {
public:
    makroprocessor(std::istream& is= std::cin, std::ostream& os = std::cout);
    void process_arg(std::vector<std::string>& args);
    void print_error_output() const;
    void read_input();
    void expand_word(const std::string& w, map_value& out);
    void clear_word();
private:
    void process(char c);
    READING_MODE mode_ = READING_WHITESPACE;
    std::string next_word_;
    makro_map makros_;
    std::string current_makro_name_;
    std::istream& in_;
    std::ostream& out_;
};


#endif //CPP_HOMEWORKS_MAKROPROCESSOR_H