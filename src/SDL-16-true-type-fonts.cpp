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
auto init(ProgramData&) -> bool;
auto loadMedia(ProgramData&) -> bool;
auto loadSurface(ManagedSDLSurface&, char const*, ManagedSDLSurface&) -> bool;
auto loadTextureFromFile(ManagedSDLTexture&, ManagedSDLRenderer&, char const*, std::optional<SDL_Colour>) -> bool;
auto loadTextureFromText(ManagedSDLTexture&, ManagedSDLRenderer&, char const*, ManagedTTFFont&, SDL_Colour) -> bool;
auto loadFont(ManagedTTFFont&, char const*, int) -> bool;


int main() {
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

        // Render current frame
        data.texture.render(data.renderer, (SCREEN_WIDTH - data.texture.width()) / 2, (SCREEN_HEIGHT - data.texture.height()) / 2);

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

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        cout << "SDL_ttf could not initialize. SDL_ttf Error: " << TTF_GetError() << "\n";
        return false;
    }

    // Get window surface
    data.screen_surface = SDL_GetWindowSurface(data.window);

    return true;
}


auto loadMedia(ProgramData& data) -> bool {
    bool success;

    auto black = SDL_Colour{0, 0, 0, 0};

    success = loadFont(data.font, "fonts/lazy.ttf", 28);
    if (!success) { return false; }

    success = loadTextureFromText(data.texture, data.renderer, "The quick brown fox jumps over the lazy dog", data.font, black);
    if (!success) { return false; }

    return true;
}


auto loadSurface(ManagedSDLSurface& image_surface, char const* image_name, ManagedSDLSurface& screen_surface) -> bool {
    auto raw_surface    = ManagedSDLSurface{IMG_Load(image_name)};

    if (!raw_surface) {
        cout << "Unable to load image " << image_name << ". SDL_image Error: " << IMG_GetError() << "\n";
        return false;
    }
    image_surface = SDL_ConvertSurface(raw_surface, screen_surface->format, 0);
    if (!image_surface) {
        cout << "Unable to optimize image " << image_name << ". SDL Error: " << SDL_GetError() << "\n";
        return false;
    }

    return true;
}


auto loadTextureFromFile(ManagedSDLTexture& texture, ManagedSDLRenderer& renderer, char const* image_name, std::optional<SDL_Colour> color_key={}) -> bool {
    auto loaded_surface = ManagedSDLSurface{IMG_Load(image_name)};

    if (!loaded_surface) {
        cout << "Unable to load image " << image_name << ". SDL_image Error: " << IMG_GetError() << "\n";
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


auto loadTextureFromText(ManagedSDLTexture& texture, ManagedSDLRenderer& renderer, char const* string_to_render, ManagedTTFFont& font, SDL_Colour colour={0, 0, 0, 0}) -> bool {
    auto loaded_surface = ManagedSDLSurface{TTF_RenderText_Solid(font, string_to_render, colour)};

    if (!loaded_surface) {
        cout << "Unable to render text to surface. SDL_ttf Error: " << TTF_GetError() << "\n";
        return false;
    }

    texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
    if (!texture) {
        cout << "Unable to create texture from rendered text. SDL Error: " << SDL_GetError() << "\n";
        return false;
    }

    return true;
}


auto loadFont(ManagedTTFFont& font, char const* font_name, int size) -> bool {
    font = TTF_OpenFont(font_name, size);

    if (!font) {
        cout << "Unable to load font " << font_name << ". SDL_ttf Error: " << TTF_GetError() << "\n";
        return false;
    }

    return true;
}
