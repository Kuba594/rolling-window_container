#include "../src/kontejner.h"
#include <iostream>
#include <stdexcept>
#include <utility>
#include <iterator>
#include <numeric>
#include <algorithm>

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


//basic functionality tests
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

//copy ctor

void test_copy_ctor_similar() {
    RollingMatrix<double> a(2, 4);
    a.push_column({1.0, 10.0});
    a.push_column({2.0, 20.0});
    a.push_column({3.0, 30.0});

    RollingMatrix<double> b(a);

    ASSERT(b.rows()     == a.rows());
    ASSERT(b.cols()     == a.cols());
    ASSERT(b.capacity() == a.capacity());
    ASSERT(b(0, 0) == 1.0);
    ASSERT(b(1, 1) == 20.0);
    ASSERT(b(0, 2) == 3.0);
}

void test_copy_ctor_independence() {
    RollingMatrix<double> a(2, 3);
    a.push_column({1.0, 10.0});
    a.push_column({2.0, 20.0});

    RollingMatrix<double> b(a);

    a.push_column({3.0, 30.0});
    a.push_column({4.0, 40.0});

    ASSERT(b.cols()  == 2);
    ASSERT(b(0, 0) == 1.0);
    ASSERT(b(0, 1) == 2.0);
    ASSERT(a(0, 0) == 2.0);
}

void test_copy_ctor_after_rolling() {
    RollingMatrix<double> a(2, 3);
    a.push_column({1.0, 10.0});
    a.push_column({2.0, 20.0});
    a.push_column({3.0, 30.0});
    a.push_column({4.0, 40.0});

    RollingMatrix<double> b(a);

    ASSERT(b.cols() == 3);
    ASSERT(b(0, 0) == 2.0);
    ASSERT(b(0, 1) == 3.0);
    ASSERT(b(0, 2) == 4.0);
    ASSERT(b(1, 2) == 40.0);
}

//copy assign

void test_copy_assignment_independence() {
    RollingMatrix<double> a(2, 3);
    a.push_column({1.0, 10.0});
    a.push_column({2.0, 20.0});

    RollingMatrix<double> b(2, 3);
    b.push_column({99.0, 999.0});

    b = a;

    ASSERT(b.cols() == 2);
    ASSERT(b(0, 0) == 1.0);
    ASSERT(b(1, 1) == 20.0);
    ASSERT(a.cols() == 2);
    ASSERT(a(0, 0) == 1.0);
    ASSERT(a(1, 1) == 20.0);

    a.push_column({3.0, 30.0});
    ASSERT(a.cols() == 3);
    ASSERT(b.cols() == 2);
}

void test_copy_assignment_self_assignment_safe() {
    RollingMatrix<double> a(2, 3);
    a.push_column({1.0, 10.0});
    a.push_column({2.0, 20.0});

    a = a;

    ASSERT(a.cols() == 2);
    ASSERT(a(0, 0) == 1.0);
    ASSERT(a(1, 1) == 20.0);
}

void test_copy_assignment_different_dims() {
    RollingMatrix<double> a(2, 3);
    a.push_column({1.0, 10.0});

    RollingMatrix<double> b(5, 7);
    b.push_column({0,0,0,0,0});

    b = a;
    ASSERT(b.rows()     == 2);
    ASSERT(b.capacity() == 3);
    ASSERT(b.cols()     == 1);
    ASSERT(b(0, 0) == 1.0);
}

//move ctor/assign

void test_move_ctor_transfers() {
    RollingMatrix<double> a(2, 3);
    a.push_column({1.0, 10.0});
    a.push_column({2.0, 20.0});

    RollingMatrix<double> b(std::move(a));

    ASSERT(b.cols()  == 2);
    ASSERT(b(0, 0) == 1.0);
    ASSERT(b(1, 1) == 20.0);
}

void test_move_assignment_transfers() {
    RollingMatrix<double> a(2, 3);
    a.push_column({1.0, 10.0});
    a.push_column({2.0, 20.0});

    RollingMatrix<double> b(2, 3);
    b.push_column({99.0, 999.0});

    b = std::move(a);

    ASSERT(b.cols()  == 2);
    ASSERT(b(0, 0) == 1.0);
    ASSERT(b(1, 1) == 20.0);
}

//swap

void test_swap_members() {
    RollingMatrix<double> a(2, 3);
    a.push_column({1.0, 10.0});
    a.push_column({2.0, 20.0});

    RollingMatrix<double> b(4, 5);
    b.push_column({100, 200, 300, 400});

    a.swap(b);

    ASSERT(a.rows()     == 4);
    ASSERT(a.capacity() == 5);
    ASSERT(a.cols()     == 1);
    ASSERT(a(0, 0) == 100);
    ASSERT(a(3, 0) == 400);

    ASSERT(b.rows()     == 2);
    ASSERT(b.capacity() == 3);
    ASSERT(b.cols()     == 2);
    ASSERT(b(0, 0) == 1.0);
    ASSERT(b(1, 1) == 20.0);
}
//== and != tests

void test_eq_empty_matrices() {
    RollingMatrix<double> a(2, 3);
    RollingMatrix<double> b(2, 3);
    ASSERT(a == b);
    ASSERT(!(a != b));
}

void test_eq_same_inside() {
    RollingMatrix<double> a(2, 3);
    a.push_column({1.0, 10.0});
    a.push_column({2.0, 20.0});

    RollingMatrix<double> b(2, 3);
    b.push_column({1.0, 10.0});
    b.push_column({2.0, 20.0});

    ASSERT(a == b);
    ASSERT(!(a != b));
}

void test_ineq_different_dims() {
    RollingMatrix<double> a(2, 3);
    RollingMatrix<double> b(3, 3);
    ASSERT(a != b);
    ASSERT(!(a == b));
}

void test_ineq_diff_cols_inside_size() {
    RollingMatrix<double> a(2, 3);
    a.push_column({1.0, 10.0});

    RollingMatrix<double> b(2, 3);
    b.push_column({1.0, 10.0});
    b.push_column({2.0, 20.0});

    ASSERT(a != b);
}

void test_ineq_diff_values() {
    RollingMatrix<double> a(2, 3);
    a.push_column({1.0, 10.0});
    a.push_column({2.0, 20.0});

    RollingMatrix<double> b(2, 3);
    b.push_column({1.0, 10.0});
    b.push_column({2.0, 19.0});

    ASSERT(a != b);
    ASSERT(!(a == b));
}

void test_eq_after_rolling() {
    RollingMatrix<double> a(2, 3);
    a.push_column({1.0, 10.0});
    a.push_column({2.0, 20.0});
    a.push_column({3.0, 30.0});
    a.push_column({4.0, 40.0});

    RollingMatrix<double> b(2, 3);
    b.push_column({2.0, 20.0});
    b.push_column({3.0, 30.0});
    b.push_column({4.0, 40.0});

    ASSERT(a == b);
}

void test_self_eq() {
    RollingMatrix<double> a(2, 3);
    a.push_column({1.0, 10.0});
    a.push_column({2.0, 20.0});
    ASSERT(a == a);
    ASSERT(!(a != a));
}

void test_eq_copy() {
    RollingMatrix<double> a(2, 3);
    a.push_column({1.0, 10.0});
    a.push_column({2.0, 20.0});

    RollingMatrix<double> b(a);
    ASSERT(a == b);

    RollingMatrix<double> c(2, 3);
    c = a;
    ASSERT(a == c);
}

void test_eq_sym() {
    RollingMatrix<double> a(2, 3);
    a.push_column({1.0, 10.0});

    RollingMatrix<double> b(2, 3);
    b.push_column({1.0, 10.0});

    ASSERT(a == b);
    ASSERT(b == a);
}

//column_view
void test_column_view_size_read() {
    double b[3] = {1.0, 2.0, 3.0};
    RollingMatrix<double>::column_view v(b, 3);

    ASSERT(v.size() == 3);
    ASSERT(v[0] == 1.0);
    ASSERT(v[1] == 2.0);
    ASSERT(v[2] == 3.0);
}

void test_column_view_write() {
    double b[3] = {1.0, 2.0, 3.0};
    RollingMatrix<double>::column_view v(b, 3);

    v[0] = 99.0;
    v[2] = -5.5;

    ASSERT(b[0] == 99.0);
    ASSERT(b[1] == 2.0);
    ASSERT(b[2] == -5.5);
}


void test_column_view_size_and_read() {
    double buf[3] = {1.0, 2.0, 3.0};
    RollingMatrix<double>::column_view v(buf, 3);

    ASSERT(v.size() == 3);
    ASSERT(v[0] == 1.0);
    ASSERT(v[1] == 2.0);
    ASSERT(v[2] == 3.0);
}

void test_column_view_write_mutates_buffer() {
    double buf[3] = {1.0, 2.0, 3.0};
    RollingMatrix<double>::column_view v(buf, 3);

    v[0] = 99.0;
    v[2] = -5.5;

    ASSERT(buf[0] == 99.0);          // view writes through to underlying buffer
    ASSERT(buf[1] == 2.0);           // untouched
    ASSERT(buf[2] == -5.5);
}

void test_column_view_const_returns_const_ref() {
    double buf[2] = {10.0, 20.0};
    const RollingMatrix<double>::column_view v(buf, 2);   // view itself is const

    ASSERT(v[0] == 10.0);
    ASSERT(v[1] == 20.0);
    ASSERT(v.size() == 2);

    // The following must NOT compile (uncomment to verify):
    // v[0] = 99.0;   // error: assignment to const reference
}

void test_column_view_zero_size() {
    // Edge case: zero-length view (degenerate but should not crash)
    double dummy = 0;
    RollingMatrix<double>::column_view v(&dummy, 0);
    ASSERT(v.size() == 0);
}

void test_column_view_with_int_type() {
    int buf[4] = {7, 8, 9, 10};
    RollingMatrix<int>::column_view v(buf, 4);
    ASSERT(v.size() == 4);
    ASSERT(v[0] == 7);
    ASSERT(v[3] == 10);
    v[2] = 999;
    ASSERT(buf[2] == 999);
}

void test_column_view_independent_from_matrix() {
    // Verify a column_view we built ourselves doesn't depend on a RollingMatrix.
    // It just wraps a raw pointer — that's the whole design.
    std::vector<double> data = {1.1, 2.2, 3.3, 4.4, 5.5};
    RollingMatrix<double>::column_view v(data.data() + 1, 3);   // points at &data[1]
    ASSERT(v.size() == 3);
    ASSERT(v[0] == 2.2);
    ASSERT(v[1] == 3.3);
    ASSERT(v[2] == 4.4);
}

// ===== NEW: column_iterator tests ============================================

void test_iterator_begin_end_basic() {
    RollingMatrix<double> m(2, 3);
    ASSERT(m.begin() == m.end());          // empty: begin == end

    m.push_column({1.0, 10.0});
    ASSERT(m.begin() != m.end());          // one column: begin != end
    ASSERT(m.end() - m.begin() == 1);
}

void test_iterator_distance_equals_cols() {
    RollingMatrix<double> m(2, 5);
    m.push_column({1.0, 10.0});
    m.push_column({2.0, 20.0});
    m.push_column({3.0, 30.0});

    ASSERT(m.end() - m.begin() == 3);
    ASSERT(std::distance(m.begin(), m.end()) == 3);
}

void test_iterator_dereference_yields_correct_column() {
    RollingMatrix<double> m(2, 3);
    m.push_column({1.0, 10.0});
    m.push_column({2.0, 20.0});

    auto it = m.begin();
    auto col0 = *it;
    ASSERT(col0.size() == 2);
    ASSERT(col0[0] == 1.0);
    ASSERT(col0[1] == 10.0);

    ++it;
    auto col1 = *it;
    ASSERT(col1[0] == 2.0);
    ASSERT(col1[1] == 20.0);
}

void test_iterator_increment_decrement() {
    RollingMatrix<double> m(2, 3);
    m.push_column({1.0, 10.0});
    m.push_column({2.0, 20.0});
    m.push_column({3.0, 30.0});

    auto it = m.begin();
    auto a  = *it++;                       // postfix: returns old, advances
    ASSERT(a[0] == 1.0);
    ASSERT((*it)[0] == 2.0);

    ++it;                                  // prefix: advances first
    ASSERT((*it)[0] == 3.0);

    auto b = *it--;                        // postfix decrement
    ASSERT(b[0] == 3.0);
    ASSERT((*it)[0] == 2.0);
}

void test_iterator_random_access_arithmetic() {
    RollingMatrix<double> m(2, 4);
    m.push_column({1.0, 10.0});
    m.push_column({2.0, 20.0});
    m.push_column({3.0, 30.0});
    m.push_column({4.0, 40.0});

    auto it = m.begin();

    ASSERT((*(it + 2))[0] == 3.0);          // it + n
    ASSERT(((it + 3) - it) == 3);           // it - it
    ASSERT((*(it + 1 + 2))[0] == 4.0);

    auto it2 = it + 3;
    ASSERT((*(it2 - 2))[0] == (*(it + 1))[0]);   // it - n: compare contents
    ASSERT(it2[-1][0]      == (*(it + 2))[0]);    // negative subscript
}

void test_iterator_subscript_operator() {
    RollingMatrix<double> m(2, 3);
    m.push_column({1.0, 10.0});
    m.push_column({2.0, 20.0});
    m.push_column({3.0, 30.0});

    auto it = m.begin();
    ASSERT(it[0][0] == 1.0);                // it[n] same as *(it + n)
    ASSERT(it[1][0] == 2.0);
    ASSERT(it[2][0] == 3.0);
    ASSERT(it[2][1] == 30.0);
}

void test_iterator_compound_assignment() {
    RollingMatrix<double> m(2, 4);
    m.push_column({1.0, 10.0});
    m.push_column({2.0, 20.0});
    m.push_column({3.0, 30.0});
    m.push_column({4.0, 40.0});

    auto it = m.begin();
    it += 2;
    ASSERT((*it)[0] == 3.0);

    it -= 1;
    ASSERT((*it)[0] == 2.0);
}

void test_iterator_comparison_operators() {
    RollingMatrix<double> m(2, 4);
    m.push_column({1.0, 10.0});
    m.push_column({2.0, 20.0});
    m.push_column({3.0, 30.0});

    auto a = m.begin();
    auto b = m.begin() + 1;

    ASSERT(a < b);
    ASSERT(a <= b);
    ASSERT(b > a);
    ASSERT(b >= a);
    ASSERT(a != b);
    ASSERT(!(a == b));

    auto c = m.begin();
    ASSERT(a == c);
    ASSERT(a <= c);
    ASSERT(a >= c);
    ASSERT(!(a < c));
}

void test_iterator_after_rolling() {
    // The most important test: when head_ != 0, the iterator must still
    // walk in chronological order.
    RollingMatrix<double> m(2, 3);
    m.push_column({1.0, 10.0});
    m.push_column({2.0, 20.0});
    m.push_column({3.0, 30.0});
    m.push_column({4.0, 40.0});             // evicts {1, 10}; head_ now 1

    auto it = m.begin();                     // points to oldest = {2, 20}
    ASSERT((*it)[0]      == 2.0);
    ASSERT((*(it + 1))[0] == 3.0);
    ASSERT((*(it + 2))[0] == 4.0);
    ASSERT(it + 3 == m.end());
}

void test_iterator_range_for_loop() {
    RollingMatrix<double> m(2, 3);
    m.push_column({1.0, 10.0});
    m.push_column({2.0, 20.0});
    m.push_column({3.0, 30.0});

    double sum0 = 0;       // sum of asset 0 across columns
    double sum1 = 0;       // sum of asset 1 across columns
    std::size_t count = 0;
    for (auto col : m) {
        sum0 += col[0];
        sum1 += col[1];
        ++count;
    }
    ASSERT(count == 3);
    ASSERT(sum0  == 6.0);    // 1 + 2 + 3
    ASSERT(sum1  == 60.0);   // 10 + 20 + 30
}

void test_iterator_works_with_std_algorithms() {
    RollingMatrix<double> m(2, 3);
    m.push_column({1.0, 10.0});
    m.push_column({2.0, 20.0});
    m.push_column({3.0, 30.0});

    // std::accumulate over columns, summing asset[0]
    double sum = std::accumulate(
        m.begin(), m.end(), 0.0,
        [](double acc, RollingMatrix<double>::column_view col) {
            return acc + col[0];
        });
    ASSERT(sum == 6.0);

    // std::find_if to find a specific column
    auto it = std::find_if(m.begin(), m.end(),
        [](RollingMatrix<double>::column_view col) {
            return col[0] == 2.0;
        });
    ASSERT(it != m.end());
    ASSERT((*it)[1] == 20.0);
}

void test_iterator_empty_matrix() {
    RollingMatrix<double> m(2, 3);
    ASSERT(m.begin() == m.end());
    ASSERT(std::distance(m.begin(), m.end()) == 0);

    std::size_t count = 0;
    for (auto col : m) { (void)col; ++count; }
    ASSERT(count == 0);                      // body never executes
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

    RUN_TEST(test_copy_ctor_similar);
    RUN_TEST(test_copy_ctor_independence);
    RUN_TEST(test_copy_ctor_after_rolling);
    RUN_TEST(test_copy_assignment_independence);
    RUN_TEST(test_copy_assignment_self_assignment_safe);
    RUN_TEST(test_copy_assignment_different_dims);
    RUN_TEST(test_move_ctor_transfers);
    RUN_TEST(test_move_assignment_transfers);
    RUN_TEST(test_swap_members);

    RUN_TEST(test_eq_empty_matrices);
    RUN_TEST(test_eq_same_inside);
    RUN_TEST(test_ineq_different_dims);
    RUN_TEST(test_ineq_diff_cols_inside_size);
    RUN_TEST(test_ineq_diff_values);
    RUN_TEST(test_eq_after_rolling);
    RUN_TEST(test_self_eq);
    RUN_TEST(test_eq_copy);
    RUN_TEST(test_eq_sym);

    RUN_TEST(test_column_view_size_and_read);
    RUN_TEST(test_column_view_write_mutates_buffer);
    RUN_TEST(test_column_view_const_returns_const_ref);
    RUN_TEST(test_column_view_zero_size);
    RUN_TEST(test_column_view_with_int_type);
    RUN_TEST(test_column_view_independent_from_matrix);

    RUN_TEST(test_iterator_begin_end_basic);
    RUN_TEST(test_iterator_distance_equals_cols);
    RUN_TEST(test_iterator_dereference_yields_correct_column);
    RUN_TEST(test_iterator_increment_decrement);
    RUN_TEST(test_iterator_random_access_arithmetic);
    RUN_TEST(test_iterator_subscript_operator);
    RUN_TEST(test_iterator_compound_assignment);
    RUN_TEST(test_iterator_comparison_operators);
    RUN_TEST(test_iterator_after_rolling);
    RUN_TEST(test_iterator_range_for_loop);
    RUN_TEST(test_iterator_works_with_std_algorithms);
    RUN_TEST(test_iterator_empty_matrix);

    std::cout << "\n"
              << "Ran "    << tests_run     << " tests,  "
              << "passed " << (tests_run - tests_failed)
              << ",  failed " << tests_failed << "\n";
    return tests_failed == 0 ? 0 : 1;
}