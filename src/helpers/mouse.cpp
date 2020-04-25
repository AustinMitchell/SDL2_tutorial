#include <SDL2/SDL.h>
#include <iostream>
#include "helpers/mouse.hpp"

static int mouse_x = 0;
static int mouse_y = 0;
static int mouse_prevx = 0;
static int mouse_prevy = 0;
static bool mouse_pressed = false;


auto mouse::x() -> int { return mouse_x; }
auto mouse::y() -> int { return mouse_y; }
auto mouse::prevx() -> int { return mouse_prevx; }
auto mouse::prevy() -> int { return mouse_prevy; }
auto mouse::pressed() -> bool { return mouse_pressed; }


auto mouse::update(SDL_Event const& event) -> void {
    // If mouse event happened
    if (event.type == SDL_MOUSEMOTION) {
        SDL_GetMouseState(&mouse_x, &mouse_y);
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        mouse_pressed = true;
    } else if (event.type == SDL_MOUSEBUTTONUP) {
        mouse_pressed = false;
    }
}
