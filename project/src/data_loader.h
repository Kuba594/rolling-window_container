#ifndef PROJECT_DATA_LOADER_H
#define PROJECT_DATA_LOADER_H
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include "matrix.h"

class CsvReader {
public:
    explicit CsvReader(const std::string& path, bool has_header = true);

    const std::vector<std::string>& headers() const noexcept;
    std::size_t expected_cols() const noexcept;

    bool next(std::vector<double>& out);

private:
    std::ifstream file_;
    std::vector<std::string> headers_;
    std::vector<std::string> cell_buffer_;
    std::size_t expected_cols_ {0};
    std::size_t line_num_ {0};
};

// Write a matrix as CSV.
//
// If `labels` is non-empty, it's used both as the header row and as the
// leftmost label column (e.g., for a labeled covariance matrix where
// rows and columns are asset names):
//
//          AAPL,    GOOG,    MSFT
//   AAPL,  0.001,   0.0003,  0.0002
//   GOOG,  0.0003,  0.002,   0.0001
//   MSFT,  0.0002,  0.0001,  0.0008
//
// If `labels` is empty, writes a plain matrix with no headers.
//
// Throws std::runtime_error if the file cannot be opened.
// Throws std::invalid_argument if labels.size() != m.rows() (must equal
// m.cols() too, since covariance matrices are square).
void write_matrix(const std::string& path, const Matrix<double>& m,const std::vector<std::string>& labels = {});
#endif