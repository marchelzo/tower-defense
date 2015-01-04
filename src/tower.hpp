#include <memory>
#include <functional>

#include "sprite.hpp"
#include "enemy.hpp"
#include "projectile.hpp"

class Tower;

#pragma once

typedef std::function<void(Tower&, const std::vector<Enemy>&,
                       std::vector<Projectile>&)> TowerUpdate;

class Tower {
    const Sprite& base_sprite; /* the part of the tower that doesn't move */
    Sprite sprite;             /* the part which rotates, and must be unique to this tower */
    Sprite projectile_sprite;
    std::function<void(Enemy&)> effect;
    double projectile_speed;
    int fire_rate;
    TowerUpdate update_fn;
    int x;
    int y;
public:
    Tower(const Sprite& base_sprite, Sprite sprite, Sprite ps,
          std::function<void(Enemy&)> effect,
          double projectile_speed,
          double fire_rate,
          TowerUpdate uf,
          int x, int y);
    void draw() const;
    void update(const std::vector<Enemy>& es, std::vector<Projectile>& ps);
    void shoot(double dx, double dy, std::vector<Projectile>& ps);
    int get_x() const;
    int get_y() const;
    int firing_rate() const;
};
