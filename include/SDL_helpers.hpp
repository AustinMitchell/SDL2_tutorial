// Copyright 2020 Nathaniel Mitchell

#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <utility>

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
