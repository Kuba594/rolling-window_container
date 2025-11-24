#include "makroprocessor.h"
#include <iostream>
#include <sstream>
#include <memory>
using namespace std;
makroprocessor::makroprocessor(istream &is, ostream &os) : in_(is), out_(os){}

void makroprocessor::process_arg(std::vector<std::string> &args) {
    if (args.empty())
        return;

    const std::string &name = args[0];

    if (!isalpha(name[0]) || args.size() == 1) {
        mode_ = ERROR;
        return;
    }

    vector<shared_ptr<string>> body;

    for (size_t i = 1; i < args.size(); ++i) {
        body.push_back(std::make_shared<string>(args[i]));
    }
    makros_[name] = std::move(body);
}
void makroprocessor::print_error_output() const {
    if (mode_ == ERROR) {
        out_<< EROOR_OUTPUT <<endl;
        return;
    }

}
void makroprocessor::read_input() {
    char c;
    for (;;) {
        if (mode_ == ERROR) {
            print_error_output();
            return;
        }
        c = in_.get();
        if (in_.fail()) {
            if (mode_ == READING_MAKRODEFINITION) {
                mode_ = ERROR;
                print_error_output();
            }
            return;
        }
        process(c);
    }
}
void makroprocessor::expand_word(const string& w, map_value& out)
{
    auto it = makros_.find(w);
    if (it == makros_.end()) {
        out.push_back(std::make_shared<std::string>(w));
    } else {
        out.insert(out.end(), it->second.begin(), it->second.end());
    }
}
void makroprocessor::clear_word() {
    if (next_word_.empty()) return;

    auto it = makros_.find(next_word_);
    if (it == makros_.end()) {
        out_ << next_word_ << SPACE;
    } else {
        for (auto &p : it->second)
            out_ << *p << SPACE;
    }
    next_word_.clear();
}
void makroprocessor::process(char c) {
    if (c == EOL) {
        if (mode_ == READING_WORD)
            clear_word();
        out_ << EOL;
        mode_ = READING_WHITESPACE;
        return;
    }
    switch (mode_) {
        case READING_WHITESPACE:
            if (c == HASHTAG) {
                next_word_.clear();
                mode_ = READING_MAKRONAME;
            } else if (!isspace(c)) {
                next_word_ = c;
                mode_ = READING_WORD;
            }
            break;

        case READING_WORD:
            if (isspace(c)) {
                clear_word();
                mode_ = READING_WHITESPACE;
            } else {
                next_word_ += c;
            }
            break;

        case READING_MAKRONAME:
            if (!isspace(c)) {
                next_word_ += c;
            } else {
                if (next_word_.empty() || !isalpha(next_word_[0])) {
                    mode_ = ERROR;
                    return;
                }
                current_makro_name_ = std::move(next_word_);
                next_word_.clear();
                makros_[current_makro_name_].clear();
                mode_ = READING_MAKRODEFINITION;
            }
            break;

        case READING_MAKRODEFINITION:
            if (c == HASHTAG) {
                int next = in_.peek();

                if (next == EOF) {
                    mode_ = ERROR;
                    return;
                }

                if (!std::isspace(next)) {
                    mode_ = ERROR;
                    return;
                }

                if (!next_word_.empty()) {
                    expand_word(next_word_, makros_[current_makro_name_]);
                    next_word_.clear();
                }

                mode_ = READING_WHITESPACE;
                break;
            }

            if (!isspace(c)) {
                next_word_ += c;
            } else {
                if (!next_word_.empty()) {
                    expand_word(next_word_, makros_[current_makro_name_]);
                    next_word_.clear();
                }
            }
            break;

        case ERROR:
            break;
    }
}