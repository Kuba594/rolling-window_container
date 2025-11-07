#include "frequencies.h"

#include <iostream>
#include <ostream>
#include <fstream>
using namespace std;
void frequencies::process_word(const std::string &word) {
    auto it = data_.find(word);
    if (it != data_.end()) {
        ++it->second;
        return;
    }
    data_[word] = 1;
}
void frequencies::reformat_stored_frequencies() {
    for (auto && word : data_) {
        output_data_[word.second].push_back(word.first);
    }
}
void frequencies::return_results(int ith_word) {
    int counter = 0;
    fstream out;
    for (auto && word : output_data_) {
        if (counter != ith_word) {
            ++counter;
            continue;
        }
        out << ith_word << ": ";
        for (auto && freq : word.second) {
            out << freq << " ";
        }
        out << endl;
        count << out;
        return;
    }
}


