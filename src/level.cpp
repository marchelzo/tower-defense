#include "level.hpp"

Wave::Wave(int num_enemies, int num_per_spawn, SpawnFunction spawn,
       	   double time_between_enemies, double delay_after_wave):
	num_enemies	     {num_enemies},
	num_per_spawn	     {num_per_spawn},
	spawn		     {spawn},
	time_between_enemies {time_between_enemies},
	delay_after_wave     {delay_after_wave}
{
}


Level::Level(std::vector<Wave> waves):
    waves        {waves},
    current_wave {0},
    paused       {false}
{
}

void Level::spawn(std::vector<Enemy>& es)
{
    if (paused) return;

    ++frames;

    Wave& wave = waves[current_wave]; /* just an alias to save characters */

    if (wave.num_enemies == 0) {
	if (current_wave + 1 == waves.size())
	    return;
	if (60.0 * wave.delay_after_wave < frames) {
	    ++current_wave;
	    frames = 0;
	}
    } else if (frames > 60.0 * wave.time_between_enemies) {
	int to_spawn = std::min(wave.num_per_spawn, wave.num_enemies);
	for (int i = 0; i < to_spawn; ++i)
	    es.push_back(std::move(wave.spawn()));
	wave.num_enemies -= to_spawn;
	frames = 0;
    }
}

void Level::pause()
{
    paused = true;
}

void Level::unpause()
{
    paused = false;
}
