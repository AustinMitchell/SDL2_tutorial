#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <utility>
#include <optional>
#include <iostream>

#include "helpers/ManagedSDLTexture.hpp"


ManagedSDLTexture::ManagedSDLTexture(): ManagedResource() {}

ManagedSDLTexture::ManagedSDLTexture(SDL_Texture* sdl_texture, std::optional<SDL_Rect> source_clip): ManagedResource(sdl_texture) {
    if (source_clip) {
        src_clip_ = *source_clip;
    } else {
        src_clip_.x = 0;
        src_clip_.y = 0;
        SDL_QueryTexture(sdl_texture, nullptr, nullptr, &src_clip_.w, &src_clip_.h);
    }
}


auto ManagedSDLTexture::operator=(SDL_Texture* sdl_texture) -> ManagedSDLTexture& {
    ManagedResource::operator=(sdl_texture);
    src_clip_.x = 0;
    src_clip_.y = 0;
    SDL_QueryTexture(sdl_texture, nullptr, nullptr, &src_clip_.w, &src_clip_.h);
    return *this;
}


auto ManagedSDLTexture::baseDim() const -> SDL_Point {
    SDL_Point dim;
    SDL_QueryTexture(*this, nullptr, nullptr, &dim.x, &dim.y);
    return dim;
}

auto ManagedSDLTexture::rect() const -> SDL_Rect const& { return src_clip_; }
auto ManagedSDLTexture::pos() const -> SDL_Point { return {src_clip_.x, src_clip_.y}; }
auto ManagedSDLTexture::dim() const -> SDL_Point { return {src_clip_.w, src_clip_.h}; }

auto ManagedSDLTexture::setClip(SDL_Rect const& rect) -> ManagedSDLTexture& { src_clip_ = rect; return *this; }
auto ManagedSDLTexture::setClipPos(SDL_Point const& pos) -> ManagedSDLTexture& { src_clip_.x = pos.x; src_clip_.y = pos.y; return *this; }
auto ManagedSDLTexture::setClipDim(SDL_Point const& dim) -> ManagedSDLTexture& { src_clip_.w = dim.x; src_clip_.h = dim.y; return *this; }

auto ManagedSDLTexture::setColour(SDL_Colour const& c) -> ManagedSDLTexture& {
    SDL_SetTextureColorMod(*this, c.r, c.g, c.b);
    return *this;
}

auto ManagedSDLTexture::setBlendMode(SDL_BlendMode blending) -> ManagedSDLTexture& {
    SDL_SetTextureBlendMode(*this, blending);
    return *this;
}

auto ManagedSDLTexture::setAlpha(uint8_t alpha) -> ManagedSDLTexture& {
    SDL_SetTextureAlphaMod(*this, alpha);
    return *this;
}

auto ManagedSDLTexture::render(SDL_Renderer* renderer,
                               SDL_Rect* clip,
                               double angle,
                               SDL_Point* center,
                               SDL_RendererFlip flip) -> void {
    SDL_RenderCopyEx(renderer, *this, &src_clip_, clip, angle, center, flip);
}
auto ManagedSDLTexture::render(SDL_Renderer* renderer, SDL_Rect* clip, SDL_RendererFlip flip) -> void {
    render(renderer, clip, 0.0, nullptr, flip);
}
auto ManagedSDLTexture::render(SDL_Renderer* renderer, SDL_Rect* clip, double angle, SDL_Point* center) -> void {
    render(renderer, clip, angle, center, SDL_FLIP_NONE);
}
auto ManagedSDLTexture::render(SDL_Renderer* renderer, SDL_Rect* clip) -> void {
    render(renderer, clip, 0.0, nullptr, SDL_FLIP_NONE);
}
auto ManagedSDLTexture::render(SDL_Renderer* renderer) -> void {
    render(renderer, nullptr, 0.0, nullptr, SDL_FLIP_NONE);
}
