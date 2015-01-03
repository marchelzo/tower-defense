class Tile;

#pragma once

#include <functional>

#include "sprite.hpp"
#include "enemy.hpp"
#include "map.hpp"

enum class TileType {
    GRASS,
    SAND,
    SPAWN,
    HOME
};

class Tile {
    const Sprite* sprite;
    const std::function<void(Enemy&, const Map&)> effect;
    const TileType type;
public:
    Tile(const Sprite* sprite, std::function<void(Enemy&, const Map&)> effect, TileType type);
    void draw(int x, int y) const;
    void draw(int x, int y, double scale) const;
    void affect(Enemy& e, const Map& m) const;
    TileType get_type() const;
    const Sprite* get_sprite() const;
    void set_sprite(const Sprite* s);
};
