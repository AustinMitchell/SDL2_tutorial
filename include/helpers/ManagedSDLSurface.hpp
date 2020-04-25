#ifndef HELPERS_MANAGEDSDLSURFACE_HPP_
#define HELPERS_MANAGEDSDLSURFACE_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>

struct ManagedSDLSurface {
    using surface_ptr = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>;

    surface_ptr surface_;

    ManagedSDLSurface();
    explicit ManagedSDLSurface(SDL_Surface* sdl_surface);
    explicit ManagedSDLSurface(ManagedSDLSurface& other);
    explicit ManagedSDLSurface(ManagedSDLSurface&& other);

    auto operator=(SDL_Surface* sdl_surface) -> ManagedSDLSurface&;
    auto operator=(ManagedSDLSurface& other) -> ManagedSDLSurface&;
    auto operator=(ManagedSDLSurface&& other) -> ManagedSDLSurface&;

    operator SDL_Surface*();
    explicit operator bool() const;
    auto operator->() -> SDL_Surface*;
};

#endif
