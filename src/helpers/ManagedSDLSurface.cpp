#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <utility>
#include <optional>

#include <SDL_helpers.hpp>

using surface_ptr = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>;

ManagedSDLSurface::ManagedSDLSurface(): surface_(nullptr, SDL_FreeSurface) {}

ManagedSDLSurface::ManagedSDLSurface(SDL_Surface* surface): surface_(surface, SDL_FreeSurface) {}

auto ManagedSDLSurface::operator=(SDL_Surface* surface) -> ManagedSDLSurface& {
    surface_ = surface_ptr{surface, SDL_FreeSurface};
    return *this;
}

ManagedSDLSurface::operator SDL_Surface*() { return surface_.get(); }

ManagedSDLSurface::operator bool() { return surface_ != nullptr; }

auto ManagedSDLSurface::operator->() -> SDL_Surface*  { return surface_.get(); }
