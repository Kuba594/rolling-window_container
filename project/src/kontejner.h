#ifndef ROLLING_MATRIX_HPP
#define ROLLING_MATRIX_HPP

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <memory>

template <typename T>
class RollingMatrix {
public:
    using value_type = T;
    using size_type  = std::size_t;

    RollingMatrix(size_type n_assets, size_type window);
    RollingMatrix(const RollingMatrix& other);                     //copy ctor
    RollingMatrix& operator=(RollingMatrix other);          //copy assign
    RollingMatrix(RollingMatrix&& other) noexcept = default;       //move ctor
    ~RollingMatrix() = default; //destructor

    size_type rows() const noexcept;
    size_type cols() const noexcept;
    size_type capacity() const noexcept;
    bool empty() const noexcept;
    bool full() const noexcept;

    T& operator()(size_type asset, size_type step) noexcept;
    const T& operator()(size_type asset, size_type step) const noexcept;
    T& at(size_type asset, size_type step);
    const T& at(size_type asset, size_type step) const;

    void push_column(const std::vector<T>& col);
    void clear() noexcept;
    void swap(RollingMatrix& other) noexcept;

private:
    size_type n_assets_; //number of all rows
    size_type window_; //number of all columns
    size_type head_; //oldest column index
    size_type filled_;   //number of filled collumns
    std::unique_ptr<T[]> data_;
};

//-----------------------------
template<typename T>
typename RollingMatrix<T>::size_type RollingMatrix<T>::rows() const noexcept {return n_assets_;}

template<typename T>
typename RollingMatrix<T>::size_type RollingMatrix<T>::cols() const noexcept { return filled_;}

template<typename T>
typename RollingMatrix<T>::size_type RollingMatrix<T>::capacity() const noexcept {return window_;}

template<typename T>
bool RollingMatrix<T>::empty() const noexcept {return filled_ == 0;}

template<typename T>
bool RollingMatrix<T>::full() const noexcept {return filled_ == window_;}

//-----------------------------

//-----------------------------
template<typename T>
RollingMatrix<T>::RollingMatrix(size_type n_assets, size_type window) : n_assets_(n_assets),
window_(window), head_(0), filled_(0), data_(std::make_unique<T[]>(n_assets * window)) {
    if (n_assets == 0 || window == 0)
        throw std::invalid_argument("RollingMatrix: dimensions must be > 0");
}

template<typename T>
RollingMatrix<T>::RollingMatrix(const RollingMatrix &other) : n_assets_(other.n_assets_), window_(other.window_), head_(other.head_),
      filled_(other.filled_), data_(std::make_unique<T[]>(other.n_assets_ * other.window_)) {
    std::copy(other.data_.get(), other.data_.get() + n_assets_ * window_,data_.get());
}

template<typename T>
RollingMatrix<T>& RollingMatrix<T>::operator=(RollingMatrix other) {
    swap(other);
    return *this;
}
//-----------------------------

//-----------------------------
template<typename T>
T& RollingMatrix<T>::operator()(size_type asset, size_type step) noexcept {
    size_type physical_col = (head_ + step) % window_;
    return data_[physical_col * n_assets_ + asset];
}

template<typename T>
const T& RollingMatrix<T>::operator()(size_type asset, size_type step) const noexcept {
    size_type physical_col = (head_ + step) % window_;
    return data_[physical_col * n_assets_ + asset];
}

template<typename T>
T& RollingMatrix<T>::at(size_type asset, size_type step) {
    if (asset >= n_assets_ || step >= filled_)
        throw std::out_of_range("RollingMatrix: index out of range");
    return (*this)(asset, step);
}

template<typename T>
const T& RollingMatrix<T>::at(size_type asset, size_type step) const {
    if (asset >= n_assets_ || step >= filled_)
        throw std::out_of_range("RollingMatrix: index out of range");
    return (*this)(asset, step);
}
//-----------------------------

//-----------------------------
template<typename T>
void RollingMatrix<T>::push_column(const std::vector<T>& col) {
    if (col.size() != n_assets_)
        throw std::invalid_argument("RollingMatrix: column size mismatch");
    size_type write_col;
    if (!full()) {
        write_col = (head_ + filled_); //% window_ není třeba modulo, není full
        ++filled_;
    } else {
        write_col = head_;
        head_ = (head_ + 1) % window_;
    }
    std::copy(col.begin(), col.end(), data_.get() + write_col * n_assets_);
}

template<typename T>
void RollingMatrix<T>::clear() noexcept {head_ = 0; filled_ = 0;}

template<typename T>
void RollingMatrix<T>::swap(RollingMatrix& other) noexcept {
    std::swap(this->data_, other.data_);
    std::swap(this->n_assets_, other.n_assets_);
    std::swap(this->window_, other.window_);
    std::swap(this->head_, other.head_);
    std::swap(this->filled_, other.filled_);
}

//-----------------------------

//-----------------------------
template<typename T>
void swap(RollingMatrix<T>& a, RollingMatrix<T>& b) noexcept {
    a.swap(b);
}
template <typename T>
bool operator==(const RollingMatrix<T>& a, const RollingMatrix<T>& b) {
    if (a.rows() != b.rows() || a.cols() != b.cols()) return false;
    for (std::size_t s = 0; s < a.cols(); ++s) {
        for (std::size_t r = 0; r < a.rows(); ++r) {
            if (!(a(r, s) == b(r, s))) return false;
        }
    }
    return true;
}

template <typename T>
bool operator!=(const RollingMatrix<T>& a, const RollingMatrix<T>& b) {
    return !(a == b);
}
//-----------------------------
#endif // ROLLING_MATRIX_HPP
