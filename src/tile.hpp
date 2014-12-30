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
    const Sprite& sprite;
    const std::function<void(Enemy&, const Map&)> effect;
    const TileType type;
public:
    Tile(const Sprite& sprite, std::function<void(Enemy&, const Map&)> effect, TileType type);
    void draw(int x, int y);
    void affect(Enemy& e, Map& m);
    TileType get_type() const;
};
