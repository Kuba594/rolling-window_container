#include "slovnik.h"
#include <iostream>
using namespace std;

int main(int argc, char ** argv) {
    slovnik s;

    s.add("hello", "world");
    s.add("hello", "ahoj");
    s.add("help", "pomoc");
    s.add("helium", "chemický prvek");

    std::cout << "--- Překlady pro 'hello' ---\n";
    s.find("hello");

    std::cout << "\n--- Prefix 'he' ---\n";
    s.prefix("he");

    std::cout << "\n--- Mazání ---\n";
    s.del("hello", "world");
    s.del("help");

    std::cout << "\n--- Překlady po mazání ---\n";
    s.find("hello");


    std::cout << "\n--- Hledání nenalezeného slova ---\n";
    s.find("help");

    std::cout << "\n--- Prefix 'he' po mazání ---\n";
    s.prefix("he");

    return 0;
}