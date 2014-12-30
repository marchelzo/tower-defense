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
    coord home;
public:
    Map(const std::string& file_path);
    const Tile& operator()(int, int) const;
    int width() const;
    int height() const;
};
