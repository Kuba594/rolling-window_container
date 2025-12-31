#include "slovnik.h"
#include <iostream>
using namespace std;


bool case_insensitive_order::operator()(const string& a, const string& b) const {
    return lexicographical_compare(
        a.begin(), a.end(),
        b.begin(), b.end(),
        [](unsigned char x, unsigned char y) {
            return tolower(x) < tolower(y);
        });
}

bool word_length_order::operator()(const string& a, const string& b) const {
    if (a.size() != b.size())
        return a.size() < b.size();
    return a < b;
}

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
const mnozina* slovnik::find(const string& word, ostream&) const {
    auto it = dict_.find(word);
    if (it == dict_.end()) return nullptr;
    return &it->second;
}

prefix_iterators slovnik::prefix(const string& pref, ostream&) const {
    auto it = dict_.lower_bound(pref);
    auto end = it;

    while (end != dict_.end() &&
           end->first.size() >= pref.size() &&
           equal(pref.begin(), pref.end(),
                 end->first.begin(),
                 [](unsigned char a, unsigned char b) {
                     return tolower(a) == tolower(b);
                 }))
    {
        ++end;
    }
    return {it, end};
}