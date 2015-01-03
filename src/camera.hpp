#pragma once

namespace Camera {

    enum class Type {
        Keyboard, Mouse
    };

    extern int x;
    extern int y;

    extern int height;
    extern int width;

    void init(int min_x, int min_y, int max_x, int max_y, int w, int h, Type);
    void update();
    void center_on(int x, int y);
    void set_pos(int x, int y);
    double zoom_amount();
    void zoom(double k);
}
