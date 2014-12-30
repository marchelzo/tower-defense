/*
 *
 * Tower Defense
 *
 */

#include <SDL2/SDL.h>
#include <cstdio>

#include "sdl_wrapper.hpp"
#include "sprite.hpp"

/* some constants */
static int constexpr MIN_WIN_WIDTH  = 10;
static int constexpr MIN_WIN_HEIGHT = 10;
static int constexpr MAX_WIN_WIDTH  = 100;
static int constexpr MAX_WIN_HEIGHT = 80;
static int constexpr BLOCK_SIZE     = 48;
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
        win_width  = 32;
        win_height = 20;
    } else {
        fprintf(stderr, "Invalid arguments\nUsage: %s\nor:    %s <width> <height>\n", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }

    /* width and height are valid, so we initialize SDL */
    SDL::init(BLOCK_SIZE * win_width, BLOCK_SIZE * win_height, GAME_TITLE);
    
    /* create an SDL_Event for polling events */
    SDL_Event e;

    /* game loop */
    while (true) {
        /* handle input */
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                goto end;
        }

        /* render output */
        SDL::render_clear();
        SDL::render_present();
    }


end: /* clean up and quit */
    SDL::quit();
    return 0;
}
