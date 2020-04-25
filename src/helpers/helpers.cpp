#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <utility>
#include <optional>
#include <iostream>

#include <SDL_helpers.hpp>

using std::cout;

auto loadSurface(char const* image_name, ManagedSDLSurface& screen_surface) -> SDL_Surface* {
    SDL_Surface *image_surface = {};
    auto raw_surface = ManagedSDLSurface{IMG_Load(image_name)};

    if (!raw_surface) {
        cout << "Unable to load image " << image_name << ". SDL_image Error: " << IMG_GetError() << "\n";
        return {};
    }
    image_surface = SDL_ConvertSurface(raw_surface, screen_surface->format, 0);
    if (!image_surface) {
        cout << "Unable to optimize image " << image_name << ". SDL Error: " << SDL_GetError() << "\n";
        return {};
    }

    return image_surface;
}


auto loadTextureFromFile(ManagedSDLRenderer& renderer, char const* image_name, std::optional<SDL_Colour> color_key) -> SDL_Texture* {
    SDL_Texture *texture = {};
    auto loaded_surface = ManagedSDLSurface{IMG_Load(image_name)};

    if (!loaded_surface) {
        cout << "Unable to load image " << image_name << ". SDL_image Error: " << IMG_GetError() << "\n";
        return {};
    }

    if (color_key) {
        SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, color_key->r, color_key->g, color_key->b));
    }

    texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
    if (!texture) {
        cout << "Unable to create texture from " << image_name << ". SDL Error: " << SDL_GetError() << "\n";
        return {};
    }

    return texture;
}


auto loadTextureFromText(ManagedSDLRenderer& renderer, char const* string_to_render, ManagedTTFFont& font, SDL_Colour colour) -> SDL_Texture* {
    SDL_Texture *texture = {};
    auto loaded_surface = ManagedSDLSurface{TTF_RenderText_Solid(font, string_to_render, colour)};

    if (!loaded_surface) {
        cout << "Unable to render text to surface. SDL_ttf Error: " << TTF_GetError() << "\n";
        return {};
    }

    texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
    if (!texture) {
        cout << "Unable to create texture from rendered text. SDL Error: " << SDL_GetError() << "\n";
        return {};
    }

    return texture;
}


auto loadFont(char const* font_name, int size) -> TTF_Font* {
    auto font = TTF_OpenFont(font_name, size);

    if (!font) {
        cout << "Unable to load font " << font_name << ". SDL_ttf Error: " << TTF_GetError() << "\n";
        return {};
    }

    return font;
}


auto init() -> bool {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize. SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

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

    cout << "SDL initialized successfully\n";

    return true;
}
