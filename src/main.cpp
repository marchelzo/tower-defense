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

/* some constants */
static int constexpr MIN_WIN_WIDTH  = 10;
static int constexpr MIN_WIN_HEIGHT = 10;
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
    Map map {"./maps/one.map"};

    /* initialize the game camera */
    Camera::init(0, 0, map.width() * BLOCK_SIZE - 1, map.height() * BLOCK_SIZE - 1, SDL::WINDOW_WIDTH, SDL::WINDOW_HEIGHT);

    /* create an SDL_Event for polling events */
    SDL_Event e;

    Enemy enemy {Sprite{Textures::RED_SPHERE}, 40, Direction::RIGHT};

    /* game loop */
    while (true) {
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
        enemy.update(map);

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
                if (map(y + Camera::y / BLOCK_SIZE, x + Camera::x / BLOCK_SIZE).get_type() != TileType::GRASS)
                map(y + Camera::y / BLOCK_SIZE, x + Camera::x / BLOCK_SIZE).draw(BLOCK_SIZE * x - Camera::x % BLOCK_SIZE, BLOCK_SIZE * y - Camera::y % BLOCK_SIZE);
            }
        }

        enemy.draw(-Camera::x, -Camera::y);

        SDL::render_present();
    }


end: /* clean up and quit */
    SDL::quit();
    return 0;
}
