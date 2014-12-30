#include "camera.hpp"
#include "sdl_wrapper.hpp"

static float constexpr THRESHOLD       = 0.4;
static float constexpr SCROLL_SPEED    = 1000;
static int constexpr MIN_SCROLL_AMOUNT = 6;
static int constexpr MAX_SCROLL_AMOUNT = 12;

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

static int CAM_W;
static int CAM_H;

static void clip_camera()
{
    /* make sure the camera stays within the max range */
    if (Camera::x < 0) Camera::x = 0;
    if (Camera::y < 0) Camera::y = 0;
    if (Camera::x + CAM_W > MAX_X) Camera::x = MAX_X - CAM_W;
    if (Camera::y + CAM_H > MAX_Y) Camera::y = MAX_Y - CAM_H;
}

void Camera::init(int min_x, int min_y, int max_x, int max_y, int w, int h)
{
    MIN_X = min_x;
    MIN_Y = min_y;

    MAX_X = max_x;
    MAX_Y = max_y;

    CAM_W = w;
    CAM_H = h;

    Camera::x = 0;
    Camera::y = 0;
}

void Camera::update()
{
    SDL::get_mouse_state();

    direction_t scroll_x {NONE};
    direction_t scroll_y {NONE};

    if (SDL::mouse_x > (float) SDL::WINDOW_WIDTH * (1 - THRESHOLD)) scroll_x = RIGHT;
    if (SDL::mouse_x < (float) SDL::WINDOW_WIDTH * THRESHOLD) scroll_x = LEFT;

    if (SDL::mouse_y > (float) SDL::WINDOW_HEIGHT * (1 - THRESHOLD)) scroll_y = DOWN;
    if (SDL::mouse_y < (float) SDL::WINDOW_HEIGHT * THRESHOLD) scroll_y = UP;

    if (scroll_x == RIGHT) {
        float scale_factor = 1.0 / (float) (SDL::WINDOW_WIDTH - SDL::mouse_x + 1);
        Camera::x += std::max(std::min((int) (scale_factor * SCROLL_SPEED), MAX_SCROLL_AMOUNT), MIN_SCROLL_AMOUNT);
    } else if (scroll_x == LEFT) {
        float scale_factor = 1.0 / (float) (SDL::mouse_x + 1);
        Camera::x -= std::max(std::min((int) (scale_factor * SCROLL_SPEED), MAX_SCROLL_AMOUNT), MIN_SCROLL_AMOUNT);
    }

    if (scroll_y == UP) {
        float scale_factor = 1.0 / (float) (SDL::mouse_y + 1);
        Camera::y -= std::max(std::min((int) (scale_factor * SCROLL_SPEED), MAX_SCROLL_AMOUNT), MIN_SCROLL_AMOUNT);
    } else if (scroll_y == DOWN) {
        float scale_factor = 1.0 / (float) (SDL::WINDOW_HEIGHT - SDL::mouse_y + 1);
        Camera::y += std::max(std::min((int) (scale_factor * SCROLL_SPEED), MAX_SCROLL_AMOUNT), MIN_SCROLL_AMOUNT);
    }

    clip_camera();
}

void Camera::set_pos(int x, int y)
{
    Camera::x = x;
    Camera::y = y;
    clip_camera();
}

void Camera::center_on(int x, int y)
{
    Camera::x = x - (int) (float) SDL::WINDOW_WIDTH / 2.0;
    Camera::y = y - (int) (float) SDL::WINDOW_HEIGHT / 2.0;
    clip_camera();
}
