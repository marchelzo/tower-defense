#include "enemy.hpp"

static inline int round_to_closest(int k, int x)
{
    if (x % k > k / 2)
        return x + (k - x % k);
    else
        return x - (x % k);

}

Enemy::Enemy(Sprite sprite, int damage, int x, int y, int speed):
    sprite                {sprite},
    direction             {Direction::NONE},
    speed                 {speed},
    damage                {damage},
    x                     {x},
    y                     {y},
    moved                 {0},
    alive                 {true},
    must_change_direction {false}
{
}

int Enemy::get_x() const
{
    return x + sprite.get_width() / 2;
}

int Enemy::get_y() const
{
    return y + sprite.get_height() / 2;
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
        y += speed;
    else if (direction == Direction::UP)
        y -= speed;
    else if (direction == Direction::RIGHT)
        x += speed;
    else if (direction == Direction::LEFT)
        x -= speed;

    if (moved > 64)
        moved = 0;

    if (moved == 0) {
	m((y + sprite.get_height() / 2) / 64, (x + sprite.get_width() / 2) /64).affect(*this, m);
    }

    if (direction == Direction::UP || direction == Direction::DOWN)
        x = round_to_closest(64, x);
    else
        y = round_to_closest(64, y);

    moved += speed;
}

bool Enemy::is_alive() const
{
    return alive;
}

int Enemy::power() const
{
    return damage;
}

void Enemy::kill()
{
    alive = false;
}

void Enemy::force_next_turn()
{
    must_change_direction = true;
}

bool Enemy::must_turn() const
{
    return must_change_direction;
}
