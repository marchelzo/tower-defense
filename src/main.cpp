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

    SDL::texture_handle grass_texture = SDL::load_texture("./assets/grass_tile.png");
    SDL::texture_handle sand_path_texture = SDL::load_texture("./assets/sand_path_tile.png");
    Sprite grass {grass_texture};
    Sprite sand_path {sand_path_texture};

    /* game loop */
    while (true) {
        /* handle input */
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                goto end;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                    grass.flip_horizontally();
                    break;
                case SDLK_DOWN:
                    grass.flip_vertically();
                    break;
                }
            }
        }

        /* render output */
        SDL::render_clear();

        /* draw some grass */
        for (int x = 0; x < win_width; ++x)
            for (int y = 0; y < win_height; ++y)
                grass.draw(BLOCK_SIZE * x, BLOCK_SIZE * y);

        /* draw the path */
        sand_path.set_angle(0);
        for (int x = 0; x < win_width / 2; ++x)
            sand_path.draw(sand_path.get_width() * x, 6 * BLOCK_SIZE);
        sand_path.set_angle(90);
        for (int y = 6; y < win_height; ++y)
            sand_path.draw(win_width / 2 * BLOCK_SIZE, y * sand_path.get_width() - 415);

        SDL::render_present();
    }


end: /* clean up and quit */
    SDL::quit();
    return 0;
}
