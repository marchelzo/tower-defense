#include <functional>

#include "tile.hpp"
#include "enemy.hpp"
#include "sprite.hpp"
#include "map.hpp"


Tile::Tile(const Sprite* sprite, std::function<void(Enemy&, const Map&)> effect, TileType type):
    sprite {sprite},
    effect {effect},
    type   {type}
{ }

void Tile::draw(int x, int y) const
{
    sprite->draw(x,y);
}

void Tile::affect(Enemy& e, const Map& m) const
{
    effect(e, m);
}

TileType Tile::get_type() const
{
    return type;
}

const Sprite* Tile::get_sprite() const
{
    return sprite;
}

void Tile::set_sprite(const Sprite* s)
{
    sprite = s;
}
