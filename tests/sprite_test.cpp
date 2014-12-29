#include "../src/sdl_wrapper.hpp"
#include "../src/sprite.hpp"
#include <SDL2/SDL.h>

int main(void)
{
    SDL::init(800, 600, "Sprite Test");

    SDL::texture_sequence_handle ts = SDL::load_texture_sequence("../../motherload/assets/explosion", 10);

    AnimatedSprite s{ts, 4};
    AnimatedSprite s2{ts, 14};

    SDL_Event e;
    while (true) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
               goto done; 
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_DOWN:
                    s.scale(0.7);
                    s2.scale(0.9);
                    break;
                case SDLK_UP:
                    s.scale(1.3);
                    s2.scale(1.1);
                    break;
                case SDLK_LEFT:
                    s.rotate(-10.0);
                    s2.rotate(-5.0);
                    break;
                case SDLK_RIGHT:
                    s.rotate(10.0);
                    s2.rotate(5.0);
                    break;
                case SDLK_h:
                    s.flip_horizontally();
                    s2.flip_horizontally();
                    break;
                case SDLK_v:
                    s.flip_vertically();
                    s2.flip_vertically();
                    break;
                case SDLK_p:
                    if (s.playing()) {
                        s.pause();
                    } else {
                        s.play();
                    }
                    break;
                }
            }
        }

        SDL::render_clear();

        s.draw(200, 200);
        s2.draw(560, 360);

        SDL::render_present();
    }

done:
    SDL::quit();
    return 0;
}
