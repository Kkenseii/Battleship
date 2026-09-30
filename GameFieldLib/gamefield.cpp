#include "GameField.h"
#include <stdexcept>

void GameField::init_field() {
    _field = new char* [_n];
    for (int i = 0; i < _n; ++i) {
        _field[i] = new char[_m];
        for (int j = 0; j < _m; ++j) {
            _field[i][j] = ' ';
        }
    }
}

void GameField::copy_field(char* const* src) {
    _field = new char* [_n];
    for (int i = 0; i < _n; ++i) {
        _field[i] = new char[_m];
        for (int j = 0; j < _m; ++j) {
            _field[i][j] = src[i][j];
        }
    }
}

void GameField::free_field() noexcept {
    if (_field) {
        for (int i = 0; i < _n; ++i) {
            delete[] _field[i];
        }
        delete[] _field;
    }
}

void GameField::check_size(int n, int m) {
    if (n <= 0 || m <= 0 || n > 25 || m > 25) {
        throw std::logic_error("Invalid input: incorrect field parameters");
    }
}

int GameField::col_to_index(char col) {
    char upperCol = col;
    if (col >= 'a' && col <= 'z') {
        upperCol = static_cast<char>(col - 'a' + 'A');
    }
    if (upperCol < 'A' || upperCol > 'Z') {
        throw std::logic_error("Invalid input: incorrect position");
    }
    return upperCol - 'A' + 1;
}

void GameField::mark(int row, char col, char symbol) {
    int colNum = col_to_index(col);
    if (row < 1 || row > _n || colNum < 1 || colNum > _m) {
        throw std::logic_error("Invalid input: incorrect position");
    }
    _field[row - 1][colNum - 1] = symbol;
}

GameField::GameField() : _field(nullptr), _n(10), _m(10) {
    init_field();
}

GameField::GameField(char** field, int n, int m) : _field(nullptr), _n(n), _m(m) {
    check_size(n, m);
    copy_field(field);
}

GameField::GameField(int n, int m) : _field(nullptr), _n(n), _m(m) {
    check_size(n, m);
    init_field();
}

GameField::GameField(const GameField& other) : _field(nullptr), _n(other._n), _m(other._m) {
    copy_field(other._field);
}

GameField::~GameField(){
    free_field();
}

void GameField::set(int row, char col) {
    mark(row, col, '*');
}

char GameField::get(int row, char col) const {
    int colNum = col_to_index(col);
    if (row < 1 || row > _n || colNum < 1 || colNum > _m) {
        throw std::logic_error("Invalid input: incorrect position");
    }
    return _field[row - 1][colNum - 1];
}

std::string GameField::render(const GameField& field, bool hide_ships, bool use_hide) {
    std::string res;
    res += "  |";
    for (int j = 0; j < field._m; ++j) {
        res += static_cast<char>('A' + j);
        if (j < field._m - 1) res += ' ';
    }
    res += "|\n";

    res += "  +";
    for (int j = 0; j < field._m - 1; ++j) {
        res += "--";
    }
    res += "-+\n";

    for (int i = 0; i < field._n; ++i) {
        std::string label = std::to_string(i + 1);
        if (label.size() < 2) label += ' ';
        res += label;
        res += "|";
        for (int j = 0; j < field._m; ++j) {
            char c = field._field[i][j];
            if (use_hide && hide_ships && c == '*') c = ' ';
            res += c;
            res += '|';
        }
        res += "\n";
    }

    res += "  +";
    for (int j = 0; j < field._m - 1; ++j) {
        res += "--";
    }
    res += "-+";
    return res;
}

std::string to_string(const GameField& field, bool hide_ships = 0) {
    return GameField::render(field, hide_ships, true);
}