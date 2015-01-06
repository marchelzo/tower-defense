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
    const Sprite& button_bg;
    int padding_top;
    int padding_left;
public:
    Interface(int w, int h, int x, int y, const Sprite& button_bg);
    void add_button(Sprite sprite, Action on_click,
                    Action on_hover);
    void click_event(int x, int y);
    void update();
    void draw() const;
    void set_padding_left(int k);
    void set_padding_top(int k);
};
