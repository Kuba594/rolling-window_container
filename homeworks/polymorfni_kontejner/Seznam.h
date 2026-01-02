
#ifndef CPP_HOMEWORKS_SEZNAM_H
#define CPP_HOMEWORKS_SEZNAM_H
#include "AbstValue.h"
#include <vector>


class Seznam {
public:
    void add( Valptr p);
    void print();
    Seznam();
    Seznam( const Seznam& s);
    Seznam& operator=(const Seznam& s);
private:
    void clone( const Seznam& s);
    std::vector<Valptr> pole_;
};





#endif //CPP_HOMEWORKS_SEZNAM_H