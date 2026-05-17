#ifndef PROJECT_INCREMENTAL_STATS_H
#define PROJECT_INCREMENTAL_STATS_H

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>
#include "rolling_matrix.h"
#include "matrix.h"

//class that computes stats on rolling matrix. If observation is added statistics are updated.
//this method wraps around rolling matrix and computes stats on top of it.
//its templated on the type that matrix inside is going to have but statistics are stored as doubles.
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

//pushes collumn and calls for stats updates
template<typename T>
void IncrementalStats<T>::push_column(const std::vector<T>& col) {
    if (col.size() != n_assets_)
        throw std::invalid_argument("IncrementalStats::push_column: column size mismatch");

    if (data_.full()) subtract_oldest();
    data_.push_column(col);
    add_newest(col);
    recompute_derived();
}
//clears matrrix and stats
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
    return data_.cols();
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
    return data_.full();
}
//------------------
//methods returning stats that are stored inside
template<typename T>
double IncrementalStats<T>::mean(size_type i) const{return mean_[i];}

template<typename T>
double IncrementalStats<T>::variance(size_type i) const {return cov_.at(i, i);}
template<typename T>
double IncrementalStats<T>::stddev(size_type i) const{return std::sqrt(variance(i));}
template<typename T>
double IncrementalStats<T>::covariance(size_type i, size_type j) const{return cov_.at(i, j);}
template<typename T>
double IncrementalStats<T>::correlation(size_type i, size_type j) const{return corr_.at(i, j);}
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
//those 3 methods recompute stats inside
template<typename T>
void IncrementalStats<T>::subtract_oldest() {
    for (size_type i = 0; i < n_assets_; ++i) {
        double xi = static_cast<double>(data_(i, 0));
        sum_[i] -= xi;
        for (size_type j = i; j < n_assets_; ++j) {
            double xj = static_cast<double>(data_(j, 0));
            ssum_(i, j) -= xi * xj;
            if (i != j) ssum_(j, i) = ssum_(i, j);
        }
    }
}
template<typename T>
void IncrementalStats<T>::add_newest(const std::vector<T>& col) {
    for (size_type i = 0; i < n_assets_; ++i) {
        double xi = static_cast<double>(col[i]);
        sum_[i] += xi;
        for (size_type j = i; j < n_assets_; ++j) {
            double xj = static_cast<double>(col[j]);
            ssum_(i, j) += xi * xj;
            if (i != j) ssum_(j, i) = ssum_(i, j);
        }
    }
}
template<typename T>
void IncrementalStats<T>::recompute_derived() {
    const size_type W = data_.cols();
    if (W == 0) return;

    const double Wd = static_cast<double>(W);
    for (size_type i = 0; i < n_assets_; ++i)
        mean_[i] = sum_[i] / Wd;

    if (W < 2) return;
    const double Wm1 = Wd - 1.0;

    for (size_type i = 0; i < n_assets_; ++i) {
        for (size_type j = i; j < n_assets_; ++j) {
            double pop_cov = ssum_(i, j) / Wd - mean_[i] * mean_[j];
            double sample_cov = pop_cov * Wd / Wm1;
            cov_(i, j) = sample_cov;
            if (i != j) cov_(j, i) = sample_cov;
        }
    }

    std::vector<double> sd(n_assets_);
    for (size_type i = 0; i < n_assets_; ++i)
        sd[i] = std::sqrt(cov_(i, i));

    for (size_type i = 0; i < n_assets_; ++i) {
        for (size_type j = i; j < n_assets_; ++j) {
            double c = (sd[i] == 0.0 || sd[j] == 0.0)
                       ? 0.0
                       : cov_(i, j) / (sd[i] * sd[j]);
            corr_(i, j) = c;
            if (i != j) corr_(j, i) = c;
        }
    }
}
//------------------
#endif
