// Copyright 2020 Nathaniel Mitchell

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <memory>
#include <utility>
#include <functional>
#include <optional>

#include <chrono> // NOLINT [build/c++11]
#include <thread> // NOLINT [build/c++11]

#include "SDL_helpers.hpp"
#include "SDL_components.hpp"

using std::cout;


// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


struct ProgramData {
    ManagedSDLWindow    window;
    ManagedSDLSurface   screen_surface;
    ManagedSDLRenderer  renderer;

    ManagedSDLTexture   texture_default;
    ManagedSDLTexture   texture_up;
    ManagedSDLTexture   texture_down;
    ManagedSDLTexture   texture_left;
    ManagedSDLTexture   texture_right;
};


auto run() -> bool;
auto loadData(ProgramData&) -> bool;


int main(__attribute__((unused))int argc, __attribute__((unused))char *argv[]) {
    run();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}


auto run() -> bool {
    auto data       = ProgramData{};
    auto event      = SDL_Event{};
    auto quit       = false;

    auto current_texture = &data.texture_default;

    if (!init()) {
        cout << "Failed to initialize.\n";
        return false;
    }

    if (!loadData(data)) {
        cout << "Failed to load data.\n";
        return false;
    }

    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&event) != 0) {
            //  User requests quit
            if (event.type == SDL_QUIT) {
                quit = true;
            }

            mouse::update(event);
        }

        auto currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_UP]) {
            current_texture = &data.texture_up;
        } else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            current_texture = &data.texture_down;
        } else if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            current_texture = &data.texture_left;
        } else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            current_texture = &data.texture_right;
        } else {
            current_texture = &data.texture_default;
        }

        // Clear screen
        SDL_SetRenderDrawColor(data.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(data.renderer);

        current_texture->render(data.renderer);

        // Update screen
        SDL_RenderPresent(data.renderer);

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    return true;
}


auto loadData(ProgramData& data) -> bool {
    // Create window
    data.window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!data.window) {
        cout << "Window could not be created. SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    // create renderer
    data.renderer = SDL_CreateRenderer(data.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!data.renderer) {
        cout << "Renderer could not be created. SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    // Initialize renderer color
    SDL_SetRenderDrawColor(data.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Get window surface
    data.screen_surface = SDL_GetWindowSurface(data.window);

    data.texture_default = loadTextureFromFile(data.renderer, "images/t18/press.png");
    if (!data.texture_default) { return false; }

    data.texture_up = loadTextureFromFile(data.renderer, "images/t18/up.png");
    if (!data.texture_up) { return false; }

    data.texture_down = loadTextureFromFile(data.renderer, "images/t18/down.png");
    if (!data.texture_down) { return false; }

    data.texture_left = loadTextureFromFile(data.renderer, "images/t18/left.png");
    if (!data.texture_left) { return false; }

    data.texture_right = loadTextureFromFile(data.renderer, "images/t18/right.png");
    if (!data.texture_right) { return false; }


    return true;
}
