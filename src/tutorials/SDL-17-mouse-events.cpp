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
#include <array>

#include <chrono> // NOLINT [build/c++11]
#include <thread> // NOLINT [build/c++11]

#include "SDL_helpers.hpp"
#include "SDL_components.hpp"

using std::cout;


// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;


struct ProgramData {
    ManagedSDLWindow    window;
    ManagedSDLSurface   screen_surface;
    ManagedSDLRenderer  renderer;
    ManagedSDLTexture   sprite_sheet;
    std::vector<Button> buttons;
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

        for (auto& b: data.buttons) {
            b.update();
        }

        // Clear screen
        SDL_SetRenderDrawColor(data.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(data.renderer);

        for (auto& b: data.buttons) {
            b.render(data.renderer);
        }

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

    data.sprite_sheet = loadTextureFromFile(data.renderer, "images/t17/button.png");
    if (!data.sprite_sheet) { return false; }

    auto source_clips = std::array<SDL_Rect, 4> {
            SDL_Rect{0,   0, BUTTON_WIDTH, BUTTON_HEIGHT},
            SDL_Rect{0, 200, BUTTON_WIDTH, BUTTON_HEIGHT},
            SDL_Rect{0, 400, BUTTON_WIDTH, BUTTON_HEIGHT},
        };

    auto screen_clips = std::array<SDL_Rect, 4> {
            SDL_Rect{             0,               0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2},
            SDL_Rect{SCREEN_WIDTH/2,               0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2},
            SDL_Rect{             0, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2},
            SDL_Rect{SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2}
        };

    for (auto i=0; i<TOTAL_BUTTONS; i++) {
        data.buttons.push_back(Button{ManagedSDLTexture{data.sprite_sheet, source_clips[0]},
                                      ManagedSDLTexture{data.sprite_sheet, source_clips[1]},
                                      ManagedSDLTexture{data.sprite_sheet, source_clips[2]},
                                      screen_clips[i]});
    }

    return true;
}
