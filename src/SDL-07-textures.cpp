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
    ManagedSDLTexture   texture;
};


auto run() -> bool;
auto init(ProgramData&) -> bool;
bool loadMedia(ProgramData&);
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
        //  Handle events on queue
        while (SDL_PollEvent(&event) != 0) {
            //  User requests quit
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_RenderClear(data.renderer);
        SDL_RenderCopy(data.renderer, data.texture, NULL, NULL);
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

bool loadMedia(ProgramData& data) {
    bool success = true;

    success = loadTexture(data.texture, "images/texture.png", data.renderer);
    if (!success) { return false; }

    return true;
}


bool loadSurface(ManagedSDLSurface& image_surface, char const* image_name, ManagedSDLSurface& screen_surface) {
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
