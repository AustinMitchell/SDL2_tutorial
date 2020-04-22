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
    ManagedSDLWindow        window;
    ManagedSDLSurface       screen_surface;
    ManagedSDLRenderer      renderer;
    ManagedSDLTexture       sprite_sheet;
    std::array<SDL_Rect, 4> sprite_clips;
};

struct color {
    int r, g, b, a;
};


auto run() -> bool;
auto init(ProgramData&) -> bool;
auto loadMedia(ProgramData&) -> bool;
auto loadSurface(ManagedSDLSurface&, char const*, ManagedSDLSurface&) -> bool;
auto loadTexture(ManagedSDLTexture&, char const*, ManagedSDLRenderer&, std::optional<color>) -> bool;


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

    if (!init(data)) {
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
        data.sprite_sheet.render(data.renderer, 0, 0, &data.sprite_clips[0]);

        // Render top right sprite
        data.sprite_sheet.render(data.renderer, SCREEN_WIDTH - data.sprite_clips[1].w, 0, &data.sprite_clips[1]);

        // Render bottom left sprite
        data.sprite_sheet.render(data.renderer, 0, SCREEN_HEIGHT - data.sprite_clips[2].h, &data.sprite_clips[2]);

        // Render bottom right sprite
        data.sprite_sheet.render(data.renderer, SCREEN_WIDTH - data.sprite_clips[3].w, SCREEN_HEIGHT - data.sprite_clips[3].h, &data.sprite_clips[3]);

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
    bool success;

    auto cyan = color{0, 0xFF, 0xFF, 0};

    success = loadTexture(data.sprite_sheet, "images/dots.png", data.renderer, cyan);
    if (!success) { return false; }

    data.sprite_clips[0] = {0,   0,   100, 100};
    data.sprite_clips[1] = {100, 0,   100, 100};
    data.sprite_clips[2] = {0,   100, 100, 100};
    data.sprite_clips[3] = {100, 100, 100, 100};

    return true;
}


auto loadSurface(ManagedSDLSurface& image_surface, char const* image_name, ManagedSDLSurface& screen_surface) -> bool {
    auto raw_surface    = ManagedSDLSurface{IMG_Load(image_name)};

    if (!raw_surface) {
        cout << "Unable to load image " << image_name << ". SDL Error: " << IMG_GetError() << "\n";
        return false;
    }
    image_surface = SDL_ConvertSurface(raw_surface, screen_surface->format, 0);
    if (!image_surface) {
        cout << "Unable to optimize image " << image_name << ". SDL Error: " << SDL_GetError() << "\n";
        return false;
    }

    return true;
}

auto loadTexture(ManagedSDLTexture& texture, char const* image_name, ManagedSDLRenderer& renderer, std::optional<color> color_key={}) -> bool {
    auto loaded_surface = ManagedSDLSurface{IMG_Load(image_name)};

    if (!loaded_surface) {
        cout << "Unable to load image " << image_name << ". SDL Error: " << IMG_GetError() << "\n";
        return false;
    }

    if (color_key) {
        SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, color_key->r, color_key->g, color_key->b));
    }

    texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
    if (!texture) {
        cout << "Unable to create texture from " << image_name << ". SDL Error: " << SDL_GetError() << "\n";
        return false;
    }

    return true;
}
