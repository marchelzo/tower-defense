class Enemy;

#pragma once

#include "sprite.hpp"
#include "map.hpp"

enum class Direction {
    RIGHT, LEFT, UP, DOWN
};

class Enemy {
    Sprite sprite;
    Direction direction;
    const int damage;
    int x;
    int y;
    int moved;
public:
    Enemy(Sprite sprite, int damage, int x, int y);
    void set_direction(Direction);
    void turn_around();
    Direction get_direction() const;
    int get_x() const;
    int get_y() const;
    void draw(int, int) const;
    void update(const Map& m);
};
