#pragma once
#include "../PlayerLib/playerlib.h"
#include <string>


class Game {
    Player _user;
    Player _computer;

    int _ai_move_count = 0;

    static void ai_target(int index, int& row, int& col) noexcept;
    static bool continues(State state) noexcept;

    void user_init(std::string input);
    void computer_init(std::string input);

    State user_move(std::string input);
    State computer_move();

    bool is_end() const noexcept;
    void show_game_window() const;

public:
    Game() = default;

    void start();
};