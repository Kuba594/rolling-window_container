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

#endif // ROLLING_MATRIX_HPP
