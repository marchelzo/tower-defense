#include <functional>
#include <vector>

#include "sprite.hpp"

#pragma once

typedef std::function<void()> Action;

class Interface {

    struct Button {
        static const int size = 48;
        Sprite sprite;
        Action on_click;
        Action on_hover;
        int x;
        int y;
        Button(Sprite sprite,
               Action on_click,
               Action on_hover,
               int x, int y);
               
    };

    struct Point {
        int x;
        int y;
        Point(int x, int y):
            x {x},
            y {y}
        { }
    };

    std::vector<Button> buttons;
    std::vector<Point> clicks;
    int width;
    int height;
    int x;
    int y;
public:
    Interface(int w, int h, int x, int y);
    void add_button(Sprite sprite, Action on_click,
                    Action on_hover);
    void click_event(int x, int y);
    void update();
};
