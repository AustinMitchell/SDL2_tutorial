#ifndef HELPERS_MANAGEDRESOURCE_HPP_
#define HELPERS_MANAGEDRESOURCE_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <utility>

template<typename T>
static void nothing(T*) {}

template<typename Resource, void (*freeResource)(Resource*)>
struct ManagedResource {
    using resource_ptr = std::unique_ptr<Resource, decltype(freeResource)>;

    resource_ptr resource_;

    ManagedResource():                                  resource_(nullptr, nothing<Resource>) {}
    explicit ManagedResource(Resource* resource):       resource_(resource, freeResource) {}
    explicit ManagedResource(ManagedResource&  other):  resource_(other, nothing<Resource>) {}
    explicit ManagedResource(ManagedResource&& other):  resource_(std::move(other.resource_)) {
        other.resource_.get_deleter() = nothing<Resource>;
    }

    auto operator=(Resource* resource) -> ManagedResource& {
        resource_ = resource_ptr{resource, freeResource};
        return *this;
    };
    auto operator=(ManagedResource& other) -> ManagedResource& {
        resource_ = resource_ptr{other, nothing<Resource>};
        return *this;
    }
    auto operator=(ManagedResource&& other) -> ManagedResource& {
        resource_.swap(other.resource_);
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

#endif
