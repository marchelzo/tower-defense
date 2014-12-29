#include "sdl_wrapper.hpp"

class Sprite {
    texture_handle t;
    double angle;
    SDL_RendererFlip flip;
    int w;
    int h;
public:
    Sprite(texture_handle);
    void draw(int x, int y);
    void rotate(double);
    void set_angle(double);
    double get_angle();
    void set_flip(SDL_RendererFlip);
    SDL_RendererFlip get_flip();
    void flip_horizontally();
    void flip_vertically();
    void set_width(int);
    void set_height(int);
    int get_height();
    int get_width();
    void scale(double);
};
