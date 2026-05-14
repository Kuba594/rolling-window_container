#ifndef PROJECT_INCREMENTAL_STATS_H
#define PROJECT_INCREMENTAL_STATS_H

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>
#include "kontejner.h"
#include "matrix.h"

template <typename T>
class IncrementalStats {
public:
    using size_type  = std::size_t;

    IncrementalStats(size_type n_assets, size_type window);

    void push_column(const std::vector<T>& col);

    void clear() noexcept;

    size_type rows() const noexcept;
    size_type cols() const noexcept;
    size_type capacity() const noexcept;
    bool empty() const noexcept;
    bool full() const noexcept;

    double mean(size_type i) const;
    double variance(size_type i) const;
    double stddev(size_type i) const;
    double covariance(size_type i, size_type j) const;
    double correlation(size_type i, size_type j) const;

    const std::vector<double>& mean_vector() const;
    const Matrix<double>& covariance_matrix() const;
    const Matrix<double>& correlation_matrix() const;

private:
    size_type n_assets_;
    size_type window_;
    RollingMatrix<T> data_;

    std::vector<double> sum_;
    Matrix<double> ssum_;

    std::vector<double> mean_;
    Matrix<double> cov_;
    Matrix<double> corr_;

    void subtract_oldest();
    void add_newest(const std::vector<T>& col);
    void recompute_derived();
};

template<typename T>
IncrementalStats<T>::IncrementalStats(size_type n_assets, size_type window): n_assets_(n_assets), window_(window),
data_(n_assets, window), sum_(n_assets, 0), ssum_(n_assets, n_assets), mean_(n_assets, 0), cov_(n_assets, n_assets),
corr_(n_assets, n_assets){}

template<typename T>
void IncrementalStats<T>::push_column(const std::vector<T>& col) {
    if (col.size() != n_assets_)
        throw std::invalid_argument("IncrementalStats::push_column: column size mismatch");

    if (data_.full()) subtract_oldest();
    data_.push_column(col);
    add_newest(col);
    recompute_derived();
}

template<typename T>
void IncrementalStats<T>::clear() noexcept {
    data_.clear();
    std::fill(sum_.begin(),  sum_.end(),  0.0);
    std::fill(mean_.begin(), mean_.end(), 0.0);
    ssum_.fill(0.0);
    cov_.fill(0.0);
    corr_.fill(0.0);
}



template<typename T>
typename IncrementalStats<T>::size_type IncrementalStats<T>::rows() const noexcept {
    return n_assets_;
}

template<typename T>
typename IncrementalStats<T>::size_type IncrementalStats<T>::cols() const noexcept {
    return data_.filled();
}

template<typename T>
typename IncrementalStats<T>::size_type IncrementalStats<T>::capacity() const noexcept {
    return window_;
}

template<typename T>
bool IncrementalStats<T>::empty() const noexcept {
    return data_.empty();
}

template<typename T>
bool IncrementalStats<T>::full() const noexcept {
    return data_.cols();
}
//------------------
template<typename T>
double IncrementalStats<T>::mean(size_type i) const{return mean_[i];}

template<typename T>
double IncrementalStats<T>::variance(size_type i) const {return cov_.at(i, i);}
template<typename T>
double IncrementalStats<T>::stddev(size_type i) const{return std::sqrt(cov_.at(i, i));}
template<typename T>
double IncrementalStats<T>::covariance(size_type i, size_type j) const{return cov_.at(i, j);}
template<typename T>
double IncrementalStats<T>::correlation(size_type i, size_type j) const{return corr_.at(i, i);}
//------------------
//------------------
template<typename T>
const std::vector<double>& IncrementalStats<T>::mean_vector() const{return mean_;}
template<typename T>
const Matrix<double>& IncrementalStats<T>::covariance_matrix() const{return cov_;}
template<typename T>
const Matrix<double>& IncrementalStats<T>::correlation_matrix() const{return corr_;}
//------------------
//------------------
template<typename T>
void IncrementalStats<T>::subtract_oldest() {}
template<typename T>
void IncrementalStats<T>::add_newest(const std::vector<T>& col){}
template<typename T>
void IncrementalStats<T>::recompute_derived(){}
//------------------
#endif
