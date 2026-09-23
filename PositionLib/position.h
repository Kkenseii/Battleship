#pragma once
#include <string>
#include <stdexcept>


class Position {
    int _row;
    int _col;
    static const int _max_row = 10;
    static const int _max_col = 10;

public:
    inline int row() const noexcept {
        return _row;
    }

    inline int col() const noexcept {
        return _col;
    }

    void row(int row);
    void col(int col);

    Position();
    Position(int row, int col);
    Position(const Position& other);
    Position(const std::string& str);

    friend std::string to_string(const Position& pos);
    friend Position parse(const std::string& str);

};

inline void Position::row(int row) {
    if (row < 1 || row > _max_row) {
        throw std::logic_error("Invalid input: incorrect position");
    }
    _row = row;
}

inline void Position::col(int col) {
    if (col < 1 || col > _max_col) {
        throw std::logic_error("Invalid input: incorrect position");
    }
    _col = col;
}

inline Position::Position() {
    _row = 1;
    _col = 1;
}

inline Position::Position(int row, int col) {
    if (row < 1 || row > _max_row || col < 1 || col > _max_col) {
        throw std::logic_error("Invalid input: incorrect position");
    }
    _row = row;
    _col = col;
}

inline Position::Position(const Position& other) {
    _row = other._row;
    _col = other._col;
}

inline Position parse(const std::string& str) {
    size_t openBracket = str.find('(');
    size_t comma = str.find(',');
    size_t closeBracket = str.find(')');

    if (openBracket == std::string::npos || comma == std::string::npos || closeBracket == std::string::npos) {
        throw std::logic_error("Invalid input: incorrect position");
    }

    std::string rowStr = str.substr(openBracket + 1, comma - openBracket - 1);
    std::string colStr = str.substr(comma + 1, closeBracket - comma - 1);

    rowStr.erase(0, rowStr.find_first_not_of(" \t"));
    rowStr.erase(rowStr.find_last_not_of(" \t") + 1);
    colStr.erase(0, colStr.find_first_not_of(" \t"));
    colStr.erase(colStr.find_last_not_of(" \t") + 1);

    int row;
    int col;
    try {
        row = std::stoi(rowStr);
        col = std::stoi(colStr);
    }
    catch (...) {
        throw std::logic_error("Invalid input: incorrect position");
    }

    return Position(row, col);
}

inline Position::Position(const std::string& str) {
    *this = parse(str);
}

inline std::string to_string(const Position& other) {
    std::string res;
    res += '(';
    res += std::to_string(other._row);
    res += ", ";
    res += std::to_string(other._col);
    res += ')';
    return res;
}