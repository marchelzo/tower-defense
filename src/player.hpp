class Player;

#pragma once

class Player {
    int hp;
    int funds;
public:
    Player(int hp, int funds);
    int get_funds() const;
    bool subtract_funds(int);
    void add_funds(int);
    int hp_remaining() const;
    void damage(int amount);
};
