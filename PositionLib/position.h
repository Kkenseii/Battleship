#pragma once
#include <string>

class Position {
    int _row;
    int _col;
    static const int _max_row = 10;
    static const int _max_col = 10;

public:
    inline int get_row() const noexcept {
        return _row;
    }
    int get_col() const noexcept {
        return _col;
    }

    void set_row(int row);
    void set_col(int col);

    Position();
    Position(int row, int col);
    Position(const Position& other);
    Position(const std::string& str);

    friend std::string to_string(const Position& pos);
    friend Position parse(const std::string& str);

};