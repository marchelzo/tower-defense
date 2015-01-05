#include "level.hpp"

Wave::Wave(int num_enemies, SpawnFunction spawn,
       	   double time_between_enemies, double delay_after_wave):
	num_enemies	     {num_enemies},
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

    if (waves[current_wave].num_enemies == 0) {
	if (current_wave + 1 == waves.size())
	    return;
	if (60.0 * waves[current_wave].delay_after_wave < frames) {
	    ++current_wave;
	    frames = 0;
	}
    } else if (frames > 60.0 * waves[current_wave].time_between_enemies) {
	es.push_back(std::move(waves[current_wave].spawn()));
	--waves[current_wave].num_enemies;
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
