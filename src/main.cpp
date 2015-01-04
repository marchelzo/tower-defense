/*
 *
 * Tower Defense
 *
 */

#include <SDL2/SDL.h>
#include <cstdio>
#include <ctime>

#include "sdl_wrapper.hpp"
#include "sprite.hpp"
#include "camera.hpp"
#include "textures.hpp"
#include "map.hpp"
#include "player.hpp"
#include "game.hpp"
#include "tower.hpp"
#include "projectile.hpp"

static TowerUpdate standard_tower_update {[](Tower& t, const std::vector<Enemy>& es, std::vector<Projectile>& ps){
    for (auto& e : es) {
        if (!e.is_alive()) continue;
        int dx = t.get_x() - e.get_x();
        int dy = t.get_y() - e.get_y();
        if (sqrt(dx*dx + dy*dy) < 200) {
            t.shoot(dx, dy, ps);
            break;
        }
    }
}};

/* some constants */
static int constexpr MIN_WIN_WIDTH  = 8;
static int constexpr MIN_WIN_HEIGHT = 5;
static int constexpr MAX_WIN_WIDTH  = 100;
static int constexpr MAX_WIN_HEIGHT = 80;
static std::string const GAME_TITLE {"Tower Defense!"};

int constexpr Game::BLOCK_SIZE = 48;
int Game::MAP_WIDTH;
int Game::MAP_HEIGHT;

using namespace Game;

int main(int argc, char *argv[])
{
    /* parse the command line arguments to determine the window dimensions */
    int win_width, win_height; /* in blocks, not pixels */
    if (argc == 3) {
        win_width  = std::stoi(argv[1]);
        win_height = std::stoi(argv[2]);
        if (win_width < MIN_WIN_WIDTH || win_width > MAX_WIN_WIDTH || win_height < MIN_WIN_HEIGHT || win_height > MAX_WIN_HEIGHT) {
            fprintf(stderr, "Invalid dimensions: %d x %d\nPlease specify a width in [%d,%d] and a height in [%d,%d]\n", win_width, win_height,
                                                                                                                        MIN_WIN_WIDTH, MAX_WIN_WIDTH,
                                                                                                                        MIN_WIN_HEIGHT, MAX_WIN_HEIGHT);
            exit(EXIT_FAILURE);
        }
    } else if (argc == 1) {
        win_width  = 18;
        win_height = 14;
    } else {
        fprintf(stderr, "Invalid arguments\nUsage: %s\nor:    %s <width> <height>\n", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }

    /* seed the random number generator */
    srand(time(nullptr));

    /* width and height are valid, so we initialize SDL */
    SDL::init(BLOCK_SIZE * (win_width + 1), BLOCK_SIZE * win_height, GAME_TITLE);

    /* load assets */
    Textures::load();

    /* load the map */
    Map map {"./maps/test.map"};

    /* make sure the window is not bigger than the actual map */
    if (win_width * 0.7 > map.width()) {
        fprintf(stderr, "Note: Specified width was bigger than the map size. Changing width to %d from %d.\n", map.width(), win_width);
        win_width = 1.434 * map.width();
    }
    if (win_height > map.height()) {
        fprintf(stderr, "Note: Specified height was bigger than the map size. Changing height to %d from %d.\n", map.height(), win_height);
        win_height = map.height();
    }

    MAP_WIDTH  = map.width();
    MAP_HEIGHT = map.height();

    SDL::set_window_size(win_width * BLOCK_SIZE, win_height * BLOCK_SIZE);

    /* initialize the game camera */
    Camera::init(0, 0, map.width() * BLOCK_SIZE - 1, map.height() * BLOCK_SIZE - 1,
            (int) (SDL::WINDOW_WIDTH * 0.7), SDL::WINDOW_HEIGHT, Camera::Type::Keyboard);

    Player::init(99999, 1000);

    std::vector<Enemy> es;
    map.spawn_enemies(3, es);

    std::vector<Projectile> ps;

    /* create an SDL_Event for polling events */
    SDL_Event e;

    Sprite grass {Textures::GRASS_TEXTURE};
    Sprite base {Textures::TOWER_BASE_SIMPLE};
    Sprite bullet {Textures::PROJECTILE_SIMPLE};
    Sprite cannon {Textures::TOWER_CANNON_SIMPLE};

    Tower t {base, cannon, bullet, [](Enemy& e){ e.kill(); }, 2.0, 3.0, standard_tower_update, 4, 5};

    /* game loop */
    while (Player::hp > 0) {
        /* handle input */
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                goto end;
            else if (e.type == SDL_MOUSEWHEEL) {
                    Camera::zoom(e.wheel.y * 0.1);
            }
        }

        /* update enemies */
	for (auto& e : es)
            if (e.is_alive())
                e.update(map);

        for (auto& p : ps)
            p.update();

        t.update(es, ps);

        /* check for collisions between enemies and projectiles */
        for (auto& e : es) {
            int ex = e.get_x();
            int ey = e.get_y();
            for (auto& p : ps) {
               int dx = ex - p.get_x();
               int dy = ey - p.get_y();
               if (sqrt(dx*dx + dy*dy) < Game::BLOCK_SIZE / 2 - 5)
                   p.affect(e);
            }
        }

        Camera::update();

        /* render output */
        SDL::render_clear();

        int block_size = Camera::zoom_amount() * Game::BLOCK_SIZE;
        int x_off = Camera::x % block_size;
        int y_off = Camera::y % block_size;

        for (int y = 0; y < Camera::height / block_size + 2; ++y) {
            for (int x = 0; x < Camera::width / block_size + 2; ++x) {

                int x_pos = block_size * x - x_off;
                int y_pos = block_size * y - y_off;

                grass.draw(x_pos, y_pos, Camera::zoom_amount());

                int ty = y + Camera::y / block_size;
                int tx = x + Camera::x / block_size;

                if (ty >= map.height() || tx >= map.width()) continue;
                if (ty < 0             || tx < 0           ) continue;

                const Tile& t = map(ty,tx);

                if (t.get_type() != TileType::GRASS) {
                    t.draw(x_pos, y_pos, Camera::zoom_amount());
                }
            }
        }

	for (auto& e : es)
            if (e.is_alive())
                e.draw();

        for (auto& p : ps)
            p.draw();
        
        t.draw();

        SDL_Rect r = { Camera::width, 0, SDL::WINDOW_WIDTH - Camera::width, SDL::WINDOW_HEIGHT };
        SDL::render_rect(&r, 102, 49, 60, 255);

        SDL::render_present();
    }

end: /* clean up and quit */
    SDL::quit();
    return 0;
}
