#ifndef CPP_LABS_APLIKACE_H
#define CPP_LABS_APLIKACE_H
#include "slovnik.h"

class aplikace {
private:
    slovnik& s_;
    std::istream &in_;
    std::ostream &out_;
    void add_word(std::istringstream &iss);
    void del_word(std::istringstream &iss);
    void print_find(std::istringstream &iss) const;
    void print_prefix(std::istringstream &iss) const;
public:
    aplikace(slovnik &dict, std::istream &in = std::cin, std::ostream &out = std::cout);
    void run();

};


#endif //CPP_LABS_APLIKACE_H