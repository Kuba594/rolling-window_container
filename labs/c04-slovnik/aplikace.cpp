#include "aplikace.h"
#include <sstream>
using namespace std;

void aplikace::run() {
    string line;
    while (getline(in_, line)) {
        istringstream iss(line);
        string cmd;
        if (!(iss >> cmd)) continue;
        if (cmd == "add") {
            add_word(iss);
            continue;
        }
        if (cmd == "del") {
            del_word(iss);
            continue;
        }
        if (cmd == "find") {
            print_find(iss);
            continue;
        }
        if (cmd == "prefix") {
            print_prefix(iss);
        }
    }
}
void aplikace::add_word(istringstream &iss) {
    string w, d;
    if (iss >> w >> d)
        s_.add(w, d);
}
void aplikace::del_word(istringstream &iss) {
    string word, def;
    if (!(iss >> word)) return;
    if (iss >> def) {
        s_.del(word, def);
    }
    else s_.del(word);
}
void aplikace::print_find(istringstream &iss) const {
    string word;
    if (!(iss >> word)) return;
    auto defs = s_.find(word);
    if (!defs) {return; }
    bool first = true;
    for ( auto && t : *defs) {
        if (!first) out_ << ' ';
        out_ << t;
        first = false;
    }
    out_ << '\n';
}
void aplikace::print_prefix(istringstream &iss) const {
    string pref;
    if (!(iss >> pref)) return;
    auto [it, end] = s_.prefix(pref);
    while (it != end) {
        out_ << it->first << ":";
        for  (auto && t : it->second) {
            out_ << ' ';
            out_ << t;
        }
        out_ << '\n';
        ++it;
    }
}

