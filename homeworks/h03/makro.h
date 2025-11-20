#ifndef CPP_HOMEWORKS_MAKRO_H
#define CPP_HOMEWORKS_MAKRO_H
#include <string>
#include <iostream>

class makro {
public:
    makro(std::string&& name, std::string&& definition);
    void print_definition(std::ostream& os = std::cout) const;
private:
    std::string name_;
    std::string definition_;
};

std::ostream& operator<<(std::ostream& os, const makro& m);
#endif