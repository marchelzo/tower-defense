#pragma once

#include "sprite.hpp"

enum class Direction {
    RIGHT, LEFT, UP, DOWN
};

class Enemy {
    Sprite sprite;
    Direction direction;
    const int damage;
    int x;
    int y;
public:
    Enemy(Sprite sprite, int damage, Direction direction);
    void set_direction(Direction);
    int get_x() const;
    int get_y() const;
};
