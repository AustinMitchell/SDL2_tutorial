#ifndef HELPERS_MANAGEDSDLTEXTURE_HPP_
#define HELPERS_MANAGEDSDLTEXTURE_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <utility>
#include <optional>

#include "ManagedResource.hpp"


struct ManagedSDLTexture: public ManagedResource<SDL_Texture, SDL_DestroyTexture> {
    SDL_Rect src_clip_;

    ManagedSDLTexture();
    explicit ManagedSDLTexture(SDL_Texture* sdl_texture, std::optional<SDL_Rect> source_clip={});

    template<typename ManagedSDLTexture_T>
    explicit ManagedSDLTexture(ManagedSDLTexture_T&& other, std::optional<SDL_Rect> source_clip={});

    auto operator=(SDL_Texture* sdl_texture) -> ManagedSDLTexture&;

    template<typename ManagedSDLTexture_T>
    auto operator=(ManagedSDLTexture_T&& other) -> ManagedSDLTexture&;

    auto baseDim()  const -> SDL_Point;

    auto rect() const -> SDL_Rect const&;
    auto pos()  const -> SDL_Point;
    auto dim()  const -> SDL_Point;

    auto setClip(SDL_Rect const&)     -> ManagedSDLTexture&;
    auto setClipPos(SDL_Point const&) -> ManagedSDLTexture&;
    auto setClipDim(SDL_Point const&) -> ManagedSDLTexture&;

    auto setColour(SDL_Colour const& c) -> ManagedSDLTexture&;
    auto setBlendMode(SDL_BlendMode blending) -> ManagedSDLTexture&;
    auto setAlpha(uint8_t alpha) -> ManagedSDLTexture&;

    auto render(SDL_Renderer* renderer, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) -> void;
    auto render(SDL_Renderer* renderer, SDL_Rect* clip, double angle, SDL_Point* center) -> void;
    auto render(SDL_Renderer* renderer, SDL_Rect* clip, SDL_RendererFlip flip) -> void;
    auto render(SDL_Renderer* renderer, SDL_Rect* clip) -> void;
    auto render(SDL_Renderer* renderer) -> void;
};

template<typename ManagedSDLTexture_T>
auto ManagedSDLTexture::operator=(ManagedSDLTexture_T&& other) -> ManagedSDLTexture& {
    ManagedResource::operator=(std::forward<ManagedSDLTexture_T>(other));
    src_clip_ = other.src_clip_;
    return *this;
}

template<typename ManagedSDLTexture_T>
ManagedSDLTexture::ManagedSDLTexture(ManagedSDLTexture_T&& other, std::optional<SDL_Rect> source_clip): ManagedResource(std::forward<ManagedSDLTexture_T>(other)) {
    if (source_clip) {
        src_clip_ = *source_clip;
    } else {
        src_clip_ = other.src_clip_;
    }
}

#endif
