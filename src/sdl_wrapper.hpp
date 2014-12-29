#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <map>

#pragma once


/* public SDL interface */
namespace SDL {

    /* type aliases to make the interface cleaner */
    typedef size_t texture_handle;
    typedef size_t texture_sequence_handle;
    typedef size_t sound_handle;

    /* animation data structure */
    class Animation {
        const size_t frame_duration;
        const texture_sequence_handle ts;
        size_t _current_frame;
        size_t frame_count;
        size_t _times_played;
        bool _playing;
    public:
        Animation(texture_sequence_handle ts, size_t frame_duration);
        bool playing() const;
        void pause();
        void play();
        void set_frame(size_t frame_number);
        int width() const;
        int height() const;
        size_t times_played() const;
        size_t current_frame() const;
        void render(SDL_Rect* r, double angle, SDL_RendererFlip flip);

    };

    bool init(int w, int h, const std::string& title);
    void quit();
    extern int WINDOW_WIDTH;
    extern int WINDOW_HEIGHT;
    extern int mouse_x;
    extern int mouse_y;
    void get_mouse_state();
    void render_clear();
    void render_present();
    texture_sequence_handle load_texture_sequence(const std::string& path, size_t frames);
    size_t load_texture(const std::string&);
    size_t texture_from_string(const std::string&, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    size_t small_texture_from_string(const std::string&, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    size_t load_sound(const std::string&);
    bool set_texture(size_t);

    bool render_texture(size_t, int x, int y);
    bool render_texture(size_t, int x, int y, double angle);
    bool render_texture(size_t, SDL_Rect* r, double angle, SDL_RendererFlip f);
    void render_texture(int x, int y);

    size_t render_animation(size_t id, int x, int y);
    size_t render_animation(size_t id, SDL_Rect* r, double angle, SDL_RendererFlip flip);
    size_t times_played(size_t animation_id);

    void replace_texture(size_t, size_t);
    void render_rect(const SDL_Rect *rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void play_sound(size_t id);
    void play_sound_loop(size_t id);
    void stop_loop(size_t id);
    void stop_all_sounds();
    int texture_width(size_t);
    int texture_height(size_t);
}
