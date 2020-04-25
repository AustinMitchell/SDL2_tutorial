#ifndef HELPERS_MANAGEDSDLRENDERER_HPP_
#define HELPERS_MANAGEDSDLRENDERER_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>

struct ManagedSDLRenderer {
    using renderer_ptr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;

    renderer_ptr renderer_;

    ManagedSDLRenderer();
    explicit ManagedSDLRenderer(SDL_Renderer* renderer);

    auto operator=(SDL_Renderer* renderer) -> ManagedSDLRenderer&;

    operator SDL_Renderer*();
    explicit operator bool() const;
    auto operator->() -> SDL_Renderer*;
};

#endif
