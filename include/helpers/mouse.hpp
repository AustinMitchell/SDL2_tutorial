#ifndef HELPERS_MOUSE_HPP_
#define HELPERS_MOUSE_HPP_

#include <SDL2/SDL.h>

namespace mouse {
    auto x() -> int;
    auto y() -> int;
    auto prevx() -> int;
    auto prevy() -> int;
    auto pressed() -> bool;
    auto moved() -> bool;
    auto update(SDL_Event const&) -> void;
}

#endif
