#pragma once
#include "../PlayerLib/playerlib.h"
#include "../ShipLib/ship.h"
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>



class Game {
    Player _user;
    Player _computer;

    int _ai_move_count = 0;

    static void ai_target(int index, int& row, int& col) noexcept {
        if (index < 10) {
            row = index + 1;
            col = index + 1;
            return;
        }
        index -= 10;
        if (index < 10) {
            row = index + 1;
            col = 10 - index;
            return;
        }
        index -= 10;
        int count = 0;
        for (int r = 1; r <= 10; ++r) {
            for (int c = 1; c <= 10; ++c) {
                if (r == c) continue;
                if (c == 11 - r) continue;
                if (count == index) {
                    row = r;
                    col = c;
                    return;
                }
                ++count;
            }
        }
        row = 1;
        col = 1;
    }

    void user_init(std::string input);
    void computer_init(std::string input);

    State user_move(std::string input);
    State computer_move();

    bool is_end() const noexcept;
    void show_game_window() const;

    static bool continues(State state) noexcept {
        return state == State::Hit ||
            state == State::BoatDestroyed ||
            state == State::DestroyersDestroyed ||
            state == State::CruisersDestroyed ||
            state == State::BattleshipDestroyed;
    }

public:
    Game() = default;

    void start();
};

inline void Game::user_init(std::string input) {
    std::istringstream stream(input);
    std::string line;
    bool any = false;
    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        std::istringstream ls(line);
        int size;
        char dir;
        int row;
        char col;
        if (!(ls >> size >> dir >> row >> col)) {
            throw std::logic_error("Invalid input: incorrect field");
        }
        Ship ship(size, dir, row, col);
        _user.set_ship(ship);
        any = true;
    }
    if (!any || !_user.check_ready()) {
        throw std::logic_error("Invalid input: incorrect field");
    }
}

inline void Game::computer_init(std::string input) {
    std::istringstream stream(input);
    std::string line;
    bool any = false;
    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        std::istringstream ls(line);
        int size;
        char dir;
        int row;
        char col;
        if (!(ls >> size >> dir >> row >> col)) {
            throw std::logic_error("Invalid input: incorrect field");
        }
        Ship ship(size, dir, row, col);
        _computer.set_ship(ship);
        any = true;
    }
    if (!any || !_computer.check_ready()) {
        throw std::logic_error("Invalid input: incorrect field");
    }
}

inline State Game::user_move(std::string input) {
    std::istringstream iss(input);
    int row;
    char col;
    if (!(iss >> row >> col)) {
        throw std::logic_error("Invalid input: incorrect move");
    }
    return _computer.set_action(row, col);
}

inline State Game::computer_move() {
    if (_ai_move_count >= 100) {
        throw std::logic_error("Invalid input: incorrect move");
    }
    int row, colNum;
    ai_target(_ai_move_count, row, colNum);
    ++_ai_move_count;
    char col = static_cast<char>('A' + colNum - 1);
    return _user.set_action(row, col);
}

inline bool Game::is_end() const noexcept {
    return _user.check_lose() || _computer.check_lose();
}

inline void Game::show_game_window() const {
    std::cout << "= COMPUTER GAME FIELD =\n\n";
    _computer.show_field(true);
    std::cout << "\n=== YOUR PLAY FIELD ===\n\n";
    _user.show_field(false);
}

inline void Game::start() {
    std::string block, line;

    block.clear();
    while (std::getline(std::cin, line) && !line.empty()) {
        if (!block.empty()) block += "\n";
        block += line;
    }
    user_init(block);

    block.clear();
    while (std::getline(std::cin, line) && !line.empty()) {
        if (!block.empty()) block += "\n";
        block += line;
    }
    computer_init(block);

    show_game_window();

    while (!is_end()) {
        State userResult;
        do {
            if (!std::getline(std::cin, line)) {
                throw std::logic_error("Invalid input: incorrect move");
            }
            userResult = user_move(line);
        } while (!is_end() && continues(userResult));

        if (is_end()) break;

        State compResult;
        do {
            compResult = computer_move();
        } while (!is_end() && continues(compResult));
    }

    show_game_window();

    if (_computer.check_lose()) {
        std::cout << "USER WIN!" << std::endl;
    }
    else {
        std::cout << "COMPUTER WIN!" << std::endl;
    }
}