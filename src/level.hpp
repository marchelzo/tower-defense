class Level;
struct Wave;

#include <functional>

#include "enemy.hpp"

#pragma once

typedef std::function<Enemy()> SpawnFunction;

struct Wave {
    int num_enemies;
    int num_per_spawn;
    SpawnFunction spawn;
    double time_between_enemies;
    double delay_after_wave;
    Wave(int num_enemies,
	 int num_per_spawn,
	 SpawnFunction spawn,
	 double time_between_enemies,
	 double delay_after_wave);
};

class Level {
    std::vector<Wave> waves;
    size_t current_wave;
    int frames;
    int paused;
public:
    Level(std::vector<Wave> waves);
    void spawn(std::vector<Enemy>& es);
    void pause();
    void unpause();
};
