//
// Created by Jakub Michalski on 25.11.2025.
//

#ifndef CPP_LABS_SEZNAM_H
#define CPP_LABS_SEZNAM_H
#include "AbstValue.h"
#include <vector>

class Seznam {
public:
    void add( Valptr p);
    void print();
    Seznam() {}
    Seznam( const Seznam& s) { clone( s); }
    Seznam& operator=(const Seznam& s) { pole_.clear(); clone( s); return *this; }
private:
    void clone( const Seznam& s)
    { for( auto&& x : s.pole_) pole_.push_back( x->clone()); }
    std::vector<Valptr> pole_;
};
inline void Seznam::print() {
    for (auto && x : pole_) x->print();
}
inline void Seznam::add( Valptr p) { pole_.push_back(std::move(p)); }



#endif //CPP_LABS_SEZNAM_H