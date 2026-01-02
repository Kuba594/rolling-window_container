#include "Seznam.h"

void Seznam::print() {
    for (auto && x : pole_) x->print();
}

void Seznam::add( Valptr p) { pole_.push_back(std::move(p)); }

void Seznam::clone( const Seznam& s) {
    for( auto&& x : s.pole_) pole_.push_back( x->clone());
}

Seznam::Seznam( const Seznam& s) { clone( s); }
Seznam::Seznam(){}

Seznam& Seznam::operator=(const Seznam& s) {
    if( this == &s)  return *this;pole_.clear();
    clone( s);
    return *this;
}