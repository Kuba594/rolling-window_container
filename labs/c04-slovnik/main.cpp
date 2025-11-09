#include "slovnik.h"
#include "aplikace.h"
using namespace std;

int main(int argc, char ** argv) {
    slovnik s;
    aplikace a(s);
    a.run();
    return 0;
}