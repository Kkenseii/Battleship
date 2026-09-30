#pragma once
#include "../GameFieldLib/gamefield.h"
#include <string>


enum State { Missed, BoatDestroyed, DestroyersDestroyed, CruisersDestroyed, BattleshipDestroyed, Hit };

class Ship; 

class Player {
    GameField _gamefield;
    int _ships_counts[4];

    static const int _max_ships_counts[4];

    static char num_to_col(int c) noexcept;
    bool is_ship_cell(int row, int col) const;

    int check_destroy_ship(int row, int col) const;

public:
    Player() noexcept;

    void set_ship(const Ship& ship);
    State set_action(int row, char col);
    void show_field(bool hide_ships = false) const;

    bool check_lose() const noexcept;
    bool check_ready() const noexcept;

};