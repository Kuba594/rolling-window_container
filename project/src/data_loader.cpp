#include "data_loader.h"

#include <sstream>
#include <stdexcept>
#include <string>


std::string trim(const std::string& s) {
    size_t first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last  = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

void split_csv(const std::string& line, std::vector<std::string>& out) {
    out.clear();
    std::stringstream ss(line);
    std::string cell;
    while (std::getline(ss, cell, ',')) {
        out.push_back(trim(cell));
    }
}

CsvReader::CsvReader(const std::string& path, bool has_header) {
    file_.open(path);
    if (!file_.is_open()) {
        throw std::runtime_error("CsvReader: can't open file: " + path);
    }

    if (has_header) {
        std::string line;
        if (std::getline(file_, line)) {
            ++line_num_;
            split_csv(line, headers_);
            expected_cols_ = headers_.size();
        }
    }
}

const std::vector<std::string>& CsvReader::headers() const noexcept {
    return headers_;
}

std::size_t CsvReader::expected_cols() const noexcept {
    return expected_cols_;
}

bool CsvReader::next(std::vector<double>& out) {
    std::string line;
    while (std::getline(file_, line)) {
        ++line_num_;
        if (trim(line).empty()) continue;

        split_csv(line, cell_buffer_);
        out.clear();
        out.reserve(cell_buffer_.size());

        for (const auto& cell : cell_buffer_) {
            try {
                std::size_t pos = 0;
                double v = std::stod(cell, &pos);
                if (pos != cell.size()) {
                    throw std::invalid_argument("trailing characters");
                }
                out.push_back(v);
            } catch (const std::exception&) {
                throw std::runtime_error("CsvReader: cannot parse '" + cell + "'on line " + std::to_string(line_num_));
            }
        }

        if (expected_cols_ == 0) expected_cols_ = out.size();
        if (out.size() != expected_cols_) {
            throw std::runtime_error("CsvReader: line " + std::to_string(line_num_) +" has " + std::to_string(out.size()) +
                " columns, expected " + std::to_string(expected_cols_));
        }
        return true;
    }
    return false;
}

void write_matrix(const std::string& path, const Matrix<double>& m, const std::vector<std::string>& labels) {

    if (!labels.empty()) {
        if (m.rows() != m.cols()) {
            throw std::invalid_argument(
                "write_matrix: labels need square matrices");
        }
        if (labels.size() != m.rows()) {
            throw std::invalid_argument(
                "write_matrix: labels.size() must equal m.rows()");
        }
    }

    std::ofstream f(path);
    if (!f.is_open()) {
        throw std::runtime_error("write_matrix: cannot open file: " + path);
    }

    if (!labels.empty()) {
        bool first = true;
        for (auto&& label : labels) {
            if (!first) f << ',';
            f << label;
            first = false;
        }
        f << '\n';
    }

    for (std::size_t i = 0; i < m.rows(); ++i) {
        if (!labels.empty()) f << labels[i];
        for (std::size_t j = 0; j < m.cols(); ++j) {
            if (!labels.empty() || j > 0) f << ',';
            f << m(i, j);
        }
        f << '\n';
    }
}
