#ifndef CPP_HOMEWORKS_RADEK_H
#define CPP_HOMEWORKS_RADEK_H
#include <iostream>
#include <vector>
#include <memory>
class AbstractVal {
public:
    virtual void print(std::ostream& os) const = 0;
    virtual bool operator< (const AbstractVal& rhs) const =0;
};

template<typename T>
class ConcreteVal : public AbstractVal {
private:
    T val_;
public:
    ConcreteVal(T&& val);
    void print(std::ostream& os) const override;
    bool operator< (const AbstractVal& rhs) const override;
};
template<typename T>
ConcreteVal<T>::ConcreteVal(T&& val):val_(std::move(val)){}

template<typename T>
void ConcreteVal<T>::print(std::ostream & os) const {os << val_;}

template<typename T>
bool ConcreteVal<T>::operator<(const AbstractVal& rhs) const {
    return val_ < static_cast<const ConcreteVal<T>&>(rhs).val_;
}

std::ostream& operator<<(std::ostream& os, const AbstractVal& r);

using u_ptr = std::unique_ptr<AbstractVal>;
using vect_ptr = std::vector<u_ptr> ;
class radek {
private:
    vect_ptr radek_;
    char separator_{' '};

public:
    void print_line(std::ostream& os=std::cout) const;
    radek(vect_ptr&& radek, char separator = ' ');
    const AbstractVal& operator[](size_t i) const {return *radek_[i-1];} //minus 1 protože se počítá od 1 a ne 0
};

std::ostream& operator<<(std::ostream& os, const radek& r);



#endif //CPP_HOMEWORKS_RADEK_H