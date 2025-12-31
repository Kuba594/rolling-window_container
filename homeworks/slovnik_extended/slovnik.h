#ifndef CPP_LABS_SLOVNIK_H
#define CPP_LABS_SLOVNIK_H
#include <string>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>

struct case_insensitive_order {
    bool operator()(const std::string& a, const std::string& b) const;
};

struct word_length_order {
    bool operator()(const std::string& a, const std::string& b) const;
};

using two_iterators =std::pair<std::map<std::string, std::set<std::string>>::const_iterator, std::map<std::string, std::set<std::string>>::const_iterator> ;

using mnozina = std::set<std::string, word_length_order>;
using mapa = std::map<std::string, mnozina, case_insensitive_order>;
using prefix_iterators = std::pair<mapa::const_iterator,mapa::const_iterator>;

class slovnik {
private:
    mapa dict_;
public:
    void add(const std::string& word, const std::string& definition);
    void del(const std::string& word, const std::string& definition);
    void del(const std::string& word);
    const mnozina*  find(const std::string& slovo, std::ostream& os = std::cout) const;
    prefix_iterators prefix(const std::string& prefix, std::ostream& os = std::cout) const;
};


#endif //CPP_LABS_SLOVNIK_H