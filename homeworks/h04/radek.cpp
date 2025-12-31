#include "radek.h"
using namespace std;
void radek::print_line(std::ostream &os) const {
    bool first = true;
    for (auto && r : radek_) {
        if (!first) os << separator_;
        os << *r;
        first = false;
    }
}
radek::radek(vect_ptr &&radek, char separator) : radek_(std::move(radek)), separator_(separator) {}


ostream &operator<<(ostream &os, const radek &r) {
    r.print_line(os);
    return os;
}
std::ostream& operator<<(std::ostream& os, const AbstractVal& av) {
    av.print(os);
    return os;
}
