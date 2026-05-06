#include "../src/kontejner.h"
#include <iostream>
#include <stdexcept>

// ===== minimal in-house test runner (no external deps) =======================

static int tests_run    = 0;
static int tests_failed = 0;

#define ASSERT(cond) do {                                                       \
    if (!(cond)) {                                                              \
        std::cerr << "    FAIL: " << #cond                                      \
                  << "  (line " << __LINE__ << ")\n";                           \
        ++tests_failed;                                                         \
        return;                                                                 \
    }                                                                           \
} while (0)

#define ASSERT_THROWS(expr, ex_type) do {                                       \
    bool caught = false;                                                        \
    try { (void)(expr); }                                                       \
    catch (const ex_type&) { caught = true; }                                   \
    if (!caught) {                                                              \
        std::cerr << "    FAIL: " << #expr                                      \
                  << " did not throw " #ex_type                                 \
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

// ===== tests =================================================================

void test_constructor_basic() {
    RollingMatrix<double> m(3, 5);
    ASSERT(m.rows()     == 3);
    ASSERT(m.capacity() == 5);
    ASSERT(m.cols()     == 0);
    ASSERT(m.empty());
    ASSERT(!m.full());
}

void test_constructor_rejects_zero_dimensions() {
    ASSERT_THROWS(RollingMatrix<double>(0, 5), std::invalid_argument);
    ASSERT_THROWS(RollingMatrix<double>(3, 0), std::invalid_argument);
    ASSERT_THROWS(RollingMatrix<double>(0, 0), std::invalid_argument);
}

void test_push_fills_window() {
    RollingMatrix<double> m(2, 3);

    m.push_column({1.0, 10.0});
    ASSERT(m.cols() == 1);
    ASSERT(!m.full());
    ASSERT(m(0, 0) == 1.0);
    ASSERT(m(1, 0) == 10.0);

    m.push_column({2.0, 20.0});
    m.push_column({3.0, 30.0});
    ASSERT(m.cols() == 3);
    ASSERT(m.full());
    ASSERT(m(0, 0) == 1.0);
    ASSERT(m(0, 2) == 3.0);
    ASSERT(m(1, 0) == 10.0);
    ASSERT(m(1, 2) == 30.0);
}

void test_rolling_overwrites_oldest() {
    RollingMatrix<double> m(2, 3);
    m.push_column({1.0, 10.0});
    m.push_column({2.0, 20.0});
    m.push_column({3.0, 30.0});
    m.push_column({4.0, 40.0});

    ASSERT(m.cols() == 3);
    ASSERT(m.full());
    ASSERT(m(0, 0) == 2.0);
    ASSERT(m(0, 1) == 3.0);
    ASSERT(m(0, 2) == 4.0);
    ASSERT(m(1, 2) == 40.0);

    m.push_column({5.0, 50.0});
    ASSERT(m(0, 0) == 3.0);
    ASSERT(m(0, 2) == 5.0);
}

void test_at_throws_on_bad_index() {
    RollingMatrix<double> m(2, 3);
    m.push_column({1.0, 10.0});

    ASSERT_THROWS(m.at(2, 0), std::out_of_range);
    ASSERT_THROWS(m.at(0, 1), std::out_of_range);
    ASSERT_THROWS(m.at(0, 99), std::out_of_range);
}

void test_push_wrong_column_size_throws() {
    RollingMatrix<double> m(2, 3);
    ASSERT_THROWS(m.push_column({1.0}), std::invalid_argument);
    ASSERT_THROWS(m.push_column({1.0, 2.0, 3.0}), std::invalid_argument);
}

void test_clear_resets_state() {
    RollingMatrix<double> m(2, 3);
    m.push_column({1.0, 10.0});
    m.push_column({2.0, 20.0});
    m.push_column({3.0, 30.0});
    m.push_column({4.0, 40.0});

    m.clear();
    ASSERT(m.cols() == 0);
    ASSERT(m.empty());
    ASSERT(!m.full());

    m.push_column({99.0, 999.0});
    ASSERT(m.cols() == 1);
    ASSERT(m(0, 0) == 99.0);
}

void test_template_works_with_int() {
    RollingMatrix<int> m(2, 2);
    m.push_column({1, 2});
    m.push_column({3, 4});
    ASSERT(m(0, 0) == 1);
    ASSERT(m(1, 1) == 4);
}

void test_unchecked_operator_does_not_throw() {
    RollingMatrix<double> m(2, 3);
    m.push_column({1.0, 10.0});
    double v = m(0, 0);
    ASSERT(v == 1.0);
}

// ===== entry point ===========================================================

int main() {
    RUN_TEST(test_constructor_basic);
    RUN_TEST(test_constructor_rejects_zero_dimensions);
    RUN_TEST(test_push_fills_window);
    RUN_TEST(test_rolling_overwrites_oldest);
    RUN_TEST(test_at_throws_on_bad_index);
    RUN_TEST(test_push_wrong_column_size_throws);
    RUN_TEST(test_clear_resets_state);
    RUN_TEST(test_template_works_with_int);
    RUN_TEST(test_unchecked_operator_does_not_throw);

    std::cout << "\n"
              << "Ran "    << tests_run     << " tests,  "
              << "passed " << (tests_run - tests_failed)
              << ",  failed " << tests_failed << "\n";
    return tests_failed == 0 ? 0 : 1;
}