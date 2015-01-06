#include <utility>
#include <vector>

#include "interface.hpp"

Interface::Button::Button(Sprite sprite,
       Action on_click,
       Action on_hover, int x, int y):
    sprite       {sprite},
    on_click     {on_click},
    on_hover     {on_hover},
    x            {x},
    y            {y}
{
}

Interface::Interface(int w, int h, int x, int y, const Sprite& button_bg):
    width         {w},
    height        {h},
    x             {x},
    y             {y},
    button_bg     {button_bg},
    padding_top   {20},
    padding_left  {20},
    horiz_padding {20},
    vert_padding  {20}
{
}

void Interface::add_button(Sprite sprite, Action on_click, Action on_hover)
{
    int x, y;

    if (buttons.size() == 0) {
	x = padding_left;
	y = padding_top;
    } else {
	x = buttons.back().x + Button::size + horiz_padding;
	y = buttons.back().y;

	if (x + Button::size + horiz_padding > width) {
	    x = padding_left;
	    y += Button::size + vert_padding;
	}
    }

    buttons.emplace_back(sprite, on_click, on_hover, x, y);
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

void Interface::draw() const
{
    for (auto& b : buttons) {
	b.sprite.draw(b.x + x, b.y + y);
	button_bg.draw(b.x + x, b.y + y);
    }
}

void Interface::set_padding_top(int k)
{
    int dp = k - padding_top;
    for (auto& b : buttons)
	b.y += k;
    padding_top = k;
}

void Interface::set_padding_left(int k)
{
    int dp = k - padding_left;
    for (auto& b : buttons)
	b.x += k;
    padding_left = k;
}

void Interface::set_vertical_padding(int k)
{
    int dp = k - vert_padding;
    for (auto& b : buttons)
	b.y += dp;
    vert_padding = k;
}

void Interface::set_horizontal_padding(int k)
{
    int dp = k - horiz_padding;
    for (auto& b : buttons)
	b.x += dp;
    horiz_padding = k;
}
