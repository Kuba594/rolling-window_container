#ifndef CPP_HOMEWORKS_STABLE_VECTOR_H
#define CPP_HOMEWORKS_STABLE_VECTOR_H
#include <memory>
#include <vector>
#include <stdexcept>


class stable_vector_exception : public std::out_of_range {
public:
    stable_vector_exception(size_t index): out_of_range("stable_vector: index out of range"), index_(index) {}
    size_t get_index() const { return index_;} ;

private:
    size_t index_;

};




template<typename T> class stable_vector {
public:
    stable_vector( size_t chunk = 100);

    stable_vector(const stable_vector& other);
    stable_vector& operator=(const stable_vector& other);
    void push_back( const T& x);
    T& operator[] ( size_t i);
    T& at( size_t i);

    const T& operator[](size_t i) const;

    const T& at(size_t i) const;
    class iterator;
    class const_iterator;
    iterator begin();
    const_iterator cbegin() const;

    iterator end();
    const_iterator cend() const;

    const_iterator begin() const;
    const_iterator end() const;
private:
    //void check( size_t i);
    void resize();
    size_t size_{0};
    size_t capacity_{0};
    size_t chunk_size_;

    std::vector< std::unique_ptr<T[]>> hrabe_;
};
template<typename T>
stable_vector<T>::const_iterator stable_vector<T>::end() const { return cend(); }
template<typename T>
stable_vector<T>::const_iterator stable_vector<T>::begin() const { return cbegin(); }

template<typename T>
stable_vector<T>& stable_vector<T>::operator=(const stable_vector& other) {
    if (this == &other) return *this;
    hrabe_.clear();
    size_ = other.size_;
    chunk_size_ = other.chunk_size_;

    for (const auto& chunk_ptr : other.hrabe_) {
        auto new_chunk = std::make_unique<T[]>(chunk_size_);
        for (size_t i = 0; i < chunk_size_; ++i) {
            new_chunk[i] = chunk_ptr[i];
        }
        hrabe_.push_back(std::move(new_chunk));
        capacity_ += chunk_size_;
    }
    return *this;
}

template<typename T>
stable_vector<T>::stable_vector(const stable_vector& other)
        : size_(other.size_), chunk_size_(other.chunk_size_) {

    for (const auto& chunk_ptr : other.hrabe_) {
        auto new_chunk = std::make_unique<T[]>(chunk_size_);
        for (size_t i = 0; i < chunk_size_; ++i) {
            new_chunk[i] = chunk_ptr[i];
        }
        hrabe_.push_back(std::move(new_chunk));
        capacity_ += chunk_size_;
    }
}
template<typename T>
const T& stable_vector<T>::operator[] (size_t i) const {
    return hrabe_[i / chunk_size_][i % chunk_size_];
}
template<typename T>
const T& stable_vector<T>::at(size_t i) const {
    if (i >= size_) throw stable_vector_exception(i);
    return (*this)[i];
}

template<typename T>
class stable_vector<T>::iterator{
public:
    iterator(stable_vector<T>* vec, size_t pos);
    T& operator*();
    iterator& operator++();
    bool operator!=(const iterator& other) const;
private:
    stable_vector<T>* vec_;
    size_t pos_;
};

template<typename T>
stable_vector<T>::iterator::iterator(stable_vector<T>* vec, size_t pos):vec_(vec), pos_(pos){}

template<typename T>
T& stable_vector<T>::iterator::operator*() { return vec_->at(pos_);}

template<typename T>
typename stable_vector<T>::iterator& stable_vector<T>::iterator::operator++() {
    ++pos_;
    return *this;
}
template<typename T>
bool stable_vector<T>::iterator::operator!=(const iterator &other) const {
    return pos_ != other.pos_;
}

template<typename T>
class stable_vector<T>::const_iterator{
public:
    const_iterator(const stable_vector<T>* vec, size_t pos);
    const T& operator*();
    const_iterator& operator++();
    bool operator!=(const const_iterator& other) const;

private:
    const stable_vector<T>* vec_;
    size_t pos_;
};

template<typename T>
stable_vector<T>::const_iterator::const_iterator(const stable_vector<T>* vec, size_t pos):vec_(vec), pos_(pos){}

template<typename T>
const T& stable_vector<T>::const_iterator::operator*() { return vec_->at(pos_);}

template<typename T>
typename stable_vector<T>::const_iterator& stable_vector<T>::const_iterator::operator++() {
    ++pos_;
    return *this;
}
template<typename T>
bool stable_vector<T>::const_iterator::operator!=(const const_iterator &other) const {
    return pos_ != other.pos_;
}

template<typename T>
T &stable_vector<T>::at(size_t i) {
    if (i >= size_) throw stable_vector_exception(i);

    return this->operator[](i);
}

template<typename T>
stable_vector<T>::stable_vector(size_t chunk) :  chunk_size_(chunk){
    resize();
}

template<typename T>
T& stable_vector<T>::operator[] ( size_t i) { return hrabe_[i/chunk_size_][i%chunk_size_]; }

template<typename T>
void stable_vector<T>::push_back(const T &x) {
    if (size_ == capacity_) resize();
    hrabe_[size_/chunk_size_][size_%chunk_size_] = x;
    ++size_;
}

template<typename T>
void stable_vector<T>::resize() {
    auto new_chunk = std::make_unique<T[]>(chunk_size_);
    hrabe_.push_back(std::move(new_chunk));
    capacity_ += chunk_size_;
}
template<typename T>
typename stable_vector<T>::iterator stable_vector<T>::begin() { return iterator(this, 0); }

template<typename T>
typename stable_vector<T>::const_iterator stable_vector<T>::cbegin()const{return const_iterator(this, 0);}

template<typename T>
typename stable_vector<T>::iterator stable_vector<T>::end() { return iterator(this, size_); }

template<typename T>
typename stable_vector<T>::const_iterator stable_vector<T>::cend()const{return const_iterator(this, size_);}

#endif //CPP_HOMEWORKS_STABLE_VECTOR_H