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

    TextureComponent    foreground,
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


int main() {
    run();
    IMG_Quit();
    SDL_Quit();
    return 0;
}


auto run() -> bool {
    auto data = ProgramData{};

    auto event          = SDL_Event{};
    auto quit           = false;

    auto stretchRect = SDL_Rect{};
    stretchRect.x = 0;
    stretchRect.y = 0;
    stretchRect.w = SCREEN_WIDTH;
    stretchRect.h = SCREEN_HEIGHT;

    if (!init()) {
        cout << "Failed to initialize.\n";
        return false;
    }

    if (!loadMedia(data)) {
        cout << "Failed to load media.\n";
        return false;
    }

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
        data.foreground.render(data.renderer);

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
    if (!data.background.texture()) { return false; }
    data.background.setPos({0, 0}).setDimToTexture();

    data.foreground = loadTextureFromFile(data.renderer, "images/t10/foo.png", cyan);
    if (!data.foreground.texture()) { return false; }
    data.foreground.setPos({240, 190}).setDimToTexture();

    cout << "Media loaded successfully\n";
    cout << "background:         ";
    printRect(data.background.rect());
    cout << "background texture: ";
    printRect(data.background.texture().clip());
    cout << "texture:            ";
    printRect(data.foreground.rect());
    cout << "texture texture:    ";
    printRect(data.foreground.texture().clip());

    return true;
}

auto printRect(SDL_Rect const& rect) -> void {
    cout << "Rect: x=" << rect.x << " y=" << rect.y << " w=" << rect.w << " h=" << rect.h << "\n";
}