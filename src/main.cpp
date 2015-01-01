/*
 *
 * Tower Defense
 *
 */

#include <SDL2/SDL.h>
#include <cstdio>
#include <ctime>

#include "sdl_wrapper.hpp"
#include "sprite.hpp"
#include "camera.hpp"
#include "textures.hpp"
#include "map.hpp"
#include "player.hpp"

/* some constants */
static int constexpr MIN_WIN_WIDTH  = 8;
static int constexpr MIN_WIN_HEIGHT = 5;
static int constexpr MAX_WIN_WIDTH  = 100;
static int constexpr MAX_WIN_HEIGHT = 80;
static int constexpr BLOCK_SIZE     = 64;
static std::string const GAME_TITLE {"Tower Defense!"};



int main(int argc, char *argv[])
{
    /* parse the command line arguments to determine the window dimensions */
    int win_width, win_height; /* in blocks, not pixels */
    if (argc == 3) {
        win_width  = std::stoi(argv[1]);
        win_height = std::stoi(argv[2]);
        if (win_width < MIN_WIN_WIDTH || win_width > MAX_WIN_WIDTH || win_height < MIN_WIN_HEIGHT || win_height > MAX_WIN_HEIGHT) {
            fprintf(stderr, "Invalid dimensions: %d x %d\nPlease specify a width in [%d,%d] and a height in [%d,%d]\n", win_width, win_height,
                                                                                                                        MIN_WIN_WIDTH, MAX_WIN_WIDTH,
                                                                                                                        MIN_WIN_HEIGHT, MAX_WIN_HEIGHT);
            exit(EXIT_FAILURE);
        }
    } else if (argc == 1) {
        win_width  = 18;
        win_height = 14;
    } else {
        fprintf(stderr, "Invalid arguments\nUsage: %s\nor:    %s <width> <height>\n", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }

    /* seed the random number generator */
    srand(time(nullptr));

    /* width and height are valid, so we initialize SDL */
    SDL::init(BLOCK_SIZE * win_width, BLOCK_SIZE * win_height, GAME_TITLE);

    /* load assets */
    Textures::load();

    /* load the map */
    Map map {"./maps/two.map"};

    /* make sure the window is not bigger than the actual map */
    if (win_width > map.width()) {
        fprintf(stderr, "Note: Specified width was bigger than the map size. Changing width to %d from %d.\n", map.width(), win_width);
        win_width = map.width();
    }
    if (win_height > map.height()) {
        fprintf(stderr, "Note: Specified height was bigger than the map size. Changing height to %d from %d.\n", map.height(), win_height);
        win_height = map.height();
    }

    SDL::set_window_size(win_width * 64, win_height * 64);

    /* initialize the game camera */
    Camera::init(0, 0, map.width() * BLOCK_SIZE - 1, map.height() * BLOCK_SIZE - 1, SDL::WINDOW_WIDTH, SDL::WINDOW_HEIGHT);

    Player::init(1000000, 1000);

    std::vector<Enemy> es;
    map.spawn_enemies(100, es);

    /* create an SDL_Event for polling events */
    SDL_Event e;

    /* game loop */
    while (Player::hp > 0) {
        /* handle input */
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                goto end;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_UP:
                    break;
                case SDLK_h:
                    Camera::center_on(0,0);
                    break;
                }
            }
        }

        /* update enemies */
	for (auto& e : es)
            if (e.is_alive())
                e.update(map);

        Camera::update();

        /* render output */
        SDL::render_clear();

        /* fill with grass first */
        for (int x = 0; x < win_width + 1; ++x)
            for (int y = 0; y < win_height + 1; ++y)
                SDL::render_texture(Textures::GRASS_TEXTURE, x * BLOCK_SIZE - Camera::x % BLOCK_SIZE,
                                        y * BLOCK_SIZE - Camera::y % BLOCK_SIZE);

        /* draw the dirt */
        for (int x = 0; x < win_width + 1; ++x) {
            for (int y = 0; y < win_height + 1; ++y) {
                if (y == map.height() || x == map.width()) continue;
                if (map(y + Camera::y / BLOCK_SIZE, x + Camera::x / BLOCK_SIZE).get_type() != TileType::GRASS)
                map(y + Camera::y / BLOCK_SIZE, x + Camera::x / BLOCK_SIZE).draw(BLOCK_SIZE * x - Camera::x % BLOCK_SIZE, BLOCK_SIZE * y - Camera::y % BLOCK_SIZE);
            }
        }

	for (auto& e : es)
            if (e.is_alive())
                e.draw(-Camera::x, -Camera::y);

        SDL::render_present();
    }


end: /* clean up and quit */
    SDL::quit();
    return 0;
}
