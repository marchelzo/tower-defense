#include "sdl_wrapper.hpp"

class Sprite {
    SDL::texture_handle t;
    double angle;
    SDL_RendererFlip flip;
    SDL_Rect rect;
public:
    Sprite(SDL::texture_handle);
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

class AnimatedSprite {
    SDL::Animation animation;
    double angle;
    SDL_RendererFlip flip;
    SDL_Rect rect;
public:
    AnimatedSprite(SDL::Animation);
    virtual void draw(int x, int y);
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
    void pause();
    void play();
    bool playing();
    void set_frame(size_t frame_number);
};
