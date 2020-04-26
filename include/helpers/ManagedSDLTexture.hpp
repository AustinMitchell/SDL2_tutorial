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
    explicit ManagedSDLTexture(ManagedSDLTexture&  other, std::optional<SDL_Rect> source_clip={});
    explicit ManagedSDLTexture(ManagedSDLTexture&& other, std::optional<SDL_Rect> source_clip={});

    auto operator=(SDL_Texture* sdl_texture) -> ManagedSDLTexture&;
    auto operator=(ManagedSDLTexture& other) -> ManagedSDLTexture&;
    auto operator=(ManagedSDLTexture&& other) -> ManagedSDLTexture&;

    auto baseDim()    const -> SDL_Point;

    auto clip()    const -> SDL_Rect const&;
    auto clipPos() const -> SDL_Point;
    auto clipDim() const -> SDL_Point;

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

#endif
