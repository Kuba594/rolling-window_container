#ifndef PROJECT_DATA_LOADER_H
#define PROJECT_DATA_LOADER_H
#include <string>
#include <vector>
#include <fstream>
#include "matrix.h"
//class that helps with loading csv files. It creates
class CsvReader {
public:
    CsvReader(const std::string& path, bool has_header = true);

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

//Helper method that writes matrix to a file
void write_matrix(const std::string& path, const Matrix<double>& m,const std::vector<std::string>& labels = {});
#endif