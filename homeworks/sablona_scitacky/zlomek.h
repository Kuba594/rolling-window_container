#ifndef CPP_HOMEWORKS_ZLOMEK_H
#define CPP_HOMEWORKS_ZLOMEK_H
#include <iostream>
template<typename T> class Zlomek {
public:
    Zlomek(T citatel, T jmenovatel) : citatel_(citatel), jmenovatel_(jmenovatel) {}
    template<typename X>
    friend Zlomek<X> operator+(const Zlomek<X>& z1, const Zlomek<X>& z2);
    template<typename X>
    friend std::ostream& operator<<(std::ostream& os,
                                    const Zlomek<X>& z);
    void print(std::ostream& os=std::cout) const;
private:
    T citatel_;
    T jmenovatel_;
};

template<typename X>
Zlomek<X> operator+(const Zlomek<X>& z1, const Zlomek<X>& z2) {
    X citatel = z1.citatel_*z2.jmenovatel_ + z2.citatel_*z1.jmenovatel_;
    X jmenovatel = z1.jmenovatel_*z2.jmenovatel_;
    return Zlomek<X>(citatel, jmenovatel);
}
template<typename T>
void Zlomek<T>::print(std::ostream& os) const {
    os << citatel_ << "/" << jmenovatel_ << std::endl;
}
template<typename X>
std::ostream& operator<<(std::ostream& os, const Zlomek<X>& z)
{
    return os << z.citatel_ << "/" << z.jmenovatel_;
}

template<typename T> class DA
{
public:
    DA( T val=T{}) : v_(val) {}
    template<typename X>
    friend DA<X> operator+
      ( const DA<X>& x, const DA<X>& y);
    void print(std::ostream& os=std::cout) const;
    template<typename X>
    friend std::ostream& operator<<(std::ostream& os,
                                    const DA<X>& da);
private:
    T v_;
};

template<typename X>
DA<X> operator+( const DA<X>&x, const DA<X>&y)
{ return DA<X> { x.v_ + x.v_ + y.v_+ y.v_}; }

template<typename T>
void DA<T>::print(std::ostream& os) const {
    v_.print(os);
}
template<typename X>
std::ostream& operator<<(std::ostream& os, const DA<X>& da){
    return os << da.v_;
}

#endif //CPP_HOMEWORKS_ZLOMEK_H