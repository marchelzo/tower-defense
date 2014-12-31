#include "enemy.hpp"

Enemy::Enemy(Sprite sprite, int damage, int x, int y):
    sprite    {sprite},
    damage    {damage},
    x         {x},
    y         {y},
    moved     {0}
{
}

int Enemy::get_x() const
{
    return x;
}

int Enemy::get_y() const
{
    return y;
}

void Enemy::set_direction(Direction d)
{
    direction = d;
}

Direction Enemy::get_direction() const
{
    return direction;
}

void Enemy::turn_around()
{
    if (direction == Direction::UP)
        direction = Direction::DOWN;
    else if (direction == Direction::DOWN)
        direction = Direction::UP;
    else if (direction == Direction::RIGHT)
        direction = Direction::LEFT;
    else if (direction == Direction::LEFT)
        direction = Direction::RIGHT;
}

void Enemy::draw(int x_off, int y_off) const
{
    sprite.draw(x + x_off, y + y_off);
}

void Enemy::update(const Map& m)
{
    if (direction == Direction::DOWN)
        y += 5;
    else if (direction == Direction::UP)
        y -= 5;
    else if (direction == Direction::RIGHT)
        x += 5;
    else if (direction == Direction::LEFT)
        x -= 5;

    if (moved % 65 == 0)
	m(y/64,x/64).affect(*this, m);
    moved += 5;
}
