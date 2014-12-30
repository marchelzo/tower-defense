#include "sdl_wrapper.hpp"
#include "sprite.hpp"

Sprite::Sprite(SDL::texture_handle t):
    t {t},
    angle {0.0},
    flip {SDL_FLIP_NONE},
    rect {0, 0, SDL::texture_width(t), SDL::texture_height(t)}
{ }

void Sprite::draw(int x, int y) const
{
    rect.x = x;
    rect.y = y;
    SDL::render_texture(t, &rect, angle, flip);
}

void Sprite::rotate(double angle) { this->angle += angle; }
void Sprite::set_angle(double angle) { this->angle = angle; }
double Sprite::get_angle() { return angle; }

void Sprite::scale(double factor)
{
    rect.w = (int) (double) rect.w * factor;
    rect.h = (int) (double) rect.h * factor;
}
void Sprite::set_height(int height) { rect.h = height; }
void Sprite::set_width(int width) { rect.w = width; }
int Sprite::get_height() const { return rect.h; }
int Sprite::get_width() const { return rect.w; }

void Sprite::set_flip(SDL_RendererFlip f) { flip = f; }
SDL_RendererFlip Sprite::get_flip() { return flip; }
void Sprite::flip_horizontally() { flip = static_cast<SDL_RendererFlip>(flip ^ SDL_FLIP_HORIZONTAL); }
void Sprite::flip_vertically() { flip = static_cast<SDL_RendererFlip>(flip ^ SDL_FLIP_VERTICAL); }


AnimatedSprite::AnimatedSprite(SDL::Animation animation):
    animation {animation},
    angle {0.0},
    flip {SDL_FLIP_NONE},
    rect {0, 0, animation.width(), animation.height()}
{ }

AnimatedSprite::AnimatedSprite(SDL::texture_sequence_handle ts, size_t frame_duration):
    AnimatedSprite {SDL::Animation {ts, frame_duration}}
{ }

void AnimatedSprite::draw(int x, int y)
{
    rect.x = x;
    rect.y = y;
    animation.render(&rect, angle, flip);
}

void AnimatedSprite::rotate(double angle) { this->angle += angle; }
void AnimatedSprite::set_angle(double angle) { this->angle = angle; }
double AnimatedSprite::get_angle() { return angle; }

void AnimatedSprite::scale(double factor)
{ rect.w = (int) (double) rect.w * factor;
    rect.h = (int) (double) rect.h * factor;
}
void AnimatedSprite::set_height(int height) { rect.h = height; }
void AnimatedSprite::set_width(int width) { rect.w = width; }
int AnimatedSprite::get_height() { return rect.h; }
int AnimatedSprite::get_width() { return rect.w; }

void AnimatedSprite::set_flip(SDL_RendererFlip f) { flip = f; }
SDL_RendererFlip AnimatedSprite::get_flip() { return flip; }
void AnimatedSprite::flip_horizontally() { flip = static_cast<SDL_RendererFlip>(flip ^ SDL_FLIP_HORIZONTAL); }
void AnimatedSprite::flip_vertically() { flip = static_cast<SDL_RendererFlip>(flip ^ SDL_FLIP_VERTICAL); }
void AnimatedSprite::set_frame(size_t frame_number) { animation.set_frame(frame_number); }
bool AnimatedSprite::playing() { return animation.playing(); }
void AnimatedSprite::pause() { animation.pause(); }
void AnimatedSprite::play() { animation.play(); }
