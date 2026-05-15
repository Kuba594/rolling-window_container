#include "../../src/Incremental_stats.h"
#include "../../src/rolling_stats.h"
#include <iostream>
#include <random>
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

//basic tests

void test_constructor() {
    IncrementalStats<double> s(3, 5);
    ASSERT(s.rows() == 3);
    ASSERT(s.capacity() == 5);
    ASSERT(s.cols() == 0);
    ASSERT(s.empty());
    ASSERT(!s.full());
}


void test_push_advances_state() {
    IncrementalStats<double> s(2, 3);
    s.push_column({1.0, 10.0});
    ASSERT(s.cols() == 1);
    ASSERT(!s.empty());
    s.push_column({2.0, 20.0});
    s.push_column({3.0, 30.0});
    ASSERT(s.full());
}

void test_clear_resets_to_empty() {
    IncrementalStats<double> s(2, 3);
    s.push_column({1.0, 10.0});
    s.push_column({2.0, 20.0});
    s.clear();
    ASSERT(s.empty());
    ASSERT(s.cols() == 0);
    ASSERT(s.mean(0) == 0.0);
    ASSERT(s.mean(1) == 0.0);
}

//incremental correctness vs computed by hadn

void test_mean_matches_hand_computed() {
    IncrementalStats<double> s(2, 4);
    s.push_column({1.0, 2.0});
    s.push_column({2.0, 4.0});
    s.push_column({3.0, 6.0});
    s.push_column({4.0, 8.0});
    ASSERT_CLOSE(s.mean(0), 2.5, 1e-12);
    ASSERT_CLOSE(s.mean(1), 5.0, 1e-12);
}

void test_variance_matches_hand_computed() {
    IncrementalStats<double> s(2, 4);
    s.push_column({1.0, 2.0});
    s.push_column({2.0, 4.0});
    s.push_column({3.0, 6.0});
    s.push_column({4.0, 8.0});
    ASSERT_CLOSE(s.variance(0), 5.0  / 3.0, 1e-12);
    ASSERT_CLOSE(s.variance(1), 20.0 / 3.0, 1e-12);
}

void test_covariance_matches_hand_computed() {
    IncrementalStats<double> s(2, 4);
    s.push_column({1.0, 2.0});
    s.push_column({2.0, 4.0});
    s.push_column({3.0, 6.0});
    s.push_column({4.0, 8.0});
    ASSERT_CLOSE(s.covariance(0, 1), 10.0 / 3.0, 1e-12);
    ASSERT_CLOSE(s.covariance(0, 0), s.variance(0), 1e-12);
}

void test_correlation_perfect_positive() {
    IncrementalStats<double> s(2, 4);
    s.push_column({1.0, 2.0});
    s.push_column({2.0, 4.0});
    s.push_column({3.0, 6.0});
    s.push_column({4.0, 8.0});
    ASSERT_CLOSE(s.correlation(0, 1), 1.0, 1e-12);
}

void test_correlation_perfect_negative() {
    IncrementalStats<double> s(2, 4);
    s.push_column({1.0, -1.0});
    s.push_column({2.0, -2.0});
    s.push_column({3.0, -3.0});
    s.push_column({4.0, -4.0});
    ASSERT_CLOSE(s.correlation(0, 1), -1.0, 1e-12);
}

void test_correlation_diagonal() {
    IncrementalStats<double> s(2, 4);
    s.push_column({1.0, 5.0});
    s.push_column({2.0, 4.0});
    s.push_column({3.0, 3.0});
    s.push_column({4.0, 2.0});
    ASSERT_CLOSE(s.correlation(0, 0), 1.0, 1e-12);
    ASSERT_CLOSE(s.correlation(1, 1), 1.0, 1e-12);
}

void test_constant_data_zero_correlation() {
    IncrementalStats<double> s(2, 4);
    s.push_column({1.0, 5.0});
    s.push_column({2.0, 5.0});
    s.push_column({3.0, 5.0});
    s.push_column({4.0, 5.0});
    ASSERT_CLOSE(s.correlation(0, 1), 0.0, 1e-12);
    ASSERT_CLOSE(s.variance(1), 0.0, 1e-12);
}

//correctness vs rolling stats on matrix random stas

void test_equivalence_on_random_data() {
    constexpr std::size_t N = 5;
    constexpr std::size_t W = 50;
    constexpr int DAYS = 200;

    RollingMatrix<double> naive(N, W);
    IncrementalStats<double> inc(N, W);

    std::mt19937 gen(42);
    std::normal_distribution<double> dist(0.0, 0.01);

    for (int day = 0; day < DAYS; ++day) {
        std::vector<double> col(N);
        for (auto& x : col) x = dist(gen);

        naive.push_column(col);
        inc.push_column(col);

        if (naive.cols() < 2) continue;

        for (std::size_t i = 0; i < N; ++i)
            ASSERT_CLOSE(inc.mean(i), stats::mean(naive, i), 1e-12);

        for (std::size_t i = 0; i < N; ++i) {
            ASSERT_CLOSE(inc.variance(i), stats::variance(naive, i), 1e-9);
            for (std::size_t j = 0; j < N; ++j)
                ASSERT_CLOSE(inc.covariance(i, j), stats::covariance(naive, i, j), 1e-9);
        }

        for (std::size_t i = 0; i < N; ++i)
            for (std::size_t j = 0; j < N; ++j)
                ASSERT_CLOSE(inc.correlation(i, j), stats::correlation(naive, i, j), 1e-9);
    }
}

//rolling corectnes

void test_after_eviction_state_matches_naive() {
    constexpr std::size_t N = 3, W = 4;
    RollingMatrix<double> naive(N, W);
    IncrementalStats<double> inc(N, W);

    double counter = 0;
    for (int t = 0; t < 10; ++t) {
        std::vector<double> col(N);
        for (std::size_t i = 0; i < N; ++i) col[i] = counter += 1.0;
        naive.push_column(col);
        inc.push_column(col);
    }

    for (std::size_t i = 0; i < N; ++i)
        for (std::size_t j = 0; j < N; ++j)
            ASSERT_CLOSE(inc.covariance(i, j), stats::covariance(naive, i, j), 1e-9);
}

void test_clear_refill_works() {
    IncrementalStats<double> s(2, 4);
    s.push_column({100.0, 200.0});
    s.push_column({101.0, 201.0});
    s.push_column({102.0, 202.0});
    s.clear();

    s.push_column({1.0, 2.0});
    s.push_column({2.0, 4.0});
    s.push_column({3.0, 6.0});
    s.push_column({4.0, 8.0});

    ASSERT_CLOSE(s.mean(0), 2.5, 1e-12);
    ASSERT_CLOSE(s.mean(1), 5.0, 1e-12);
    ASSERT_CLOSE(s.covariance(0, 1), 10.0 / 3.0, 1e-12);
}

//matrix metrics

void test_covariance_matrix_is_symmetric() {
    IncrementalStats<double> s(3, 5);
    s.push_column({1.0, 2.0, 3.0});
    s.push_column({2.0, 4.0, 1.0});
    s.push_column({3.0, 1.0, 4.0});
    s.push_column({4.0, 3.0, 2.0});
    s.push_column({5.0, 5.0, 5.0});

    const auto& cov = s.covariance_matrix();
    ASSERT(cov.rows() == 3 && cov.cols() == 3);
    for (std::size_t i = 0; i < 3; ++i)
        for (std::size_t j = 0; j < 3; ++j)
            ASSERT_CLOSE(cov(i, j), cov(j, i), 1e-12);
}

void test_correlation_matrix_diagonal_is_one() {
    IncrementalStats<double> s(3, 5);
    s.push_column({1.0, 2.0, 3.0});
    s.push_column({2.0, 4.0, 1.0});
    s.push_column({3.0, 1.0, 4.0});
    s.push_column({4.0, 3.0, 2.0});
    s.push_column({5.0, 5.0, 5.0});

    const auto& corr = s.correlation_matrix();
    for (std::size_t i = 0; i < 3; ++i)
        ASSERT_CLOSE(corr(i, i), 1.0, 1e-12);
}

void test_mean_vector_matches_individual_means() {
    IncrementalStats<double> s(3, 5);
    s.push_column({1.0, 10.0, 100.0});
    s.push_column({2.0, 20.0, 200.0});
    s.push_column({3.0, 30.0, 300.0});

    const auto& v = s.mean_vector();
    ASSERT(v.size() == 3);
    for (std::size_t i = 0; i < 3; ++i)
        ASSERT_CLOSE(v[i], s.mean(i), 1e-12);
}

//int tests

void test_int_input() {
    IncrementalStats<int> s(2, 4);
    s.push_column({1, 10});
    s.push_column({2, 20});
    s.push_column({3, 30});
    s.push_column({4, 40});
    ASSERT_CLOSE(s.mean(0), 2.5, 1e-12);
    ASSERT_CLOSE(s.mean(1), 25.0, 1e-12);
    ASSERT_CLOSE(s.correlation(0, 1), 1.0, 1e-12);
}

int main() {
    RUN_TEST(test_constructor);
    RUN_TEST(test_push_advances_state);
    RUN_TEST(test_clear_resets_to_empty);

    RUN_TEST(test_mean_matches_hand_computed);
    RUN_TEST(test_variance_matches_hand_computed);
    RUN_TEST(test_covariance_matches_hand_computed);
    RUN_TEST(test_correlation_perfect_positive);
    RUN_TEST(test_correlation_perfect_negative);
    RUN_TEST(test_correlation_diagonal);
    RUN_TEST(test_constant_data_zero_correlation);

    RUN_TEST(test_equivalence_on_random_data);
    RUN_TEST(test_after_eviction_state_matches_naive);
    RUN_TEST(test_clear_refill_works);

    RUN_TEST(test_covariance_matrix_is_symmetric);
    RUN_TEST(test_correlation_matrix_diagonal_is_one);
    RUN_TEST(test_mean_vector_matches_individual_means);

    RUN_TEST(test_int_input);

    std::cout << "\nRan " << tests_run << " tests,  passed "
              << (tests_run - tests_failed) << ",  failed " << tests_failed << "\n";
    return tests_failed == 0 ? 0 : 1;
}