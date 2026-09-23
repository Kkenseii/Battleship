#pragma once
#include "../GameFieldLib/gamefield.h"
#include "../ShipLib/ship.h"
#include <iostream>
#include <string>
#include <stdexcept>

enum State { Missed, BoatDestroyed, DestroyersDestroyed, CruisersDestroyed, BattleshipDestroyed, Hit };

class Player {
    GameField _gamefield;
    int _ships_counts[4];

    static constexpr int _max_ships_counts[4] = { 4, 3, 2, 1 };

    static inline char num_to_col(int c) noexcept {
        return static_cast<char>('A' + c - 1);
    }

    bool is_ship_cell(int row, int col) const {
        if (row < 1 || row > 10 || col < 1 || col > 10) return false;
        char c = _gamefield.get(row, num_to_col(col));
        return (c == '*' || c == 'X');
    }

    int sunk_ship_size(int row, int col) const {
        int left = col, right = col;
        while (left > 1 && is_ship_cell(row, left - 1)) --left;
        while (right < 10 && is_ship_cell(row, right + 1)) ++right;
        if (right > left) {
            for (int c = left; c <= right; ++c) {
                if (_gamefield.get(row, num_to_col(c)) != 'X') return 0;
            }
            return right - left + 1;
        }

        int top = row, bottom = row;
        while (top > 1 && is_ship_cell(top - 1, col)) --top;
        while (bottom < 10 && is_ship_cell(bottom + 1, col)) ++bottom;
        if (bottom > top) {
            for (int r = top; r <= bottom; ++r) {
                if (_gamefield.get(r, num_to_col(col)) != 'X') return 0;
            }
            return bottom - top + 1;
        }

        return 1;
    }

public:
    Player() noexcept;

    void set_ship(const Ship& ship);
    State set_action(int row, char col);
    void show_field(bool hide_ships = false) const;

    bool check_lose() const noexcept;
    bool check_ready() const noexcept;

};

inline Player::Player() noexcept : _gamefield() {
    for (int i = 0; i < 4; ++i) {
        _ships_counts[i] = 0;
    }
}

inline void Player::set_ship(const Ship& ship) {
    int size = ship.size();
    if (size < 1 || size > 4) {
        throw std::logic_error("Invalid input: incorrect field");
    }
    if (_ships_counts[size - 1] >= _max_ships_counts[size - 1]) {
        throw std::logic_error("Invalid input: incorrect field");
    }

    int row = ship.row();
    int col = ship.col();
    Direction dir = ship.direction();

    int rowEnd = row + (dir == Vertical ? size - 1 : 0);
    int colEnd = col + (dir == Horizontal ? size - 1 : 0);

    if (row < 1 || col < 1 || rowEnd > 10 || colEnd > 10) {
        throw std::logic_error("Invalid input: incorrect field");
    }

    int rFrom = (row > 1) ? row - 1 : 1;
    int rTo = (rowEnd < 10) ? rowEnd + 1 : 10;
    int cFrom = (col > 1) ? col - 1 : 1;
    int cTo = (colEnd < 10) ? colEnd + 1 : 10;

    for (int r = rFrom; r <= rTo; ++r) {
        for (int c = cFrom; c <= cTo; ++c) {
            if (_gamefield.get(r, num_to_col(c)) != ' ') {
                throw std::logic_error("Invalid input: incorrect field");
            }
        }
    }

    for (int i = 0; i < size; ++i) {
        int r = row + (dir == Vertical ? i : 0);
        int c = col + (dir == Horizontal ? i : 0);
        _gamefield.set(r, num_to_col(c));
    }

    _ships_counts[size - 1]++;
}

inline State Player::set_action(int row, char col) {
    char current;
    try {
        current = _gamefield.get(row, col);
    }
    catch (...) {
        throw std::logic_error("Invalid input: incorrect move");
    }

    if (current == '.' || current == 'X') {
        throw std::logic_error("Invalid input: incorrect move");
    }

    if (current == ' ') {
        _gamefield.mark(row, col, '.');
        return State::Missed;
    }

    _gamefield.mark(row, col, 'X');

    char upperCol = col;
    if (upperCol >= 'a' && upperCol <= 'z') upperCol = static_cast<char>(upperCol - 'a' + 'A');
    int colNum = upperCol - 'A' + 1;

    int sunkSize = sunk_ship_size(row, colNum);
    if (sunkSize == 0) {
        return State::Hit;
    }

    _ships_counts[sunkSize - 1]--;
    switch (sunkSize) {
    case 1: return State::BoatDestroyed;
    case 2: return State::DestroyersDestroyed;
    case 3: return State::CruisersDestroyed;
    case 4: return State::BattleshipDestroyed;
    default: return State::Hit;
    }
}

inline void Player::show_field(bool hide_ships) const {
    std::cout << to_string(_gamefield, hide_ships) << "\n\n";
    std::cout << "Ships Left:\n";
    std::cout << "* - " << _ships_counts[0] << " "
        << "** - " << _ships_counts[1] << " "
        << "*** - " << _ships_counts[2] << " "
        << "**** - " << _ships_counts[3] << "\n";
}

inline bool Player::check_lose() const noexcept {
    return (_ships_counts[0] == 0 &&
        _ships_counts[1] == 0 &&
        _ships_counts[2] == 0 &&
        _ships_counts[3] == 0);
}

inline bool Player::check_ready() const noexcept {
    return (_ships_counts[0] == _max_ships_counts[0] &&
        _ships_counts[1] == _max_ships_counts[1] &&
        _ships_counts[2] == _max_ships_counts[2] &&
        _ships_counts[3] == _max_ships_counts[3]);
}