#pragma once

namespace Camera {
    extern int x;
    extern int y;
    
    void init(int min_x, int min_y, int max_x, int max_y, int w, int h);
    void update();
    void center_on(int x, int y);
    void set_pos(int x, int y);
}
