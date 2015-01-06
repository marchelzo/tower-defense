#include "sdl_wrapper.hpp"
#include "textures.hpp"

SDL::texture_handle Textures::GRASS_TEXTURE;
SDL::texture_handle Textures::SAND_TEXTURE;
SDL::texture_handle Textures::SAND_CORNER_TEXTURE;
SDL::texture_handle Textures::SAND_3WAY_TEXTURE;
SDL::texture_handle Textures::SAND_4WAY_TEXTURE;
SDL::texture_handle Textures::HOME_TEXTURE;
SDL::texture_handle Textures::SPAWN_TEXTURE;
SDL::texture_handle Textures::RED_SPHERE;
SDL::texture_handle Textures::TOWER_BASE_SIMPLE;
SDL::texture_handle Textures::TOWER_CANNON_SIMPLE;
SDL::texture_handle Textures::PROJECTILE_SIMPLE;
SDL::texture_handle Textures::BUTTON_BORDER;

/* load all of the textures and texture sequences use in the game */
void Textures::load()
{
    Textures::GRASS_TEXTURE       = SDL::load_texture("./assets/grass_tile.png");
    Textures::SAND_TEXTURE        = SDL::load_texture("./assets/sand_tile.png");
    Textures::SAND_CORNER_TEXTURE = SDL::load_texture("./assets/sand_tile_corner.png");
    Textures::SAND_3WAY_TEXTURE   = SDL::load_texture("./assets/sand_tile_3.png");
    Textures::SAND_4WAY_TEXTURE   = SDL::load_texture("./assets/sand_tile_4.png");
    Textures::HOME_TEXTURE        = SDL::load_texture("./assets/home_tile.png");
    Textures::SPAWN_TEXTURE       = SDL::load_texture("./assets/spawn_tile.png");
    Textures::RED_SPHERE          = SDL::load_texture("./assets/red_sphere.png");
    Textures::TOWER_BASE_SIMPLE   = SDL::load_texture("./assets/simple_tower_base.png");
    Textures::TOWER_CANNON_SIMPLE = SDL::load_texture("./assets/simple_tower_cannon.png");
    Textures::PROJECTILE_SIMPLE   = SDL::load_texture("./assets/simple_projectile.png");
    Textures::BUTTON_BORDER       = SDL::load_texture("./assets/button_border.png");
}
