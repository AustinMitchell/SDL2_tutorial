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
#include "SDL_components.hpp"

using std::cout;


// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


struct ProgramData {
    ManagedSDLWindow    window;
    ManagedSDLSurface   screen_surface;
    ManagedSDLRenderer  renderer;

    ManagedSDLTexture   foreground,
                        background;
};

struct color {
    int r,
        g,
        b,
        a;
};


auto run() -> bool;
auto loadMedia(ProgramData&) -> bool;
auto printRect(SDL_Rect const&) -> void;


int main(__attribute__((unused))int argc, __attribute__((unused))char *argv[]) {
    run();
    IMG_Quit();
    SDL_Quit();
    return 0;
}


auto run() -> bool {
    auto data = ProgramData{};

    auto event          = SDL_Event{};
    auto quit           = false;

    if (!init()) {
        cout << "Failed to initialize.\n";
        return false;
    }

    if (!loadMedia(data)) {
        cout << "Failed to load media.\n";
        return false;
    }

    auto clip_foreground = SDL_Rect{240, 190, data.foreground.rect().w, data.foreground.rect().h};

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

        // Render background texture to screen
        data.background.render(data.renderer);

        // Render Foo' to the screen
        data.foreground.render(data.renderer, &clip_foreground);

        // Update screen
        SDL_RenderPresent(data.renderer);
    }

    return true;
}


auto loadMedia(ProgramData& data) -> bool {
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

    auto cyan = SDL_Colour{0, 0xFF, 0xFF, 0};

    data.background = loadTextureFromFile(data.renderer, "images/t10/background.png", cyan);
    if (!data.background) { return false; }

    data.foreground = loadTextureFromFile(data.renderer, "images/t10/foo.png", cyan);
    if (!data.foreground) { return false; }

    return true;
}

auto printRect(SDL_Rect const& rect) -> void {
    cout << "Rect: x=" << rect.x << " y=" << rect.y << " w=" << rect.w << " h=" << rect.h << "\n";
}
