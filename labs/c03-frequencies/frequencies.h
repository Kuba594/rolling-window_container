#ifndef CPP_LABS_FREQUENCIES_H
#define CPP_LABS_FREQUENCIES_H
#include <string>
#include <unordered_map>
#include <map>
#include <vector>

class frequencies {
public:
    void process_word(const std::string& word);
    void reformat_stored_frequencies();
    void return_results(int ith_word);
private:
    std::unordered_map<std::string, int> data_;
    std::map<int, std::vector<std::string>> output_data_;
};


#endif //CPP_LABS_FREQUENCIES_H