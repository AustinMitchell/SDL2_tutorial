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


// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


struct ProgramData {
    ManagedSDLWindow    window;
    ManagedSDLSurface   screen_surface;
    ManagedSDLRenderer  renderer;
};


auto run() -> bool;
auto init(ProgramData&) -> bool;
auto loadMedia(ProgramData&) -> bool;
auto loadSurface(ManagedSDLSurface&, char const*, ManagedSDLSurface&) -> bool;
auto loadTexture(ManagedSDLTexture&, char const*, ManagedSDLRenderer&) -> bool;


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
        // clear screen
        SDL_SetRenderDrawColor(data.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(data.renderer);

        // Render red filled quad
        auto fillRect = SDL_Rect{SCREEN_WIDTH/4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
        SDL_SetRenderDrawColor(data.renderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(data.renderer, &fillRect);

        // Render green outlined quad
        auto outlineRect = SDL_Rect{SCREEN_WIDTH/6, SCREEN_HEIGHT/6, SCREEN_WIDTH * 2/3, SCREEN_HEIGHT * 2/3};
        SDL_SetRenderDrawColor(data.renderer, 0x00, 0xFF, 0x00, 0xFF);
        SDL_RenderDrawRect(data.renderer, &outlineRect);

        // Draw blue horizontal line
        SDL_SetRenderDrawColor(data.renderer, 0x00, 0x00, 0xFF, 0xFF);
        SDL_RenderDrawLine(data.renderer, 0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2);

        // Draw vertical line of yellow dots
        SDL_SetRenderDrawColor(data.renderer, 0xFF, 0xFF, 0x00, 0xFF);
        for (int i=0; i<SCREEN_HEIGHT; i+=4) {
            SDL_RenderDrawPoint(data.renderer, SCREEN_WIDTH/2, i);
        }

        // display buffer
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

    // success = loadTexture(data.texture, "images/texture.png", data.renderer);
    // if (!success) { return false; }

    return true;
}


auto loadSurface(ManagedSDLSurface& image_surface, char const* image_name, ManagedSDLSurface& screen_surface) -> bool {
    auto raw_surface    = ManagedSDLSurface{IMG_Load(image_name)};

    if (!raw_surface) {
        cout << "Unable to load image " << image_name << ". SDL Error: " << IMG_GetError() << "\n";
        return false;
    } else {
        image_surface = SDL_ConvertSurface(raw_surface, screen_surface->format, 0);
        if (!image_surface) {
            cout << "Unable to optimize image " << image_name << ". SDL Error: " << SDL_GetError() << "\n";
            return false;
        }
    }

    return true;
}

auto loadTexture(ManagedSDLTexture& texture, char const* image_name, ManagedSDLRenderer& renderer) -> bool {
    auto loaded_surface = ManagedSDLSurface{IMG_Load(image_name)};

    if (!loaded_surface) {
        cout << "Unable to load image " << image_name << ". SDL Error: " << IMG_GetError() << "\n";
        return false;
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
        if (!texture) {
            cout << "Unable to create texture from " << image_name << ". SDL Error: " << SDL_GetError() << "\n";
            return false;
        }
    }

    return true;
}
