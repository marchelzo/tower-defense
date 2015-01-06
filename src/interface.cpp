#include <utility>

#include "interface.hpp"

Interface::Button::Button(Sprite sprite,
       Action on_click,
       Action on_hover, int x, int y):
    sprite   {sprite},
    on_click {on_click},
    on_hover {on_hover},
    x        {x},
    y        {y}
{
}

Interface::Interface(int w, int h, int x, int y):
    width  {w},
    height {h},
    x      {x},
    y      {y}
{
}

void Interface::add_button(Sprite sprite, Action on_click, Action on_hover)
{
    buttons.emplace_back(sprite, on_click, on_hover);
}

void Interface::click_event(int x, int y)
{
    clicks.emplace_back(x,y);
}

void Interface::update()
{
    for (auto& c : clicks) {
        for (auto& b : buttons) {
            /* check to see if the click was inside the button */
            if (c.x > x + b.x && c.x < x + b.x + Button::size) {
                if (c.y > y + b.y && c.y < y + b.y + Button::size) {
                    b.on_click();
                    break;
                }
            }
        }
        std::swap(c, clicks.back());
        clicks.pop_back();
    }
    /* do hover events */
    for (auto& b : buttons) {
        if (SDL::mouse_x > x + b.x && SDL::mouse_x < x + b.x + Button::size) {
            if (SDL::mouse_y > y + b.y && SDL::mouse_y < y + b.y + Button::size) {
                b.on_hover();
                break;
            }
        }
    }
}
