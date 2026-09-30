#include "Ship.h"
#include <stdexcept>

bool Ship::is_collision(int size, const Position& coord, Direction dir) noexcept {
    if (size < 1 || size > 4) return true;
    if (dir == Horizontal) {
        if (coord.get_col() + size - 1 > 10) return true;
    }
    else {
        if (coord.get_row() + size - 1 > 10) return true;
    }
    return false;
}

Ship::Ship(int size, const Position& coord, Direction direction) {
    if (is_collision(size, coord, direction)) {
        throw std::logic_error("Invalid input: incorrect ship parameters");
    }
    _size = size;
    _coord = coord;
    _direction = direction;
}

Ship::Ship(int size, const Position& coord) {
    if (is_collision(size, coord, Horizontal)) {
        throw std::logic_error("Invalid input: incorrect ship parameters");
    }
    _size = size;
    _coord = coord;
    _direction = Horizontal;
}

Ship::Ship(int size, char direction, int row, char col) {
    Direction dir;
    if (direction == 'H' || direction == 'h') {
        dir = Horizontal;
    }
    else if (direction == 'V' || direction == 'v') {
        dir = Vertical;
    }
    else {
        throw std::logic_error("Invalid input: incorrect ship parameters");
    }

    char upperCol = col;
    if (col >= 'a' && col <= 'z') {
        upperCol = static_cast<char>(col - 'a' + 'A');
    }
    if (upperCol < 'A' || upperCol > 'J') {
        throw std::logic_error("Invalid input: incorrect ship parameters");
    }
    int colNum = upperCol - 'A' + 1;

    if (row < 1 || row > 10) {
        throw std::logic_error("Invalid input: incorrect ship parameters");
    }

    Position coord(row, colNum);
    if (is_collision(size, coord, dir)) {
        throw std::logic_error("Invalid input: incorrect ship parameters");
    }
    _size = size;
    _coord = coord;
    _direction = dir;
}

int Ship::get_size() const noexcept {
    return _size;
}

Direction Ship::get_direction() const noexcept {
    return _direction;
}

int Ship::get_row() const noexcept {
    return _coord.get_row();
}

int Ship::get_col() const noexcept {
    return _coord.get_col();
}

void Ship::rotate() {
    Direction newDir = (_direction == Horizontal) ? Vertical : Horizontal;
    if (is_collision(_size, _coord, newDir)) {
        throw std::logic_error("Invalid input: incorrect ship parameters");
    }
    _direction = newDir;
}