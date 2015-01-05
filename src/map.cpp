#include <vector>
#include <fstream>
#include <cstdio>
#include <memory>
#include <algorithm>

#include "map.hpp"
#include "tile.hpp"
#include "sprite.hpp"
#include "textures.hpp"
#include "enemy.hpp"
#include "player.hpp"
#include "game.hpp"

static bool sprites_loaded;

/* sprites that we will use */
static std::unique_ptr<Sprite> grass                     { };
static std::unique_ptr<Sprite> sand_h                    { };
static std::unique_ptr<Sprite> sand_v                    { };
static std::unique_ptr<Sprite> sand_corner_top_left      { };
static std::unique_ptr<Sprite> sand_corner_bottom_left   { };
static std::unique_ptr<Sprite> sand_corner_top_right     { };
static std::unique_ptr<Sprite> sand_corner_bottom_right  { };
static std::unique_ptr<Sprite> sand_3way_up              { };
static std::unique_ptr<Sprite> sand_3way_down            { };
static std::unique_ptr<Sprite> sand_3way_right           { };
static std::unique_ptr<Sprite> sand_3way_left            { };
static std::unique_ptr<Sprite> sand_4way                 { };
static std::unique_ptr<Sprite> home                      { };
static std::unique_ptr<Sprite> spawn                     { };

static void should_not_happen(Enemy& e, const Map& m)
{
    fputs("An enemy is on the grass\n"
          "This should not happen!!!\n", stderr);
}

static void no_op(Enemy& e, const Map& m) {}

static void load_map_sprites()
{
    grass  = std::make_unique<Sprite>(Textures::GRASS_TEXTURE);
    sand_h = std::make_unique<Sprite>(Textures::SAND_TEXTURE);
    sand_v = std::make_unique<Sprite>(Textures::SAND_TEXTURE);
    sand_v->rotate(90);
    sand_corner_top_right = std::make_unique<Sprite>(Textures::SAND_CORNER_TEXTURE);
    sand_corner_top_left = std::make_unique<Sprite>(Textures::SAND_CORNER_TEXTURE);
    sand_corner_top_left->rotate(270);
    sand_corner_bottom_right = std::make_unique<Sprite>(Textures::SAND_CORNER_TEXTURE);
    sand_corner_bottom_right->rotate(90);
    sand_corner_bottom_left = std::make_unique<Sprite>(Textures::SAND_CORNER_TEXTURE);
    sand_corner_bottom_left->rotate(180);
    sand_4way = std::make_unique<Sprite>(Textures::SAND_4WAY_TEXTURE);
    sand_3way_down  = std::make_unique<Sprite>(Textures::SAND_3WAY_TEXTURE);
    sand_3way_up    = std::make_unique<Sprite>(Textures::SAND_3WAY_TEXTURE);
    sand_3way_up->rotate(180);
    sand_3way_right = std::make_unique<Sprite>(Textures::SAND_3WAY_TEXTURE);
    sand_3way_right->rotate(270);
    sand_3way_left  = std::make_unique<Sprite>(Textures::SAND_3WAY_TEXTURE);
    sand_3way_left->rotate(90);
    home = std::make_unique<Sprite>(Textures::HOME_TEXTURE);
    spawn = std::make_unique<Sprite>(Textures::SPAWN_TEXTURE);
}

static void spawned(Enemy& e, const Map& m)
{
    int x = e.get_x() / 48;
    int y = e.get_y() / 48;

    bool below = y + 1 != m.height() && m(y+1,x).get_type() != TileType::GRASS;
    bool above = y > 0               && m(y-1,x).get_type() != TileType::GRASS;
    bool right = x + 1 != m.width()  && m(y,x+1).get_type() != TileType::GRASS;
    bool left  = x > 0               && m(y,x-1).get_type() != TileType::GRASS;

    if (left) {
        e.set_direction(Direction::LEFT);
        return;
    }

    if (right) {
        e.set_direction(Direction::RIGHT);
        return;
    }

    if (above) {
        e.set_direction(Direction::UP);
        return;
    }

    if (below) {
        e.set_direction(Direction::DOWN);
        return;
    }

    fputs("Error: Invalid spawn position.\n", stderr);

}

static void turn_left(Enemy& e, const Map& m)
{
    e.set_direction(Direction::LEFT);
}

static void turn_right(Enemy& e, const Map& m)
{
    e.set_direction(Direction::RIGHT);
}

static void turn_up(Enemy& e, const Map& m)
{
    e.set_direction(Direction::UP);
}

static void turn_down(Enemy& e, const Map& m)
{
    e.set_direction(Direction::DOWN);
}

static void turn_random(Enemy& e, const Map& m)
{
    int x = e.get_x() / 48;
    int y = e.get_y() / 48;

    std::vector<Direction> ds;

    bool below = y + 1 != m.height() && m(y+1,x).get_type() != TileType::GRASS;
    bool above = y > 0               && m(y-1,x).get_type() != TileType::GRASS;
    bool right = x + 1 != m.width()  && m(y,x+1).get_type() != TileType::GRASS;
    bool left  = x > 0               && m(y,x-1).get_type() != TileType::GRASS;

    if (below && e.get_direction() != Direction::UP) ds.push_back(Direction::DOWN);
    if (above && e.get_direction() != Direction::DOWN) ds.push_back(Direction::UP);
    if (right && e.get_direction() != Direction::LEFT) ds.push_back(Direction::RIGHT);
    if (left  && e.get_direction() != Direction::RIGHT) ds.push_back(Direction::LEFT);

    if (e.must_turn()) {
        auto pos = std::find(ds.begin(), ds.end(), e.get_direction());
        if (pos != ds.end())
            ds.erase(pos);
    }

    if (ds.size() == 0) {
        e.turn_around();
    } else {
        e.set_direction(ds[rand() % ds.size()]);
    }

}

static void inflict_damage(Enemy& e, const Map& m)
{
    Player::hp -= e.power();
    e.kill();
}

static void force_next_turn(Enemy& e, const Map& m)
{
    e.force_next_turn();
}

Map::Map(const std::string& file_path):
    tiles      {},
    spawns     {},
    home_coord {}
{
    if (!sprites_loaded)
        load_map_sprites();

    std::ifstream in {file_path};
    std::string line;

    /* consume the top border */
    std::getline(in, line);
    
    for (int y = 0; std::getline(in, line); ++y) {
        tiles.emplace_back();
        int x = 0;
        for (char c : line) {
            switch (c) {
            case '#':
                continue;
            case ' ':
                tiles[y].emplace_back(grass.get(), should_not_happen, TileType::GRASS);
                break;
            case '=':
                tiles[y].emplace_back(sand_h.get(), no_op, TileType::SAND);
                break;
            case '|':
                tiles[y].emplace_back(sand_v.get(), no_op, TileType::SAND);
                break;
            case '^':
                tiles[y].emplace_back(nullptr, turn_up, TileType::SAND);
                break;
            case 'v':
                tiles[y].emplace_back(nullptr, turn_down, TileType::SAND);
                break;
            case '>':
                tiles[y].emplace_back(nullptr, turn_right, TileType::SAND);
                break;
            case '<':
                tiles[y].emplace_back(nullptr, turn_left, TileType::SAND);
                break;
            case '@':
                tiles[y].emplace_back(spawn.get(), spawned, TileType::SPAWN);
		spawns.push_back({ .x = x, .y = y });
                break;
            case '*':
                tiles[y].emplace_back(home.get(), inflict_damage, TileType::HOME);
                home_coord.x = x;
                home_coord.y = y;
                break;
            case '%':
                tiles[y].emplace_back(nullptr, turn_random, TileType::SAND);
                break;
            case '!':
                tiles[y].emplace_back(nullptr, force_next_turn, TileType::SAND);
                break;
            }
            ++x;
        }
    }

    /* remove the last vector, because it is empty (bottom border) and will result
     * in Map::height() returning the wrong value
     */
    tiles.erase(tiles.end() - 1);

    /* insert sprites for all of the corners / intersections */
    for (unsigned int y = 0; y < tiles.size(); ++y) {
        for (unsigned int x = 0; x < tiles[y].size(); ++x) {
            if (tiles[y][x].get_sprite()) continue;

            bool below = y + 1 != tiles.size()    && tiles[y+1][x].get_type() != TileType::GRASS;
            bool above = y > 0                    && tiles[y-1][x].get_type() != TileType::GRASS;
            bool right = x + 1 != tiles[y].size() && tiles[y][x+1].get_type() != TileType::GRASS;
            bool left  = x > 0                    && tiles[y][x-1].get_type() != TileType::GRASS;

            if (below) {
                if (right) {
                    if (above) {
                        if (left) tiles[y][x].set_sprite(sand_4way.get());
                        else tiles[y][x].set_sprite(sand_3way_right.get());
                    } else if (left)
                        tiles[y][x].set_sprite(sand_3way_down.get());
                      else tiles[y][x].set_sprite(sand_corner_top_left.get());
                } else if (above) {
                    if (left)
                        tiles[y][x].set_sprite(sand_3way_left.get());
                    else tiles[y][x].set_sprite(sand_v.get());
                } else tiles[y][x].set_sprite(sand_corner_top_right.get());
            } else {
                if (right) {
                    if (left) {
                        if (above) {
                            tiles[y][x].set_sprite(sand_3way_up.get());
                        } else {
                            tiles[y][x].set_sprite(sand_h.get());
                        }
                    } else {
                        tiles[y][x].set_sprite(sand_corner_bottom_left.get());
                    }
                } else {
                    tiles[y][x].set_sprite(sand_corner_bottom_right.get());
                }
            }
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

std::function<Enemy()> Map::make_enemy_spawner(const Sprite& sprite, int damage, int speed, int hp)
{
    const std::vector<coord>& spawns_ref = spawns;

    return [=, &spawns_ref](){
	coord s = spawns[rand() % spawns.size()];
	return Enemy(sprite, damage, s.x * Game::BLOCK_SIZE, s.y * Game::BLOCK_SIZE, speed, hp);
    };
}
