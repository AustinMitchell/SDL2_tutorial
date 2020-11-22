#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <utility>

template<typename T>
static void nothing(T*) {}

/** Wraps a heap allocated object around a shared pointer with a custom deleter */
template<typename Resource, void (*freeResource)(Resource*)>
struct ManagedResource {
    using resource_ptr = std::shared_ptr<Resource>;

    resource_ptr resource_;

    ManagedResource(): resource_(nullptr,  nothing<Resource>) {}

    explicit ManagedResource(Resource* resource): resource_(resource, freeResource) {}

    template<typename ManagedResource_T>
    explicit ManagedResource(ManagedResource_T&&  other): resource_(other.resource_) {}

    auto operator=(Resource* resource) -> ManagedResource& {
        resource_ = resource_ptr{resource, freeResource};
        return *this;
    };

    template<typename ManagedResource_T>
    auto operator=(ManagedResource_T&& other) -> ManagedResource& {
        resource_ = other.resource_;
        return *this;
    }

    operator Resource*()       { return resource_.get(); }
    operator Resource*() const { return resource_.get(); }

    explicit operator bool() const { return !!resource_; }

    auto operator->() -> Resource* { return resource_.get(); }
};

using ManagedSDLWindow      = ManagedResource<SDL_Window,   SDL_DestroyWindow>;
using ManagedSDLRenderer    = ManagedResource<SDL_Renderer, SDL_DestroyRenderer>;
using ManagedSDLSurface     = ManagedResource<SDL_Surface,  SDL_FreeSurface>;
using ManagedTTFFont        = ManagedResource<TTF_Font,     TTF_CloseFont>;
using ManagedMixChunk       = ManagedResource<Mix_Chunk,    Mix_FreeChunk>;
using ManagedMixMusic       = ManagedResource<Mix_Music,    Mix_FreeMusic>;
