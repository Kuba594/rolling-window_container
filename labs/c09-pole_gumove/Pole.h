#ifndef CPP_LABS_POLE_H
#define CPP_LABS_POLE_H
#include <vector>
#include <memory>
#include "myexpt.h"

template<typename T> class Pole {
public:
    Pole( size_t chunk = 100);
    void push_back( const T& x);
    T& operator[] ( size_t i);
    T& at( int i);

private:
    //void check( size_t i);
    void resize();
    size_t size_{0};
    size_t capacity_{0};
    size_t chunk_size_;

    std::vector< std::unique_ptr<T[]>> hrabe_;
};
template<typename T>
T &Pole<T>::at(int i) {
    if (i >= size_) throw myexpt(i, size_);
    if (i < 0) throw myexpt(i, size_);

    return this->operator[](i);
}

template<typename T>
Pole<T>::Pole(size_t chunk) :  chunk_size_(chunk){
    resize();
}

template<typename T>
T& Pole<T>::operator[] ( size_t i) { return hrabe_[i/chunk_size_][i%chunk_size_]; }

template<typename T>
void Pole<T>::push_back(const T &x) {
    if (size_ == capacity_) resize();
    hrabe_[size_/chunk_size_][size_%chunk_size_] = x;
    ++size_;
}

template<typename T>
void Pole<T>::resize() {
    auto new_chunk = std::make_unique<T[]>(chunk_size_);
    hrabe_.push_back(std::move(new_chunk));
    capacity_ += chunk_size_;
}



#endif //CPP_LABS_POLE_H