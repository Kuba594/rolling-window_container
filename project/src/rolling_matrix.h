#ifndef ROLLING_MATRIX_HPP
#define ROLLING_MATRIX_HPP

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <memory>

//Main class of whole project. It holds window size infomration on data. Once window is filled the first observation
//is overwritten.
template <typename T>
class RollingMatrix {
public:
    using value_type = T;
    using size_type  = std::size_t;

    RollingMatrix(size_type n_assets, size_type window);
    RollingMatrix(const RollingMatrix& other); //copy ctor
    RollingMatrix& operator=(RollingMatrix other); //copy assign
    RollingMatrix(RollingMatrix&& other) noexcept = default; //move ctor
    ~RollingMatrix() = default; //destructor

    size_type rows() const noexcept;
    size_type cols() const noexcept;
    size_type capacity() const noexcept;
    bool empty() const noexcept;
    bool full() const noexcept;

    class column_iterator;
    class const_column_iterator;

    column_iterator begin();
    column_iterator end();
    const_column_iterator begin() const noexcept;
    const_column_iterator end() const noexcept;
    const_column_iterator cbegin() const noexcept;
    const_column_iterator cend() const noexcept;


    T& operator()(size_type asset, size_type step) noexcept;
    const T& operator()(size_type asset, size_type step) const noexcept;
    T& at(size_type asset, size_type step);
    const T& at(size_type asset, size_type step) const;

    void push_column(const std::vector<T>& col);
    void clear() noexcept;
    void swap(RollingMatrix& other) noexcept;
    class column_view {
        T* col_data_;
        size_type n_assets_;
    public:
        column_view(T* data, size_type n);
        T& operator[](size_type asset) noexcept;
        const T&  operator[](size_type asset) const noexcept;
        size_type size() const noexcept;
        T* data() const noexcept;
    };
    class const_column_view {
        const T*  col_data_;
        size_type n_assets_;
    public:
        const_column_view(const T* data, size_type n);
        const_column_view(const column_view& v);
        const T&  operator[](size_type asset) const noexcept;
        size_type size() const noexcept;
    };
private:
    size_type n_assets_; //number of all rows
    size_type window_; //number of all columns
    size_type head_; //oldest column index
    size_type filled_;   //number of filled collumns
    std::unique_ptr<T[]> data_;

};


//-----------------------------
//const iterator
template<typename T>
class RollingMatrix<T>::const_column_iterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = const_column_view;
    using difference_type   = std::ptrdiff_t;
    using pointer           = const_column_view*;
    using reference         = const_column_view;

    const_column_iterator() noexcept;
    const_column_iterator(const RollingMatrix* m, size_type step) noexcept;

    const_column_view operator*() const noexcept;
    const_column_view operator[](difference_type n) const noexcept;

    const_column_iterator& operator++()    noexcept;
    const_column_iterator  operator++(int) noexcept;
    const_column_iterator& operator--()    noexcept;
    const_column_iterator  operator--(int) noexcept;

    const_column_iterator& operator+=(difference_type n) noexcept;
    const_column_iterator& operator-=(difference_type n) noexcept;
    const_column_iterator  operator+ (difference_type n) const noexcept;
    const_column_iterator  operator- (difference_type n) const noexcept;
    difference_type  operator- (const const_column_iterator& other) const noexcept;

    bool operator==(const const_column_iterator& o) const noexcept;
    bool operator!=(const const_column_iterator& o) const noexcept;
    bool operator< (const const_column_iterator& o) const noexcept;
    bool operator<=(const const_column_iterator& o) const noexcept;
    bool operator> (const const_column_iterator& o) const noexcept;
    bool operator>=(const const_column_iterator& o) const noexcept;

private:
    const RollingMatrix* matrix_;
    size_type step_;
};
template<typename T>
RollingMatrix<T>::const_column_iterator::const_column_iterator() noexcept
    : matrix_(nullptr), step_(0) {}

template<typename T>
RollingMatrix<T>::const_column_iterator::const_column_iterator(const RollingMatrix* m, size_type step) noexcept
    : matrix_(m), step_(step) {}

//
template<typename T>
typename RollingMatrix<T>::const_column_view
RollingMatrix<T>::const_column_iterator::operator*() const noexcept {
    size_type physical = (matrix_->head_ + step_) % matrix_->window_;
    return const_column_view{matrix_->data_.get() + physical * matrix_->n_assets_, matrix_->n_assets_};
}

template<typename T>
typename RollingMatrix<T>::const_column_view
RollingMatrix<T>::const_column_iterator::operator[](difference_type n) const noexcept {
    return *(*this + n);
}
//
template<typename T>
typename RollingMatrix<T>::const_column_iterator& RollingMatrix<T>::const_column_iterator::operator++() noexcept {
    ++step_;
    return *this;
}
template<typename T>
typename RollingMatrix<T>::const_column_iterator RollingMatrix<T>::const_column_iterator::operator++(int) noexcept {
    const_column_iterator tmp = *this;
    ++step_;
    return tmp;
}
template<typename T>
typename RollingMatrix<T>::const_column_iterator& RollingMatrix<T>::const_column_iterator::operator--() noexcept {
    --step_;
    return *this;
}
template<typename T>
typename RollingMatrix<T>::const_column_iterator RollingMatrix<T>::const_column_iterator::operator--(int) noexcept {
    const_column_iterator tmp = *this;
    --step_;
    return tmp;
}
//
template<typename T>
typename RollingMatrix<T>::const_column_iterator& RollingMatrix<T>::const_column_iterator::operator+=(difference_type n) noexcept {
    step_ = static_cast<size_type>(static_cast<difference_type>(step_) + n);
    return *this;
}
template<typename T>
typename RollingMatrix<T>::const_column_iterator& RollingMatrix<T>::const_column_iterator::operator-=(difference_type n) noexcept {
    step_ = static_cast<size_type>(static_cast<difference_type>(step_) - n);
    return *this;
}
template<typename T>
typename RollingMatrix<T>::const_column_iterator RollingMatrix<T>::const_column_iterator::operator+(difference_type n) const noexcept {
    const_column_iterator tmp = *this;
    tmp += n;
    return tmp;
}
template<typename T>
typename RollingMatrix<T>::const_column_iterator RollingMatrix<T>::const_column_iterator::operator-(difference_type n) const noexcept {
    const_column_iterator tmp = *this;
    tmp -= n;
    return tmp;
}
template<typename T>
typename RollingMatrix<T>::const_column_iterator::difference_type
RollingMatrix<T>::const_column_iterator::operator-(const const_column_iterator& other) const noexcept {
    return static_cast<difference_type>(step_) - static_cast<difference_type>(other.step_);
}
//
template<typename T>
bool RollingMatrix<T>::const_column_iterator::operator==(const const_column_iterator& o) const noexcept {return matrix_ == o.matrix_ && step_ == o.step_;}

template<typename T>
bool RollingMatrix<T>::const_column_iterator::operator!=(const const_column_iterator& o) const noexcept {return !(*this == o);}

template<typename T>
bool RollingMatrix<T>::const_column_iterator::operator<=(const const_column_iterator& o) const noexcept {return matrix_ == o.matrix_ && step_ <= o.step_;}

template<typename T>
bool RollingMatrix<T>::const_column_iterator::operator>=(const const_column_iterator& o) const noexcept {return matrix_ == o.matrix_ && step_ >= o.step_;}

template<typename T>
bool RollingMatrix<T>::const_column_iterator::operator<(const const_column_iterator& o) const noexcept {return matrix_ == o.matrix_ && step_ < o.step_;}

template<typename T>
bool RollingMatrix<T>::const_column_iterator::operator>(const const_column_iterator& o) const noexcept {return matrix_ == o.matrix_ && step_ > o.step_;}
//-----------------------------

//-----------------------------
//non-const iterator
template<typename T>
class RollingMatrix<T>::column_iterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = column_view;
    using difference_type   = std::ptrdiff_t;
    using pointer           = column_view*;
    using reference         = column_view;

    column_iterator() noexcept;
    column_iterator(RollingMatrix* m, size_type step) noexcept;

    column_view operator*() const noexcept;
    column_view operator[](difference_type n) const noexcept;

    column_iterator& operator++()    noexcept;
    column_iterator  operator++(int) noexcept;
    column_iterator& operator--()    noexcept;
    column_iterator  operator--(int) noexcept;

    column_iterator& operator+=(difference_type n) noexcept;
    column_iterator& operator-=(difference_type n) noexcept;
    column_iterator  operator+ (difference_type n) const noexcept;
    column_iterator  operator- (difference_type n) const noexcept;
    difference_type  operator- (const column_iterator& other) const noexcept;

    bool operator==(const column_iterator& o) const noexcept;
    bool operator!=(const column_iterator& o) const noexcept;
    bool operator< (const column_iterator& o) const noexcept;
    bool operator<=(const column_iterator& o) const noexcept;
    bool operator> (const column_iterator& o) const noexcept;
    bool operator>=(const column_iterator& o) const noexcept;

private:
    RollingMatrix* matrix_;
    size_type step_;
};
template<typename T>
RollingMatrix<T>::column_iterator::column_iterator() noexcept
    : matrix_(nullptr), step_(0) {}

template<typename T>
RollingMatrix<T>::column_iterator::column_iterator(RollingMatrix* m, size_type step) noexcept
    : matrix_(m), step_(step) {}
//
template<typename T>
typename RollingMatrix<T>::column_view RollingMatrix<T>::column_iterator::operator*() const noexcept {
    size_type step = (matrix_->head_ + step_) % matrix_->window_;
    return column_view{
        matrix_->data_.get() + step * matrix_->n_assets_,
        matrix_->n_assets_
    };
}

template<typename T>
typename RollingMatrix<T>::column_view RollingMatrix<T>::column_iterator::operator[](difference_type n) const noexcept {
    return *(*this + n);
}
//


template<typename T>
typename RollingMatrix<T>::column_iterator& RollingMatrix<T>::column_iterator::operator++() noexcept {
    ++step_;
    return *this;
}
template<typename T>
typename RollingMatrix<T>::column_iterator RollingMatrix<T>::column_iterator::operator++(int) noexcept {
    column_iterator tmp = *this;
    ++step_;
    return tmp;
}
template<typename T>
typename RollingMatrix<T>::column_iterator& RollingMatrix<T>::column_iterator::operator--() noexcept {
    --step_;
    return *this;
}
template<typename T>
typename RollingMatrix<T>::column_iterator RollingMatrix<T>::column_iterator::operator--(int) noexcept {
    column_iterator tmp = *this;
    --step_;
    return tmp;
}
//

template<typename T>
typename RollingMatrix<T>::column_iterator& RollingMatrix<T>::column_iterator::operator+=(difference_type n) noexcept {
    step_ = static_cast<size_type>(static_cast<difference_type>(step_) + n);
    return *this;
}
template<typename T>
typename RollingMatrix<T>::column_iterator& RollingMatrix<T>::column_iterator::operator-=(difference_type n) noexcept {
    step_ = static_cast<size_type>(static_cast<difference_type>(step_) - n);
    return *this;
}
template<typename T>
typename RollingMatrix<T>::column_iterator RollingMatrix<T>::column_iterator::operator+(difference_type n)const noexcept {
    column_iterator tmp = *this;
    tmp += n;
    return tmp;
}
template<typename T>
typename RollingMatrix<T>::column_iterator RollingMatrix<T>::column_iterator::operator-(difference_type n) const noexcept {
    column_iterator tmp = *this;
    tmp -= n;
    return tmp;
}
template<typename T>
typename RollingMatrix<T>::column_iterator::difference_type RollingMatrix<T>::column_iterator::operator-(const column_iterator& n) const noexcept {
    return static_cast<difference_type>(step_) - static_cast<difference_type>(n.step_);
}
//

template<typename T>
bool RollingMatrix<T>::column_iterator::operator==(const column_iterator& o) const noexcept {return matrix_ == o.matrix_ && step_ == o.step_;}

template<typename T>
bool RollingMatrix<T>::column_iterator::operator!=(const column_iterator& o) const noexcept {return !(*this == o);}

template<typename T>
bool RollingMatrix<T>::column_iterator::operator<=(const column_iterator& o) const noexcept {return matrix_ == o.matrix_ && step_ <= o.step_;}

template<typename T>
bool RollingMatrix<T>::column_iterator::operator>=(const column_iterator& o) const noexcept {return matrix_ == o.matrix_ && step_ >= o.step_;}

template<typename T>
bool RollingMatrix<T>::column_iterator::operator<(const column_iterator& o) const noexcept {return matrix_ == o.matrix_ && step_ < o.step_;}

template<typename T>
bool RollingMatrix<T>::column_iterator::operator>(const column_iterator& o) const noexcept {return matrix_ == o.matrix_ && step_ > o.step_;}

//-----------------------------

//-----------------------------
//const and non-const column view implementaios. It shows colulm of assets
template <typename T>
T& RollingMatrix<T>::column_view::operator[](size_type asset) noexcept {return col_data_[asset];}

template <typename T>
const T& RollingMatrix<T>::column_view::operator[](size_type asset) const noexcept {return col_data_[asset];}

template <typename T>
typename RollingMatrix<T>::size_type RollingMatrix<T>::column_view::size() const noexcept {return n_assets_;}

template <typename T>
RollingMatrix<T>::column_view::column_view(T* data, size_type n) : col_data_(data), n_assets_(n) {}
template <typename T>
T* RollingMatrix<T>::column_view::data() const noexcept { return col_data_; }

template <typename T>
const T& RollingMatrix<T>::const_column_view::operator[](size_type asset) const noexcept {
    return col_data_[asset];
}

template <typename T>
typename RollingMatrix<T>::size_type
RollingMatrix<T>::const_column_view::size() const noexcept {
    return n_assets_;
}

template <typename T>
RollingMatrix<T>::const_column_view::const_column_view(const T* data, size_type n)
    : col_data_(data), n_assets_(n) {}

template <typename T>
RollingMatrix<T>::const_column_view::const_column_view(const column_view& v)
    : col_data_(v.data()), n_assets_(v.size()) {}
//-----------------------------

//-----------------------------
//basic methods returning info about rolling matrix
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
//ctors and assignment
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
//methods that allow to get info from inside of matrix
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
//method that pushes one of observations if is filled first column is overwritten
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
//comparing operators
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
//-----------------------------
//methods that begin and end iterators
template<typename T>
typename RollingMatrix<T>::column_iterator RollingMatrix<T>::begin() {
    return column_iterator(this, 0);
}

template<typename T>
typename RollingMatrix<T>::column_iterator RollingMatrix<T>::end() {
    return column_iterator(this, filled_);
}
template<typename T>
typename RollingMatrix<T>::const_column_iterator RollingMatrix<T>::begin() const noexcept {
    return const_column_iterator(this, 0);
}

template<typename T>
typename RollingMatrix<T>::const_column_iterator RollingMatrix<T>::end() const noexcept {
    return const_column_iterator(this, filled_);
}

template<typename T>
typename RollingMatrix<T>::const_column_iterator RollingMatrix<T>::cbegin() const noexcept {
    return begin();
}

template<typename T>
typename RollingMatrix<T>::const_column_iterator RollingMatrix<T>::cend() const noexcept {
    return end();
}
//-----------------------------
#endif // ROLLING_MATRIX_HPP
