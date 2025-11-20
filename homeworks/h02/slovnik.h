#ifndef CPP_LABS_SLOVNIK_H
#define CPP_LABS_SLOVNIK_H
#include <string>
#include <map>
#include <set>
#include <iostream>

class slovnik {
public:
    void add(const std::string& word, const std::string& definition);
    void del(const std::string& word, const std::string& definition);
    void del(const std::string& word);
    const std::set<std::string>*  find(const std::string& slovo, std::ostream& os = std::cout) const;
    std::pair<std::map<std::string, std::set<std::string>>::const_iterator, std::map<std::string, std::set<std::string>>::const_iterator> prefix(const std::string& prefix, std::ostream& os = std::cout) const;
private:
    std::map<std::string, std::set<std::string>> dict_;
};


#endif //CPP_LABS_SLOVNIK_H