#include "../../src/kontejner.h"
#include "../../src/rolling_stats.h"
#include <iostream>
#include <cmath>
#include <stdexcept>

static int tests_run    = 0;
static int tests_failed = 0;

#define ASSERT(cond) do {                                                       \
    if (!(cond)) {                                                              \
        std::cerr << "    FAIL: " << #cond << "  (line " << __LINE__ << ")\n"; \
        ++tests_failed;                                                         \
        return;                                                                 \
    }                                                                           \
} while (0)

#define ASSERT_CLOSE(a, b, tol) do {                                            \
    double _a = (a), _b = (b);                                                  \
    if (std::abs(_a - _b) > (tol)) {                                            \
        std::cerr << "    FAIL: |" << _a << " - " << _b << "| > " << (tol)      \
                  << "  (line " << __LINE__ << ")\n";                           \
        ++tests_failed;                                                         \
        return;                                                                 \
    }                                                                           \
} while (0)

#define ASSERT_THROWS(expr, ex_type) do {                                       \
    bool caught = false;                                                        \
    try { (void)(expr); } catch (const ex_type&) { caught = true; }             \
    if (!caught) {                                                              \
        std::cerr << "    FAIL: " #expr " did not throw " #ex_type              \
                  << "  (line " << __LINE__ << ")\n";                           \
        ++tests_failed;                                                         \
        return;                                                                 \
    }                                                                           \
} while (0)

#define RUN_TEST(name) do {                                                     \
    ++tests_run;                                                                \
    int before = tests_failed;                                                  \
    std::cout << "[ RUN ] " #name << "\n";                                      \
    name();                                                                     \
    if (tests_failed == before) std::cout << "[  OK ]\n";                       \
} while (0)

//mean
void test_mean_basic() {
    RollingMatrix<double> m(2, 4);
    m.push_column({1.0, 2.0});
    m.push_column({2.0, 4.0});
    m.push_column({3.0, 6.0});
    m.push_column({4.0, 8.0});

    ASSERT_CLOSE(stats::mean(m, 0), 2.5, 1e-12);
    ASSERT_CLOSE(stats::mean(m, 1), 5.0, 1e-12);
}

void test_mean_with_int_type() {
    RollingMatrix<int> m(2, 4);
    m.push_column({1, 10});
    m.push_column({2, 20});
    m.push_column({3, 30});

    ASSERT_CLOSE(stats::mean(m, 0), 2.0,  1e-12);
    ASSERT_CLOSE(stats::mean(m, 1), 20.0, 1e-12);
}

void test_mean_partially_filled_window() {
    RollingMatrix<double> m(1, 10);
    m.push_column({1.0});
    m.push_column({2.0});
    m.push_column({3.0});
    ASSERT_CLOSE(stats::mean(m, 0), 2.0, 1e-12);
}

void test_mean_after_rolling() {
    RollingMatrix<double> m(1, 3);
    m.push_column({10.0});
    m.push_column({20.0});
    m.push_column({30.0});
    m.push_column({40.0});
    ASSERT_CLOSE(stats::mean(m, 0), 30.0, 1e-12);
}

void test_mean_throws() {
    RollingMatrix<double> m(2, 4);
    ASSERT_THROWS(stats::mean(m, 0), std::logic_error);
}

//variance

void test_variance_basic() {
    RollingMatrix<double> m(2, 4);
    m.push_column({1.0, 2.0});
    m.push_column({2.0, 4.0});
    m.push_column({3.0, 6.0});
    m.push_column({4.0, 8.0});

    ASSERT_CLOSE(stats::variance(m, 0), 5.0  / 3.0, 1e-12);
    ASSERT_CLOSE(stats::variance(m, 1), 20.0 / 3.0, 1e-12);
}

void test_variance_constant_data_is_zero() {
    RollingMatrix<double> m(1, 4);
    m.push_column({5.0});
    m.push_column({5.0});
    m.push_column({5.0});
    m.push_column({5.0});
    ASSERT_CLOSE(stats::variance(m, 0), 0.0, 1e-12);
}

void test_variance_throws() {
    RollingMatrix<double> m(1, 4);
    ASSERT_THROWS(stats::variance(m, 0), std::logic_error);
    m.push_column({1.0});
    ASSERT_THROWS(stats::variance(m, 0), std::logic_error);
}

//stddev

void test_stddev_basic() {
    RollingMatrix<double> m(1, 4);
    m.push_column({1.0});
    m.push_column({2.0});
    m.push_column({3.0});
    m.push_column({4.0});
    ASSERT_CLOSE(stats::stddev(m, 0), std::sqrt(5.0 / 3.0), 1e-12);
}

//covariance
void test_covariance_perfec_corr() {
    RollingMatrix<double> m(2, 4);
    m.push_column({1.0, 2.0});
    m.push_column({2.0, 4.0});
    m.push_column({3.0, 6.0});
    m.push_column({4.0, 8.0});

    ASSERT_CLOSE(stats::covariance(m, 0, 1), 10.0 / 3.0, 1e-12);
}

void test_covariance_symmetric() {
    RollingMatrix<double> m(2, 4);
    m.push_column({1.0, 5.0});
    m.push_column({2.0, 4.0});
    m.push_column({3.0, 3.0});
    m.push_column({4.0, 2.0});
    ASSERT_CLOSE(stats::covariance(m, 0, 1), stats::covariance(m, 1, 0), 1e-12);
}

void test_covariance_with_self() {
    RollingMatrix<double> m(1, 4);
    m.push_column({1.0});
    m.push_column({2.0});
    m.push_column({3.0});
    m.push_column({4.0});
    ASSERT_CLOSE(stats::covariance(m, 0, 0), stats::variance(m, 0), 1e-12);
}

//correlation

void test_correlation_one() {
    RollingMatrix<double> m(2, 4);
    m.push_column({1.0, 2.0});
    m.push_column({2.0, 4.0});
    m.push_column({3.0, 6.0});
    m.push_column({4.0, 8.0});
    ASSERT_CLOSE(stats::correlation(m, 0, 1), 1.0, 1e-12);
}

void test_correlation_perfect_minus_one() {
    RollingMatrix<double> m(2, 4);
    m.push_column({1.0, -1.0});
    m.push_column({2.0, -2.0});
    m.push_column({3.0, -3.0});
    m.push_column({4.0, -4.0});
    ASSERT_CLOSE(stats::correlation(m, 0, 1), -1.0, 1e-12);
}

void test_correlation_with_self() {
    RollingMatrix<double> m(1, 4);
    m.push_column({1.0});
    m.push_column({2.0});
    m.push_column({3.0});
    m.push_column({4.0});
    ASSERT_CLOSE(stats::correlation(m, 0, 0), 1.0, 1e-12);
}

void test_correlation_const_zero() {
    RollingMatrix<double> m(2, 4);
    m.push_column({1.0, 5.0});
    m.push_column({2.0, 5.0});
    m.push_column({3.0, 5.0});
    m.push_column({4.0, 5.0});
    ASSERT_CLOSE(stats::correlation(m, 0, 1), 0.0, 1e-12);
}
//mean_vector
void test_mean_vector_basic() {
    RollingMatrix<double> m(2, 4);
    m.push_column({1.0, 2.0});
    m.push_column({2.0, 4.0});
    m.push_column({3.0, 6.0});
    m.push_column({4.0, 8.0});

    auto means = stats::mean_vector(m);
    ASSERT(means.size() == 2);
    ASSERT_CLOSE(means[0], 2.5, 1e-12);
    ASSERT_CLOSE(means[1], 5.0, 1e-12);
}

//covariance_matrix
void test_covariance_matrix_basic() {
    RollingMatrix<double> m(2, 4);
    m.push_column({1.0, 2.0});
    m.push_column({2.0, 4.0});
    m.push_column({3.0, 6.0});
    m.push_column({4.0, 8.0});

    auto cov = stats::covariance_matrix(m);
    ASSERT(cov.rows() == 2);
    ASSERT(cov.cols() == 2);

    ASSERT_CLOSE(cov(0, 0), stats::variance(m, 0), 1e-12);
    ASSERT_CLOSE(cov(1, 1), stats::variance(m, 1), 1e-12);

    ASSERT_CLOSE(cov(0, 1), 10.0 / 3.0, 1e-12);
    ASSERT_CLOSE(cov(1, 0), cov(0, 1), 1e-12);
}

//correlation_matrix
void test_correlation_matrix_basic() {
    RollingMatrix<double> m(2, 4);
    // Asset 0 and Asset 1 are perfectly correlated
    m.push_column({1.0, 2.0});
    m.push_column({2.0, 4.0});
    m.push_column({3.0, 6.0});
    m.push_column({4.0, 8.0});

    auto corr = stats::correlation_matrix(m);
    ASSERT(corr.rows() == 2);
    ASSERT(corr.cols() == 2);

    ASSERT_CLOSE(corr(0, 0), 1.0, 1e-12);
    ASSERT_CLOSE(corr(1, 1), 1.0, 1e-12);

    ASSERT_CLOSE(corr(0, 1), 1.0, 1e-12);
    ASSERT_CLOSE(corr(1, 0), 1.0, 1e-12);
}


// ===== entry point ===========================================================

int main() {
    RUN_TEST(test_mean_basic);
    RUN_TEST(test_mean_with_int_type);
    RUN_TEST(test_mean_partially_filled_window);
    RUN_TEST(test_mean_after_rolling);
    RUN_TEST(test_mean_throws);

    RUN_TEST(test_variance_basic);
    RUN_TEST(test_variance_constant_data_is_zero);
    RUN_TEST(test_variance_throws);

    RUN_TEST(test_stddev_basic);

    RUN_TEST(test_covariance_perfec_corr);
    RUN_TEST(test_covariance_symmetric);
    RUN_TEST(test_covariance_with_self);

    RUN_TEST(test_correlation_one);
    RUN_TEST(test_correlation_perfect_minus_one);
    RUN_TEST(test_correlation_with_self);
    RUN_TEST(test_correlation_const_zero);

    RUN_TEST(test_mean_vector_basic);
    RUN_TEST(test_covariance_matrix_basic);
    RUN_TEST(test_correlation_matrix_basic);

    std::cout << "\nRan " << tests_run << " tests,  passed "
              << (tests_run - tests_failed) << ",  failed " << tests_failed << "\n";
    return tests_failed == 0 ? 0 : 1;
}