#include "enemy.hpp"

Enemy::Enemy(Sprite sprite, int damage, Direction direction):
    sprite    {sprite},
    direction {direction},
    damage    {damage},
    x         {0},
    y         {0}
{
}

int Enemy::get_x() const
{
    return (x + sprite.get_width() / 2) / 48;
}

int Enemy::get_y() const
{
    return (y + sprite.get_height() / 2) / 48;
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

    m(y/64,x/64).affect(*this, m);
}
