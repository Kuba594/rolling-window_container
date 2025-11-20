
#include "makro.h"
using namespace std;
makro::makro(string&& name, string&& definition) : name_(std::move(name)), definition_(std::move(definition)){}
void makro::print_definition(ostream &os) const {
    os << definition_;
}
ostream& operator<<(ostream& os, const makro& m) {
    m.print_definition(os);
    return os;
}
