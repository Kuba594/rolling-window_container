#include "AbstractVal.h"
#include <iostream>
std::ostream& operator<<(std::ostream& os, const AbstractVal& av) {
    av.print(os);
    return os;
}
