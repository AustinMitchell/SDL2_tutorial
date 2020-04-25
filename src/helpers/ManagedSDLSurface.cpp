#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <utility>
#include <optional>

#include "helpers/ManagedSDLSurface.hpp"

using surface_ptr = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>;

static void nothing(SDL_Surface*) {}

ManagedSDLSurface::ManagedSDLSurface():                          surface_(nullptr, nothing) {}
ManagedSDLSurface::ManagedSDLSurface(SDL_Surface* sdl_surface):  surface_(sdl_surface, SDL_FreeSurface) {}
ManagedSDLSurface::ManagedSDLSurface(ManagedSDLSurface& other):  surface_(other, nothing) {}
ManagedSDLSurface::ManagedSDLSurface(ManagedSDLSurface&& other): surface_(std::move(other.surface_)) {
    other.surface_.get_deleter() = nothing;
}

auto ManagedSDLSurface::operator=(SDL_Surface* other) -> ManagedSDLSurface& {
    surface_ = surface_ptr{other, SDL_FreeSurface};
    return *this;
}
auto ManagedSDLSurface::operator=(ManagedSDLSurface& other) -> ManagedSDLSurface& {
    surface_ = surface_ptr{other, nothing};
    return *this;
}
auto ManagedSDLSurface::operator=(ManagedSDLSurface&& other) -> ManagedSDLSurface& {
    surface_.swap(other.surface_);
    return *this;
}

ManagedSDLSurface::operator SDL_Surface*() { return surface_.get(); }

ManagedSDLSurface::operator bool() const { return surface_ != nullptr; }

auto ManagedSDLSurface::operator->() -> SDL_Surface*  { return surface_.get(); }
