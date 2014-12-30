#include "sdl_wrapper.hpp"
#include "textures.hpp"

SDL::texture_handle Textures::GRASS_TEXTURE;
SDL::texture_handle Textures::SAND_TEXTURE;

/* load all of the textures and texture sequences use in the game */
void Textures::load()
{
    Textures::GRASS_TEXTURE = SDL::load_texture("./assets/grass_tile.png");
    Textures::SAND_TEXTURE  = SDL::load_texture("./assets/sand_tile.png");
}
