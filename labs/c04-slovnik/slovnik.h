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
    void find(const std::string& slovo, std::ostream& os = std::cout) const;
    void prefix(const std::string& prefix, std::ostream& os = std::cout) const;
private:
    std::map<std::string, std::set<std::string>> dict_;
    std::set<std::string> empty_;
    std::map<std::string, std::set<std::string>> prefix_result_;
};


#endif //CPP_LABS_SLOVNIK_H