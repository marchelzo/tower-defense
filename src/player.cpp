#include "player.hpp"

Player::Player(int hp, int funds):
    hp    {hp},
    funds {funds}
{ }

int Player::get_funds() const
{
    return funds;
}

bool Player::subtract_funds(int amount)
{ 
    if (funds >= amount) {
        funds -= amount;
        return true;
    } else return false;
}

void Player::add_funds(int amount)
{
    funds += amount;
}

int Player::hp_remaining() const
{
    return hp;
}

void Player::damage(int amount)
{
    hp -= amount;
}
