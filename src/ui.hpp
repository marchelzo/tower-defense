#include "interface.hpp"

#pragma once

namespace UserInterface {

    enum Menu {
	MAIN,
	NUM_MENUS
    };

    extern Menu current_menu;
    
    extern std::unique_ptr<Interface> menus[NUM_MENUS];

    extern std::string message;

    void init();
    void draw();
    void click_event(int x, int y);
    void update();
}
