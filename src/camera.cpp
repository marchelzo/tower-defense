#include <SDL2/SDL.h>

#include "camera.hpp"
#include "sdl_wrapper.hpp"
#include "game.hpp"

using namespace Camera;

static double constexpr THRESHOLD       = 0.4;
static double constexpr SCROLL_SPEED    = 1000;
static int constexpr MIN_SCROLL_AMOUNT = 6;
static int constexpr MAX_SCROLL_AMOUNT = 12;
static int constexpr KBD_SPEED         = 7;

static void (*update_function)();

enum direction_t {
    LEFT, RIGHT, UP, DOWN, NONE
};

/*  camera position  */
/* x: left of camera */
/* y: top  of camera */

int Camera::x;
int Camera::y;

static int MIN_X;
static int MIN_Y;

static int MAX_X;
static int MAX_Y;

int Camera::width;
int Camera::height;

static double _zoom;

static void clip_camera()
{
    /* make sure the camera stays within the max range */
    if (Camera::x < MIN_X) Camera::x = MIN_X;
    if (Camera::y < MIN_Y) Camera::y = MIN_Y;
    if (Camera::x + width >= Game::MAP_WIDTH * ((int) (Game::BLOCK_SIZE * _zoom))) Camera::x = Game::MAP_WIDTH * ((int) (Game::BLOCK_SIZE * _zoom)) - width;
    if (Camera::y + height >= Game::MAP_HEIGHT * ((int) (Game::BLOCK_SIZE * _zoom))) Camera::y = Game::MAP_HEIGHT * ((int) (Game::BLOCK_SIZE * _zoom)) - height;
}

static void mouse_update()
{
    SDL::get_mouse_state();

    direction_t scroll_x {NONE};
    direction_t scroll_y {NONE};

    if (SDL::mouse_x > (double) SDL::WINDOW_WIDTH * (1 - THRESHOLD)) scroll_x = RIGHT;
    if (SDL::mouse_x < (double) SDL::WINDOW_WIDTH * THRESHOLD) scroll_x = LEFT;

    if (SDL::mouse_y > (double) SDL::WINDOW_HEIGHT * (1 - THRESHOLD)) scroll_y = DOWN;
    if (SDL::mouse_y < (double) SDL::WINDOW_HEIGHT * THRESHOLD) scroll_y = UP;

    if (scroll_x == RIGHT) {
        double scale_factor = 1.0 / (double) (SDL::WINDOW_WIDTH - SDL::mouse_x + 1);
        Camera::x += std::max(std::min((int) (scale_factor * SCROLL_SPEED), MAX_SCROLL_AMOUNT), MIN_SCROLL_AMOUNT);
    } else if (scroll_x == LEFT) {
        double scale_factor = 1.0 / (double) (SDL::mouse_x + 1);
        Camera::x -= std::max(std::min((int) (scale_factor * SCROLL_SPEED), MAX_SCROLL_AMOUNT), MIN_SCROLL_AMOUNT);
    }

    if (scroll_y == UP) {
        double scale_factor = 1.0 / (double) (SDL::mouse_y + 1);
        Camera::y -= std::max(std::min((int) (scale_factor * SCROLL_SPEED), MAX_SCROLL_AMOUNT), MIN_SCROLL_AMOUNT);
    } else if (scroll_y == DOWN) {
        double scale_factor = 1.0 / (double) (SDL::WINDOW_HEIGHT - SDL::mouse_y + 1);
        Camera::y += std::max(std::min((int) (scale_factor * SCROLL_SPEED), MAX_SCROLL_AMOUNT), MIN_SCROLL_AMOUNT);
    }

    clip_camera();
}

static void keyboard_update()
{
    const Uint8* keys = SDL_GetKeyboardState(nullptr);

    if (keys[SDL_SCANCODE_S])
        Camera::y += KBD_SPEED;
    if (keys[SDL_SCANCODE_W])
        Camera::y -= KBD_SPEED;
    if (keys[SDL_SCANCODE_D])
        Camera::x += KBD_SPEED;
    if (keys[SDL_SCANCODE_A])
        Camera::x -= KBD_SPEED;

    clip_camera();
}

void normalize_zoom()
{
    int block_size = _zoom * Game::BLOCK_SIZE;
    while (width / block_size >= Game::MAP_WIDTH) {
        _zoom *= 1.0001;
        block_size = _zoom * Game::BLOCK_SIZE;
    }

    while (height / block_size >= Game::MAP_HEIGHT) {
        _zoom *= 1.0001;
        block_size = _zoom * Game::BLOCK_SIZE;
    }

    _zoom = std::min(_zoom, 3.0);
}

void Camera::init(int min_x, int min_y, int max_x, int max_y, int w, int h, Type t)
{
    MIN_X = min_x;
    MIN_Y = min_y;

    MAX_X = max_x;
    MAX_Y = max_y;

    width = w;
    height = h;

    Camera::x = 0;
    Camera::y = 0;

    _zoom = 1.0;

    if (t == Type::Keyboard)
        update_function = keyboard_update;
    else
        update_function = mouse_update;
}

void Camera::update()
{
    update_function();
}

void Camera::set_pos(int x, int y)
{
    Camera::x = x;
    Camera::y = y;
    clip_camera();
}

void Camera::center_on(int x, int y)
{
    Camera::x = x * _zoom - width  / 2.0;
    Camera::y = y * _zoom - height / 2.0;
    clip_camera();
}

void Camera::zoom(double k)
{
    SDL::get_mouse_state();

    int x = (Camera::x + SDL::mouse_x) / _zoom;
    int y = (Camera::y + SDL::mouse_y) / _zoom;

    _zoom += k;
    normalize_zoom();

    center_on(x, y);
    clip_camera();
}

double Camera::zoom_amount()
{
    return _zoom;
}
