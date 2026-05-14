#ifndef PROJECT_MATRIX_H
#define PROJECT_MATRIX_H

#include <stdexcept>
#include <vector>
#include <algorithm>

template <typename T>
class Matrix {
public:
    using size_type       = std::size_t;

    Matrix(size_type rows, size_type cols);
    Matrix(size_type rows, size_type cols, const T& value);

    T& operator()(size_type i, size_type j) noexcept;
    const T& operator()(size_type i, size_type j) const noexcept;
    T& at(size_type i, size_type j);
    const T& at(size_type i, size_type j) const;

    T* data() noexcept { return m_data.data(); }
    const T* data() const noexcept { return m_data.data(); }

    size_type rows()  const noexcept { return m_rows; }
    size_type cols()  const noexcept { return m_cols; }
    size_type size()  const noexcept { return m_rows * m_cols; }
    bool empty() const noexcept { return m_rows == 0 || m_cols == 0; }

    void fill(const T& value);
    void swap(Matrix& other) noexcept;

private:
    size_type m_rows;
    size_type m_cols;
    std::vector<T> m_data;
};

template <typename T>
void swap(Matrix<T>& a, Matrix<T>& b) noexcept { a.swap(b); }

template <typename T>
bool operator==(const Matrix<T>& a, const Matrix<T>& b) {
    if (a.rows() != b.rows() || a.cols() != b.cols()) return false;
    for (typename Matrix<T>::size_type i = 0; i < a.rows(); ++i)
        for (typename Matrix<T>::size_type j = 0; j < a.cols(); ++j)
            if (!(a(i, j) == b(i, j))) return false;
    return true;
}

template <typename T>
bool operator!=(const Matrix<T>& a, const Matrix<T>& b) {
    return !(a == b);
}

template <typename T>
Matrix<T>::Matrix(size_type rows, size_type cols)
    : m_rows(rows), m_cols(cols), m_data(rows * cols, T{}) {}

template <typename T>
Matrix<T>::Matrix(size_type rows, size_type cols, const T& value)
    : m_rows(rows), m_cols(cols), m_data(rows * cols, value) {}

template <typename T>
T& Matrix<T>::operator()(size_type i, size_type j) noexcept {
    return m_data[i * m_cols + j];
}

template <typename T>
const T& Matrix<T>::operator()(size_type i, size_type j) const noexcept {
    return m_data[i * m_cols + j];
}

template <typename T>
T& Matrix<T>::at(size_type i, size_type j) {
    if (i >= m_rows || j >= m_cols)
        throw std::out_of_range("Matrix::at: index out of range");
    return m_data[i * m_cols + j];
}

template <typename T>
const T& Matrix<T>::at(size_type i, size_type j) const {
    if (i >= m_rows || j >= m_cols)
        throw std::out_of_range("Matrix::at: index out of range");
    return m_data[i * m_cols + j];
}

template <typename T>
void Matrix<T>::fill(const T& value) {
    std::fill(m_data.begin(), m_data.end(), value);
}

template <typename T>
void Matrix<T>::swap(Matrix& other) noexcept {
    std::swap(m_rows, other.m_rows);
    std::swap(m_cols, other.m_cols);
    std::swap(m_data, other.m_data);
}

#endif