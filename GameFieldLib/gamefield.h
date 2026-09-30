#pragma once
#include <string>


class GameField {
    char** _field;
    const int _n;
    const int _m;

    void init_field();
    void copy_field(char* const* src);
    void free_field() noexcept;

    static void check_size(int n, int m);
    static int col_to_index(char col);

    void mark(int row, char col, char symbol);

    static std::string render(const GameField& field, bool hide_ships, bool use_hide);

    friend class Player;

public:
    GameField();
    GameField(char** field, int n, int m);
    GameField(int n, int m);
    GameField(const GameField& other);
    ~GameField() noexcept;

    void set(int row, char col);
    char get(int row, char col) const;

    friend std::string to_string(const GameField& field, bool hide_ships);

};