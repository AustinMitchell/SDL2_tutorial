// Copyright 2020 Nathaniel Mitchell

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <memory>
#include <utility>
#include <functional>

#include "SDL_helpers.hpp"


using std::cout;
using surface_string_pair = std::pair<std::reference_wrapper<ManagedSDLSurface>, char const*>;


// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


auto run() -> bool;
auto init(ManagedSDLWindow&, ManagedSDLSurface&) -> bool;
template<uint64_t N>
bool loadMedia(std::array<surface_string_pair, N>, ManagedSDLSurface&);


int main() {
    run();
    SDL_Quit();
    return 0;
}


auto run() -> bool {
    auto window         = ManagedSDLWindow{};
    auto screen_surface = ManagedSDLSurface{};

    auto scaled_surface = ManagedSDLSurface{};

    auto event          = SDL_Event{};
    auto quit           = false;

    auto stretchRect = SDL_Rect{};
    stretchRect.x = 0;
    stretchRect.y = 0;
    stretchRect.w = SCREEN_WIDTH;
    stretchRect.h = SCREEN_HEIGHT;

    if (!init(window, screen_surface)) {
        cout << "Failed to initialize.\n";
        return false;
    }

    auto surface_array = std::array<surface_string_pair, 1> {{
            {scaled_surface, "images/loaded.png"}
        }};
    if (!loadMedia(surface_array, screen_surface)) {
        cout << "Failed to load media.\n";
        return false;
    }

    while (!quit) {
        //  Handle events on queue
        while (SDL_PollEvent(&event) != 0) {
            //  User requests quit
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_BlitScaled(scaled_surface, NULL, screen_surface, &stretchRect);
        SDL_UpdateWindowSurface(window);
    }

    return true;
}


auto init(ManagedSDLWindow& window, ManagedSDLSurface& screen_surface) -> bool {
    //  Initialization flag
    bool success = true;

    //  Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        success = false;
    } else {
        //  Create window
        window = SDL_CreateWindow("SDL Tutorial",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH,
                                  SCREEN_HEIGHT,
                                  SDL_WINDOW_SHOWN);
        if (!window) {
            cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
            success = false;
        } else {
            //  Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags)) {
                cout << "SDL_image could not initialize. SDL_image Error: " << IMG_GetError() << "\n";
                success = false;
            } else {
                //  Get window surface
                screen_surface = SDL_GetWindowSurface(window);
            }
        }
    }

    return success;
}

template<uint64_t N>
bool loadMedia(std::array<surface_string_pair, N> surfaces_to_load, ManagedSDLSurface& screen_surface) {
    bool success = true;
    for (auto& pair: surfaces_to_load) {
        success = success && loadSurface(pair.first, pair.second, screen_surface);
    }
    return success;
}
