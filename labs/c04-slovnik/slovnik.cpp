#include "slovnik.h"
#include <iostream>
using namespace std;

void slovnik::add(const string& word, const string& definition) {
    dict_[word].insert(definition);
}

void slovnik::del(const string& word, const string& definition) {
    auto it = dict_.find(word);
    if (it != dict_.end()) {
        it->second.erase(definition);
        if (it->second.empty()) {
            dict_.erase(it);
        }
    }
}

void slovnik::del(const string& word) {
    dict_.erase(word);
}
void slovnik::find(const string& word, ostream& os) const {
    auto it = dict_.find(word);
    if (it != dict_.end()) {
        const auto& defs = it->second;
        for (auto dit = defs.begin(); dit != defs.end(); ++dit) {
            if (dit != defs.begin()) os << ' ';
            os << *dit;
        }
    }
    os << '\n';
}

void slovnik::prefix(const string& pref, std::ostream& os) const {
    auto it = dict_.lower_bound(pref);
    bool first_word = true;

    while (it != dict_.end() && it->first.starts_with(pref)) {
        if (!first_word) os << '\n';
        first_word = false;

        os << it->first << ": ";
        const auto& defs = it->second;
        for (auto dit = defs.begin(); dit != defs.end(); ++dit) {
            if (dit != defs.begin()) os << ' ';
            os << *dit;
        }
        ++it;
    }
}