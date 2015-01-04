#include <vector>
#include <cmath>

#include "tower.hpp"
#include "camera.hpp"
#include "sprite.hpp"
#include "game.hpp"

template<typename T>
static inline int signum(T x) { return x < 0 ? -1 : x != 0; }

#define PI 3.14159265358979323846
static inline double get_angle(double dx, double dy)
{
    double a = std::atan(std::abs(dy/dx)) * 180.0 / PI;
    if (dx <= 0 && dy >= 0) return -a + 90;
    if (dx >= 0 && dy >= 0) return -(180 - a) + 90;
    if (dx >= 0 && dy <= 0) return -(180 + a) + 90;
    return a + 90;
}

Tower::Tower(const Sprite& base_sprite, Sprite sprite, Sprite projectile_sprite,
      std::function<void(Enemy&)> effect,
      double projectile_speed,
      double fire_rate,
      TowerUpdate update_fn,
      int x, int y):
    base_sprite       {base_sprite},
    sprite            {sprite},
    projectile_sprite {projectile_sprite},
    effect            {effect},
    projectile_speed  {projectile_speed},
    fire_rate         {(int) (60.0 / fire_rate)},
    update_fn         {update_fn},
    x                 {x},
    y                 {y}
{
}

void Tower::draw() const
{
    int block_size = Camera::zoom_amount() * Game::BLOCK_SIZE;
    int x = this->x - Camera::x / block_size;
    int y = this->y - Camera::y / block_size;
    base_sprite.draw(x * block_size - Camera::x % block_size,
                     y * block_size - Camera::y % block_size,
                     Camera::zoom_amount());
    sprite.draw(x * block_size - Camera::x % block_size,
                y * block_size - Camera::y % block_size,
                Camera::zoom_amount());
}

void Tower::update(const std::vector<Enemy>& es, std::vector<Projectile>& ps)
{
    update_fn(*this, es, ps);
}

void Tower::shoot(double dx, double dy, std::vector<Projectile>& ps)
{
    double k = dy / dx;
    double vx = -sqrt(projectile_speed * projectile_speed / (k * k + 1)) * signum(dx);
    double vy = k * vx;

    /* rotate the tower to point at its target */
    sprite.set_angle(get_angle(dx,dy));

    if (SDL::frame_count % fire_rate) return;

    ps.emplace_back(projectile_sprite, effect, (double) this->get_x(), (double) this->get_y(), vx, vy);
}

int Tower::get_x() const
{
    return x * Game::BLOCK_SIZE + Game::BLOCK_SIZE / 2;
}

int Tower::get_y() const
{
    return y * Game::BLOCK_SIZE + Game::BLOCK_SIZE / 2;
}

int Tower::firing_rate() const
{
    return fire_rate;
}
