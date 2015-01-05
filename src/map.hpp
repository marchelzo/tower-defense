class Map;

#pragma once

#include <vector>

#include "tile.hpp"

class Map {
    struct coord {
        int x;
        int y;
    };
    std::vector<std::vector<Tile>> tiles;
    std::vector<coord> spawns;
    coord home_coord;
public:
    Map(const std::string& file_path);
    const Tile& operator()(int, int) const;
    int width() const;
    int height() const;
    std::function<Enemy()> make_enemy_spawner(const Sprite& sprite, int damage, int speed, int hp);
};
