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

    int size() const noexcept;
    Direction direction() const noexcept;
    int row() const noexcept;
    int col() const noexcept;

    void rotate();

};