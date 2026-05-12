#ifndef PROJECT_ROLLING_STATS_H
#define PROJECT_ROLLING_STATS_H
#include "kontejner.h"
#include "matrix.h"
#include <cmath>
#include <stdexcept>

namespace stats {
    template <typename T> double mean (const RollingMatrix<T>& m, size_t asset);
    template <typename T> double variance (const RollingMatrix<T>& m, size_t asset);
    template <typename T> double stddev (const RollingMatrix<T>& m, size_t asset);
    template <typename T> double covariance (const RollingMatrix<T>& m, size_t i, size_t j);
    template <typename T> double correlation (const RollingMatrix<T>& m, size_t i, size_t j);

    template <typename T> std::vector<double> mean_vector (const RollingMatrix<T>& m);
    template <typename T> Matrix<double> covariance_matrix (const RollingMatrix<T>& m);
    template <typename T> Matrix<double> correlation_matrix(const RollingMatrix<T>& m);
}

template<typename T>
double stats::mean(const RollingMatrix<T> & m, size_t asset) {
    if (m.cols() == 0)
        throw std::logic_error("stats::mean: empty window");

    double sum = 0.0;
    for (auto&& col : m)
        sum += static_cast<double>(col[asset]);
    return sum / static_cast<double>(m.cols());
}
template<typename T>
double stats::variance(const RollingMatrix<T> & m, size_t asset) {
    if (m.cols() < 2)
        throw std::logic_error("stats::variance: need at least 2 observations for sample variance");

    double mu  = mean(m, asset);
    double acc = 0.0;
    for (auto&& col : m) {
        double d = static_cast<double>(col[asset]) - mu;
        acc += d * d;
    }
    return acc / (static_cast<double>(m.cols())-1);
}
template<typename T>
double stats::stddev(const RollingMatrix<T> & m, size_t asset) {
    return std::sqrt(variance(m, asset));
}
template<typename T>
double stats::covariance(const RollingMatrix<T>& m, size_t i, size_t j) {
    if (m.cols() == 0)
        throw std::logic_error("stats::covaraince: empty window");
    double mu_i = mean(m, i);
    double mu_j = mean(m, j);
    double acc = 0.0;
    for (auto&& col : m) {
        double d_i = static_cast<double>(col[i]) - mu_i;
        double d_j = static_cast<double>(col[j]) - mu_j;
        acc += d_i * d_j;
    }
    return acc / (static_cast<double>(m.cols())-1);
}
template<typename T>
double stats::correlation(const RollingMatrix<T>& m, size_t i, size_t j) {
    if (m.cols() == 0)
        throw std::logic_error("stats::correlation: empty window");
    double si = stddev(m, i);
    double sj = stddev(m, j);
    if (si == 0.0 || sj == 0.0) return 0.0;
    return covariance(m, i, j) / (si * sj);
}


template <typename T>
std::vector<double> stats::mean_vector(const RollingMatrix<T>& m) {
    std::vector<double> res(m.rows());
    for (size_t i = 0; i < m.rows(); ++i) {
        res[i] = mean(m, i);
    }
    return res;
}

template <typename T>
Matrix<double> stats::covariance_matrix(const RollingMatrix<T>& m) {
    size_t n = m.rows();
    Matrix<double> cov(n, n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i; j < n; ++j) {
            double c = covariance(m, i, j);
            cov(i, j) = c;
            cov(j, i) = c;
        }
    }
    return cov;
}

template <typename T>
Matrix<double> stats::correlation_matrix(const RollingMatrix<T>& m) {
    size_t n = m.rows();
    Matrix<double> corr(n, n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i; j < n; ++j) {
            double c = correlation(m, i, j);
            corr(i, j) = c;
            corr(j, i) = c;
        }
    }
    return corr;
}

#endif //PROJECT_ROLLING_STATS_H
