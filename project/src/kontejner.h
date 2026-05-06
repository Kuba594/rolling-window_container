#ifndef ROLLING_MATRIX_HPP
#define ROLLING_MATRIX_HPP

#include <iostream>
#include <vector>

template <typename T>
class RollingMatrix {
public:
    using value_type = T;
    using size_type  = std::size_t;

    RollingMatrix(size_type n_assets, size_type window);

    size_type rows() const noexcept;
    size_type cols() const noexcept;
    size_type capacity() const noexcept;
    bool empty() const noexcept;
    bool full() const noexcept;

    T& operator()(size_type asset, size_type step);
    const T& operator()(size_type asset, size_type step) const;

    void push_column(const std::vector<T>& col);
    void clear() noexcept;

private:
    size_type n_assets_;
    size_type window_;
    size_type head_; //oldest column index
    size_type filled_;   //number of filled collumns
    std::vector<T> data_;
};

//-----------------------------
template<typename T>
typename RollingMatrix<T>::size_type RollingMatrix<T>::rows() const noexcept {return n_assets_;}
template<typename T>
typename RollingMatrix<T>::size_type RollingMatrix<T>::cols() const noexcept { return filled_;}
template<typename T>
typename RollingMatrix<T>::size_type RollingMatrix<T>::capacity() const noexcept {return window_;}
//-----------------------------

//-----------------------------
template<typename T>
RollingMatrix<T>::RollingMatrix(size_type n_assets, size_type window) : n_assets_(n_assets),
window_(window), head_(0), filled_(0), data_(n_assets * window) {
    if (n_assets == 0 || window == 0)
        throw std::invalid_argument("RollingMatrix: dimensions must be > 0");
}
//-----------------------------

#endif // ROLLING_MATRIX_HPP
