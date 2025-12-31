#ifndef CPP_HOMEWORKS_ABSTRACTVAL_H
#define CPP_HOMEWORKS_ABSTRACTVAL_H
#include <iostream>

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


#endif //CPP_HOMEWORKS_ABSTRACTVAL_H