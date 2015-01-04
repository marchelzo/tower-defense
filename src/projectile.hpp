#include <functional>

#include "sprite.hpp"
#include "enemy.hpp"

class Projectile;
#pragma once

typedef std::function<void(Enemy&)> Effect;

class Projectile {
    const Sprite& sprite;
    Effect effect;
    double x, y;
    double vx, vy;
public:
    Projectile(const Sprite& sprite,
               Effect effect, double x, double y,
               double vx, double vy);
    void affect(Enemy& e) const;
    void update();
    void draw() const;
    int get_x() const;
    int get_y() const;
};
