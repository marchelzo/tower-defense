class Enemy;

#pragma once

#include "sprite.hpp"
#include "map.hpp"

enum class Direction {
    RIGHT, LEFT, UP, DOWN, NONE
};

class Enemy {
    Sprite sprite;
    Direction direction;
    int hp;
    int speed;
    const int damage;
    int x;
    int y;
    int moved;
    bool alive;
    bool must_change_direction;
public:
    Enemy(Sprite sprite, int damage, int x, int y, int speed);
    void set_direction(Direction);
    void turn_around();
    Direction get_direction() const;
    int get_x() const;
    int get_y() const;
    void draw(int, int) const;
    void draw(int, int, double scale) const;
    void draw() const;
    void update(const Map& m);
    bool is_alive() const;
    int power() const;
    void kill();
    void force_next_turn();
    bool must_turn() const;
};
