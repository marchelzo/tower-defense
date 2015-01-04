#include <functional>

#include "projectile.hpp"
#include "camera.hpp"

Projectile::Projectile(const Sprite& sprite, Effect effect,
                       double x, double y, double vx, double vy):
    sprite {sprite},
    effect {effect},
    x      {x},
    y      {y},
    vx     {vx},
    vy     {vy}
{
}

void Projectile::affect(Enemy& e) const
{
    effect(e);
}

void Projectile::update()
{
    x = x + vx;
    y = y + vy;
}

void Projectile::draw() const
{
    sprite.draw(Camera::zoom_amount() * x - Camera::x,
                Camera::zoom_amount() * y - Camera::y,
                Camera::zoom_amount());
}

int Projectile::get_x() const
{
    return x + 5;
}

int Projectile::get_y() const
{
    return y + 5;
}
