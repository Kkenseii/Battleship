#pragma once
#include "../PositionLib/position.h"


enum Direction { Horizontal, Vertical };

class Ship {
    int _size;
    Position _coord;
    Direction _direction;

    static bool is_collision(int size, const Position& coord, Direction dir) noexcept;

public:
    Ship(int size, const Position& coord, Direction direction);
    Ship(int size, const Position& coord);
    Ship(int size, char direction, int row, char col);

    Ship() = delete;
    Ship(const Ship&) = delete;

    int get_size() const noexcept;
    Direction get_direction() const noexcept;
    int get_row() const noexcept;
    int get_col() const noexcept;

    void rotate();

};