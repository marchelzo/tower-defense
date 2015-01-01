#include "player.hpp"

int Player::hp;
int Player::funds;

void Player::init(int hp, int funds)
{
    Player::hp = hp;
    Player::funds = funds;
}
