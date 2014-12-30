#include <vector>
#include <fstream>
#include <cstdio>

#include "map.hpp"
#include "tile.hpp"
#include "sprite.hpp"
#include "textures.hpp"

static void should_not_happen(Enemy& e, const Map& m)
{
    fputs("An enemy is on the grass\n"
          "This should not happen!!!", stderr);
}

static void no_op(Enemy& e, const Map& m) {}

static void spawn(Enemy& e, const Map& m)
{
    int x = e.get_x() / 48;
    int y = e.get_y() / 48;

    if (x == 0) {
        if (y == 0) {
            if (m(x,y+1).get_type() != TileType::GRASS)
                e.set_direction(Direction::DOWN);
            else
                e.set_direction(Direction::RIGHT);
        } else {
            if (m(x,y+1).get_type() != TileType::GRASS)
                e.set_direction(Direction::DOWN);
            else if (m(x,y-1).get_type() != TileType::GRASS)
                e.set_direction(Direction::UP);
            else
                e.set_direction(Direction::RIGHT);
        }
    } else {
        if (y + 1 == m.height()) {
            if (m(x,y-1).get_type() != TileType::GRASS)
                e.set_direction(Direction::UP);
            else
                e.set_direction(Direction::LEFT);
        } else {
            if (m(x,y-1).get_type() != TileType::GRASS)
                e.set_direction(Direction::UP);
            else if (m(x,y+1).get_type() != TileType::GRASS)
                e.set_direction(Direction::DOWN);
            else
                e.set_direction(Direction::LEFT);
        }
    }
}

Map::Map(const std::string& file_path)
{
    std::ifstream in {file_path};
    std::string line;

    /* sprites that we will use */
    Sprite grass {Textures::GRASS_TEXTURE};
    Sprite sand_h  {Textures::SAND_TEXTURE};
    Sprite sand_v  {Textures::SAND_TEXTURE};
    sand_v.rotate(90);
    
    for (int y = 0; std::getline(in, line); ++y) {
        tiles.emplace_back();
        int x = 0;
        for (char c : line) {
            switch (c) {
            case '#':
                continue;
            case ' ':
                tiles[y].emplace_back(grass, should_not_happen, TileType::GRASS);
                break;
            case '=':
                tiles[y].emplace_back(sand_h, no_op, TileType::SAND);
                break;
            case '|':
                tiles[y].emplace_back(sand_v, no_op, TileType::SAND);
                break;
            case '^':
                tiles[y].emplace_back(sand_v, no_op, TileType::SAND);
                break;
            case 'v':
                tiles[y].emplace_back(sand_v, no_op, TileType::SAND);
                break;
            case '>':
                tiles[y].emplace_back(sand_v, no_op, TileType::SAND);
                break;
            case '<':
                tiles[y].emplace_back(sand_v, no_op, TileType::SAND);
                break;
            case '@':
                tiles[y].emplace_back(sand_v, spawn, TileType::SPAWN);
                break;
            case '*':
                tiles[y].emplace_back(sand_v, no_op, TileType::HOME);
                break;
            }
            ++x;
        }
    }
}

const Tile& Map::operator()(int x, int y) const
{
    return tiles[x][y];
}

int Map::width() const
{
    return tiles[1].size();
}

int Map::height() const
{
    return tiles.size();
}
