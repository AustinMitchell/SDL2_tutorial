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
    ManagedSDLWindow        window;
    ManagedSDLSurface       screen_surface;
    ManagedSDLRenderer      renderer;
    ManagedSDLTexture       sprite_sheet;
    std::array<TextureComponent, 4> sprite_clips;
};

struct color {
    int r, g, b, a;
};


auto run() -> bool;
auto loadMedia(ProgramData&) -> bool;

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

        // Render top left sprite
        data.sprite_clips[0].render(data.renderer);

        // Render top right sprite
        data.sprite_clips[1].render(data.renderer);

        // Render bottom left sprite
        data.sprite_clips[2].render(data.renderer);

        // Render bottom right sprite
        data.sprite_clips[3].render(data.renderer);

        // Update screen
        SDL_RenderPresent(data.renderer);
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
    data.renderer = SDL_CreateRenderer(data.window, -1, SDL_RENDERER_ACCELERATED);
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

    auto cyan = SDL_Colour{0, 0xFF, 0xFF, 0};

    data.sprite_sheet = loadTextureFromFile(data.renderer, "images/t11/dots.png", cyan);
    if (!data.sprite_sheet) { return false; }

    // //Render top left sprite
    // gSpriteSheetTexture.render(0, 0, &gSpriteClips[0]);

    // //Render top right sprite
    // gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[1].w, 0, &gSpriteClips[1]);

    // //Render bottom left sprite
    // gSpriteSheetTexture.render(0, SCREEN_HEIGHT - gSpriteClips[2].h, &gSpriteClips[2]);

    // //Render bottom right sprite
    // gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[3].w, SCREEN_HEIGHT - gSpriteClips[3].h, &gSpriteClips[3]);

    data.sprite_clips[0] = TextureComponent{ManagedSDLTexture{data.sprite_sheet, SDL_Rect{  0,   0, 100, 100}}, {               0,                 0, 100, 100}};
    data.sprite_clips[1] = TextureComponent{ManagedSDLTexture{data.sprite_sheet, SDL_Rect{100,   0, 100, 100}}, {SCREEN_WIDTH-100,                 0, 100, 100}};
    data.sprite_clips[2] = TextureComponent{ManagedSDLTexture{data.sprite_sheet, SDL_Rect{  0, 100, 100, 100}}, {               0, SCREEN_HEIGHT-100, 100, 100}};
    data.sprite_clips[3] = TextureComponent{ManagedSDLTexture{data.sprite_sheet, SDL_Rect{100, 100, 100, 100}}, {SCREEN_WIDTH-100, SCREEN_HEIGHT-100, 100, 100}};

    return true;
}
