// Copyright 2020 Nathaniel Mitchell

#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <utility>
#include <array>

#include "SDL_helpers.hpp"


using std::cout;


enum SurfaceState {
    SS_DEFAULT,
    SS_UP,
    SS_DOWN,
    SS_LEFT,
    SS_RIGHT,
    SS_TOTAL
};


// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


auto run() -> bool;
auto init(ManagedSDLWindow&, ManagedSDLSurface&) -> bool;
auto loadSurface(ManagedSDLSurface&, char const*) -> bool;
template<int N>
bool loadMedia(std::array<std::pair<ManagedSDLSurface*, char const*>, N>);


int main(__attribute__((unused))int argc, __attribute__((unused))char *argv[]) {
    run();
    SDL_Quit();
    return 0;
}


auto run() -> bool {
    auto window         = ManagedSDLWindow{};
    auto screen_surface = ManagedSDLSurface{};

    SDL_Surface* current_surface;

    auto key_press_surfaces     = std::array<ManagedSDLSurface, SS_TOTAL>{};
    auto key_press_image_names  = std::array<char const*, SS_TOTAL>{
            "images/t04/press.bmp",
            "images/t04/up.bmp",
            "images/t04/down.bmp",
            "images/t04/left.bmp",
            "images/t04/right.bmp"
        };

    auto event          = SDL_Event{};
    auto quit           = false;


    if (!init(window, screen_surface)) {
        cout << "Failed to initialize.\n";
        return false;
    }

    if (!loadMedia<SS_TOTAL>({{{&key_press_surfaces[SS_DEFAULT],   key_press_image_names[SS_DEFAULT]},
                               {&key_press_surfaces[SS_UP],        key_press_image_names[SS_UP]},
                               {&key_press_surfaces[SS_DOWN],      key_press_image_names[SS_DOWN]},
                               {&key_press_surfaces[SS_LEFT],      key_press_image_names[SS_LEFT]},
                               {&key_press_surfaces[SS_RIGHT],     key_press_image_names[SS_RIGHT]}}})) {
        cout << "Failed to load media.\n";
        return false;
    }

    current_surface = key_press_surfaces[SS_DEFAULT];

    while (!quit) {
        //  Handle events on queue
        while (SDL_PollEvent(&event) != 0) {
            //  User requests quit
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                //  Select surfaces based on key press
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        current_surface = key_press_surfaces[SS_UP];
                        break;
                    case SDLK_DOWN:
                        current_surface = key_press_surfaces[SS_DOWN];
                        break;
                    case SDLK_LEFT:
                        current_surface = key_press_surfaces[SS_LEFT];
                        break;
                    case SDLK_RIGHT:
                        current_surface = key_press_surfaces[SS_RIGHT];
                        break;
                    default:
                        current_surface = key_press_surfaces[SS_DEFAULT];
                        break;
                }
            }
        }
        SDL_BlitSurface(current_surface, nullptr, screen_surface, nullptr);
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
            //  Get window surface
            screen_surface = SDL_GetWindowSurface(window);
        }
    }

    return success;
}

template<int N>
bool loadMedia(std::array<std::pair<ManagedSDLSurface*, char const*>, N> surfaces_to_load) {
    bool success = true;
    for (auto& [surface_ptr, filename]: surfaces_to_load) {
        success = success && loadSurface(surface_ptr, filename);
    }
    return success;
}


bool loadSurface(ManagedSDLSurface* image_surface, char const* image_name) {
    //  Loading success flag
    bool success = true;

    //  Load splash image
    *image_surface = SDL_LoadBMP(image_name);
    if (!(*image_surface)) {
        cout << "Unable to load image " << image_name << ". SDL Error: " << SDL_GetError() << "\n";
        success = false;
    }

    return success;
}
