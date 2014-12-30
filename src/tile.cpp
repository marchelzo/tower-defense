#include <functional>

#include "tile.hpp"
#include "enemy.hpp"
#include "sprite.hpp"
#include "map.hpp"


Tile::Tile(const Sprite& sprite, std::function<void(Enemy&, const Map&)> effect, TileType type):
    sprite {sprite},
    effect {effect},
    type   {type}
{ }

void Tile::draw(int x, int y)
{
    sprite.draw(x,y);
}

void Tile::affect(Enemy& e, Map& m)
{
    effect(e, m);
}

TileType Tile::get_type() const
{
    return type;
}
