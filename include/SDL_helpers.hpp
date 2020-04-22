// Copyright 2020 Nathaniel Mitchell

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <utility>
#include <optional>

struct ManagedSDLWindow;
struct ManagedSDLSurface;
struct ManagedSDLTexture;
struct ManagedSDLRenderer;

struct ManagedSDLWindow {
    using window_ptr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;

    window_ptr window_;

    ManagedSDLWindow(): window_(nullptr, SDL_DestroyWindow) {}
    explicit ManagedSDLWindow(SDL_Window* window): window_(window, SDL_DestroyWindow) {}

    auto operator=(SDL_Window* window) -> ManagedSDLWindow& {
        window_ = window_ptr{window, SDL_DestroyWindow};
        return *this;
    }

    operator SDL_Window*() { return window_.get(); }
    explicit operator bool() { return window_ != nullptr; }
    SDL_Window* operator->() { return window_.get(); }
};

struct ManagedSDLSurface {
    using surface_ptr = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>;

    surface_ptr surface_;

    ManagedSDLSurface(): surface_(nullptr, SDL_FreeSurface) {}
    explicit ManagedSDLSurface(SDL_Surface* surface): surface_(surface, SDL_FreeSurface) {}

    auto operator=(SDL_Surface* surface) -> ManagedSDLSurface& {
        surface_ = surface_ptr{surface, SDL_FreeSurface};
        return *this;
    }

    operator SDL_Surface*() { return surface_.get(); }
    explicit operator bool() { return surface_ != nullptr; }
    SDL_Surface* operator->() { return surface_.get(); }
};

struct ManagedSDLRenderer {
    using renderer_ptr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;

    renderer_ptr renderer_;

    ManagedSDLRenderer(): renderer_(nullptr, SDL_DestroyRenderer) {}
    explicit ManagedSDLRenderer(SDL_Renderer* renderer): renderer_(renderer, SDL_DestroyRenderer) {}

    auto operator=(SDL_Renderer* renderer) -> ManagedSDLRenderer& {
        renderer_ = renderer_ptr{renderer, SDL_DestroyRenderer};
        return *this;
    }

    operator SDL_Renderer*() { return renderer_.get(); }
    explicit operator bool() { return renderer_ != nullptr; }
    SDL_Renderer* operator->() { return renderer_.get(); }
};

struct ManagedSDLTexture {
    using texture_ptr = std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>;

    SDL_Rect    render_quad;
    texture_ptr texture_;

    ManagedSDLTexture(): texture_(nullptr, SDL_DestroyTexture) {}
    explicit ManagedSDLTexture(SDL_Texture* texture): texture_(texture, SDL_DestroyTexture) {
        SDL_QueryTexture(texture, nullptr, nullptr, &render_quad.w, &render_quad.h);
    }

    auto operator=(SDL_Texture* texture) -> ManagedSDLTexture& {
        texture_ = texture_ptr{texture, SDL_DestroyTexture};
        SDL_QueryTexture(texture, nullptr, nullptr, &render_quad.w, &render_quad.h);
        return *this;
    }

    auto width()  -> int { return render_quad.w; }
    auto height() -> int { return render_quad.h; }

    auto render(ManagedSDLRenderer& renderer, int x, int y, SDL_Rect* clip = nullptr) {
        render_quad.x = x;
        render_quad.y = y;

        if (clip) {
            render_quad.w = clip->w;
            render_quad.h = clip->h;
        }

        SDL_RenderCopy(renderer, *this, clip, &render_quad);
    }

    operator SDL_Texture*() { return texture_.get(); }

    explicit operator bool() { return texture_ != nullptr; }

    auto operator->() -> SDL_Texture* { return texture_.get(); }
};
