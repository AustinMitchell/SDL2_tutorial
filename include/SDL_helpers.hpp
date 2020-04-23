// Copyright 2020 Nathaniel Mitchell

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <utility>
#include <optional>


struct ManagedSDLWindow {
    using window_ptr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;

    window_ptr window_;

    ManagedSDLWindow();
    explicit ManagedSDLWindow(SDL_Window* window);

    auto operator=(SDL_Window* window) -> ManagedSDLWindow&;

    operator SDL_Window*();
    explicit operator bool();
    auto operator->() -> SDL_Window*;
};


struct ManagedSDLSurface {
    using surface_ptr = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>;

    surface_ptr surface_;

    ManagedSDLSurface();
    explicit ManagedSDLSurface(SDL_Surface* surface);

    auto operator=(SDL_Surface* surface) -> ManagedSDLSurface&;

    operator SDL_Surface*();
    explicit operator bool();
    auto operator->() -> SDL_Surface*;
};


struct ManagedTTFFont {
    using font_ptr = std::unique_ptr<TTF_Font, void(*)(TTF_Font*)>;

    font_ptr font_;

    ManagedTTFFont();
    explicit ManagedTTFFont(TTF_Font* font);

    auto operator=(TTF_Font* font) -> ManagedTTFFont&;

    operator TTF_Font*();
    explicit operator bool();
    auto operator->() -> TTF_Font*;
};


struct ManagedSDLRenderer {
    using renderer_ptr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;

    renderer_ptr renderer_;

    ManagedSDLRenderer();
    explicit ManagedSDLRenderer(SDL_Renderer* renderer);

    auto operator=(SDL_Renderer* renderer) -> ManagedSDLRenderer&;

    operator SDL_Renderer*();
    explicit operator bool();
    auto operator->() -> SDL_Renderer*;
};


struct ManagedSDLTexture {
    using texture_ptr = std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>;

    SDL_Rect    render_quad;
    texture_ptr texture_;

    ManagedSDLTexture();
    explicit ManagedSDLTexture(SDL_Texture* texture);

    auto operator=(SDL_Texture* texture) -> ManagedSDLTexture&;

    auto width()  -> int;
    auto height() -> int;

    auto setColour(SDL_Colour const& c) -> void;

    auto setBlendMode(SDL_BlendMode blending) -> void;

    auto setAlpha(uint8_t alpha) -> void;

    auto render(ManagedSDLRenderer& renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) -> void;
    auto render(ManagedSDLRenderer& renderer, int x, int y, SDL_RendererFlip flip) -> void;
    auto render(ManagedSDLRenderer& renderer, int x, int y, double angle, SDL_Point* center) -> void;
    auto render(ManagedSDLRenderer& renderer, int x, int y, SDL_Rect* clip) -> void;
    auto render(ManagedSDLRenderer& renderer, int x, int y) -> void;

    operator SDL_Texture*();

    explicit operator bool();

    auto operator->() -> SDL_Texture*;
};


auto loadSurface(ManagedSDLSurface&, char const*, ManagedSDLSurface&) -> bool;
auto loadTextureFromFile(ManagedSDLTexture&, ManagedSDLRenderer&, char const*, std::optional<SDL_Colour>color_key={}) -> bool;
auto loadTextureFromText(ManagedSDLTexture&, ManagedSDLRenderer&, char const*, ManagedTTFFont&, SDL_Colour colour={0, 0, 0, 0}) -> bool;
auto loadFont(ManagedTTFFont&, char const*, int) -> bool;
auto init() -> bool;
