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

#include "SDL_helpers.hpp"

using std::cout;


// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


struct ProgramData {
    ManagedSDLWindow    window;
    ManagedSDLSurface   screen_surface;
    ManagedSDLRenderer  renderer;
    ManagedSDLTexture   texture;
    ManagedTTFFont      font;
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

    auto render_rect = SDL_Rect{(SCREEN_WIDTH  - data.texture.rect().w)/2,
                                (SCREEN_HEIGHT - data.texture.rect().h)/2,
                                data.texture.rect().w,
                                data.texture.rect().h};

    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&event) != 0) {
            //  User requests quit
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(data.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(data.renderer);

        // Render current frame
        data.texture.render(data.renderer, &render_rect);

        // Update screen
        SDL_RenderPresent(data.renderer);
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

    data.font = loadFont("fonts/lazy.ttf", 28);
    if (!data.font) { return false; }

    auto black = SDL_Colour{0, 0, 0, 0};
    data.texture = loadTextureFromText(data.renderer, "The quick brown fox jumps over the lazy dog", data.font, black);
    if (!data.texture) { return false; }

    return true;
}
