#include <iostream>
#include <memory>

#include "ui.hpp"
#include "sdl_wrapper.hpp"
#include "textures.hpp"
#include "game.hpp"
#include "camera.hpp"

static constexpr int PADDING_TOP   = 100;
static constexpr int PADDING_LEFT  = 20;
static constexpr int HORIZ_PADDING = 20;
static constexpr int VERT_PADDING  = 20;

static SDL_Rect new_tower_pos { 0, 0, Game::BLOCK_SIZE, Game::BLOCK_SIZE};

static std::unique_ptr<Sprite> button_border;

static std::function<void()> update_function {nullptr};
static std::function<void()> draw_function   {nullptr};

static Action buy_tower = [](){
    draw_function = [](){
	SDL::get_mouse_state();

	int block_size = Game::BLOCK_SIZE * Camera::zoom_amount();

	new_tower_pos.x = block_size * (SDL::mouse_x / block_size) - Camera::x % block_size;
	new_tower_pos.y = block_size * (SDL::mouse_y / block_size) - Camera::y % block_size;

	new_tower_pos.w = block_size;
	new_tower_pos.h = block_size;

	SDL::render_rect(&new_tower_pos, 20, 200, 20, 100);
    };
};


static Interface* load_main_menu()
{

    int width = SDL::WINDOW_WIDTH * 0.3;
    int height = SDL::WINDOW_HEIGHT;

    int x_off = SDL::WINDOW_WIDTH * 0.7;
    int y_off = 0;

    Interface* ifc = new Interface{width, height, x_off, y_off, *button_border};

    ifc->set_padding_top(PADDING_TOP);
    ifc->set_padding_left(PADDING_LEFT);
    ifc->set_horizontal_padding(HORIZ_PADDING);
    ifc->set_vertical_padding(VERT_PADDING);

    ifc->add_button(Sprite{Textures::GRASS_TEXTURE}, buy_tower, [](){});
    ifc->add_button(Sprite{Textures::GRASS_TEXTURE}, [](){ std::cout << "2" << std::endl; }, [](){});

    return ifc;
}

UserInterface::Menu UserInterface::current_menu {MAIN};

std::unique_ptr<Interface> UserInterface::menus[NUM_MENUS];

void UserInterface::init()
{
    button_border = std::make_unique<Sprite>(Textures::BUTTON_BORDER);
    menus[MAIN]   = std::unique_ptr<Interface>(load_main_menu());
}

void UserInterface::draw()
{
    menus[current_menu]->draw();
    if (draw_function)
	draw_function();
}

void UserInterface::click_event(int x, int y)
{
    menus[current_menu]->click_event(x,y);
}

void UserInterface::update()
{
    menus[current_menu]->update();
    if (update_function)
	update_function();
}
