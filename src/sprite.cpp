#include "sdl_wrapper.hpp"
#include "sprite.hpp"

Sprite::Sprite(texture_handle t):
    t {t},
    angle {0.0},
    flip {SDL_FLIP_NONE},
    w {SDL::texture_width(t)},
    h {SDL::texture_height(t)}
{ }

void Sprite::draw(int x, int y)
{
    SDL::render_texture(t, x, y, w, h, angle, flip);
}

void Sprite::rotate(double angle) { this->angle += angle; }
void Sprite::set_angle(double angle) { this->angle = angle; }
double Sprite::get_angle() { return angle; }

void Sprite::scale(double factor)
{
    w = (int) (double) w * factor;
    h = (int) (double) h * factor;
}
void Sprite::set_height(int height) { h = height; }
void Sprite::set_width(int width) { w = width; }
int Sprite::get_height() { return h; }
int Sprite::get_width() { return w; }

void Sprite::set_flip(SDL_RendererFlip f) { flip = f; }
SDL_RendererFlip Sprite::get_flip() { return flip; }
void Sprite::flip_horizontally() { flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_HORIZONTAL); }
void Sprite::flip_vertically() { flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_VERTICAL); }
