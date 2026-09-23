#pragma once
#include <string>
#include <stdexcept>

class GameField;

namespace detail_gamefield {
    std::string draw(const GameField& field, bool hide_ships, bool use_hide);
}

class GameField {
    char** _field;
    const int _n;
    const int _m;

    void init_field() {
        _field = new char* [_n];
        for (int i = 0; i < _n; ++i) {
            _field[i] = new char[_m];
            for (int j = 0; j < _m; ++j) {
                _field[i][j] = ' ';
            }
        }
    }

    void copy_field(char* const* src) {
        _field = new char* [_n];
        for (int i = 0; i < _n; ++i) {
            _field[i] = new char[_m];
            for (int j = 0; j < _m; ++j) {
                _field[i][j] = src[i][j];
            }
        }
    }

    void free_field() noexcept {
        if (_field) {
            for (int i = 0; i < _n; ++i) {
                delete[] _field[i];
            }
            delete[] _field;
        }
    }

    static void check_size(int n, int m) {
        if (n <= 0 || m <= 0 || n > 25 || m > 25) {
            throw std::logic_error("Invalid input: incorrect field parameters");
        }
    }

    static int col_to_index(char col) {
        char upperCol = col;
        if (col >= 'a' && col <= 'z') {
            upperCol = static_cast<char>(col - 'a' + 'A');
        }
        if (upperCol < 'A' || upperCol > 'Z') {
            throw std::logic_error("Invalid input: incorrect position");
        }
        return upperCol - 'A' + 1;
    }

    void mark(int row, char col, char symbol) {
        int colNum = col_to_index(col);
        if (row < 1 || row > _n || colNum < 1 || colNum > _m) {
            throw std::logic_error("Invalid input: incorrect position");
        }
        _field[row - 1][colNum - 1] = symbol;
    }

    friend class Player;

public:
    GameField();
    GameField(char** field, int n, int m);
    GameField(int n, int m);
    GameField(const GameField& other);
    ~GameField() noexcept;

    void set(int row, char col);
    char get(int row, char col) const;

    friend std::string to_string(const GameField& field);
    friend std::string to_string(const GameField& field, bool hide_ships);
    friend std::string detail_gamefield::draw(const GameField& field, bool hide_ships, bool use_hide);

};

inline GameField::GameField() : _field(nullptr), _n(10), _m(10) {
    init_field();
}

inline GameField::GameField(char** field, int n, int m) : _field(nullptr), _n(n), _m(m) {
    check_size(n, m);
    copy_field(field);
}

inline GameField::GameField(int n, int m) : _field(nullptr), _n(n), _m(m) {
    check_size(n, m);
    init_field();
}

inline GameField::GameField(const GameField& other) : _field(nullptr), _n(other._n), _m(other._m) {
    copy_field(other._field);
}

inline GameField::~GameField() noexcept {
    free_field();
}

inline void GameField::set(int row, char col) {
    mark(row, col, '*');
}

inline char GameField::get(int row, char col) const {
    int colNum = col_to_index(col);
    if (row < 1 || row > _n || colNum < 1 || colNum > _m) {
        throw std::logic_error("Invalid input: incorrect position");
    }
    return _field[row - 1][colNum - 1];
}

namespace detail_gamefield {
    inline std::string draw(const GameField& field, bool hide_ships, bool use_hide) {
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
}

inline std::string to_string(const GameField& field) {
    return detail_gamefield::draw(field, false, false);
}

inline std::string to_string(const GameField& field, bool hide_ships) {
    return detail_gamefield::draw(field, hide_ships, true);
}