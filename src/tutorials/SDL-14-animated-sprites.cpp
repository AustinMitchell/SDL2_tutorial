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

#include "SDL_helpers.hpp"
#include "SDL_components.hpp"

using std::cout;


// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


struct ProgramData {
    static const int WALKING_ANIMATION_FRAMES = 4;

    ManagedSDLWindow        window;
    ManagedSDLSurface       screen_surface;
    ManagedSDLRenderer      renderer;
    ManagedSDLTexture       sprite_sheet;

    std::array<ManagedSDLTexture, WALKING_ANIMATION_FRAMES> sprite_clips;
};


auto run() -> bool;
auto loadMedia(ProgramData&) -> bool;


auto printRect(SDL_Rect const& rect) -> void {
    cout << "Rect: x=" << rect.x << " y=" << rect.y << " w=" << rect.w << " h=" << rect.h << "\n";
}


int main(__attribute__((unused))int argc, __attribute__((unused))char *argv[]) {
    run();
    IMG_Quit();
    SDL_Quit();
    return 0;
}


auto run() -> bool {
    auto data   = ProgramData{};
    auto frame  = 0;
    auto event  = SDL_Event{};
    auto quit   = false;

    if (!init()) {
        cout << "Failed to initialize.\n";
        return false;
    }

    if (!loadMedia(data)) {
        cout << "Failed to load media.\n";
        return false;
    }

    auto rect = SDL_Rect{(SCREEN_WIDTH  - data.sprite_sheet.rect().w/ProgramData::WALKING_ANIMATION_FRAMES) / 2,
                         (SCREEN_HEIGHT - data.sprite_sheet.rect().h) / 2,
                         data.sprite_sheet.rect().w/ProgramData::WALKING_ANIMATION_FRAMES,
                         data.sprite_sheet.rect().h};

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
        data.sprite_clips[frame/8].render(data.renderer, &rect);

        // Update screen
        SDL_RenderPresent(data.renderer);

        // Go to next frame
        ++frame;

        // Cycle animation
        if (frame/8 >= ProgramData::WALKING_ANIMATION_FRAMES) {
            frame = 0;
        }
    }

    return true;
}


auto init(ProgramData& data) -> bool {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize. SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

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

    // Initialize PNG loading
    auto imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        cout << "SDL_image could not initialize. SDL_image Error: " << IMG_GetError() << "\n";
        return false;
    }

    // Get window surface
    data.screen_surface = SDL_GetWindowSurface(data.window);

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

    auto cyan = SDL_Colour{0, 0xff, 0xff, 0};
    data.sprite_sheet = ManagedSDLTexture{loadTextureFromFile(data.renderer, "images/t14/foo.png", cyan)};
    if (!data.sprite_sheet) { return false; }

    data.sprite_clips[0] = ManagedSDLTexture{data.sprite_sheet, SDL_Rect{  0, 0, 64, 205}};
    data.sprite_clips[1] = ManagedSDLTexture{data.sprite_sheet, SDL_Rect{ 64, 0, 64, 205}};
    data.sprite_clips[2] = ManagedSDLTexture{data.sprite_sheet, SDL_Rect{128, 0, 64, 205}};
    data.sprite_clips[3] = ManagedSDLTexture{data.sprite_sheet, SDL_Rect{196, 0, 64, 205}};

    return true;
}
