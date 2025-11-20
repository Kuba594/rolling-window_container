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
const std::set<std::string>* slovnik::find(const string& word, ostream& os) const {
    auto it = dict_.find(word);
    if (it == dict_.end()) return nullptr;
    return &it->second;
}

pair<map<string, set<string>>::const_iterator, map<string, set<string>>::const_iterator> slovnik::prefix(const string& pref, std::ostream& os) const {
    auto it = dict_.lower_bound(pref);
    auto end = it;
    while (end != dict_.end() && end->first.starts_with(pref)) {
        ++end;
    }
    return {it, end};
}