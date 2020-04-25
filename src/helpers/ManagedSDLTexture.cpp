#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <utility>
#include <optional>
#include <iostream>

#include "helpers/ManagedSDLTexture.hpp"

using texture_ptr = std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>;


static void nothing(SDL_Texture*) {}
static void deleter(SDL_Texture* tex) {
    SDL_DestroyTexture(tex);
}

ManagedSDLTexture::ManagedSDLTexture(): texture_(nullptr, nothing) {}

ManagedSDLTexture::ManagedSDLTexture(SDL_Texture* sdl_texture, std::optional<SDL_Rect> source_clip): texture_(sdl_texture, deleter) {
    if (source_clip) {
        src_clip_ = *source_clip;
    } else {
        src_clip_.x = 0;
        src_clip_.y = 0;
        SDL_QueryTexture(sdl_texture, nullptr, nullptr, &src_clip_.w, &src_clip_.h);
    }
}

ManagedSDLTexture::ManagedSDLTexture(ManagedSDLTexture& other, std::optional<SDL_Rect> source_clip): texture_(other, nothing) {
    if (source_clip) {
        src_clip_ = *source_clip;
    } else {
        src_clip_ = other.src_clip_;
    }
}


ManagedSDLTexture::ManagedSDLTexture(ManagedSDLTexture&& other, std::optional<SDL_Rect> source_clip): texture_(std::move(other.texture_)) {
    other.texture_.get_deleter() = nothing;
    if (source_clip) {
        src_clip_ = *source_clip;
    } else {
        src_clip_ = other.src_clip_;
    }
}


auto ManagedSDLTexture::operator=(SDL_Texture* sdl_texture) -> ManagedSDLTexture& {
    texture_ = texture_ptr{sdl_texture, deleter};
    src_clip_.x = 0;
    src_clip_.y = 0;
    SDL_QueryTexture(sdl_texture, nullptr, nullptr, &src_clip_.w, &src_clip_.h);
    return *this;
}

auto ManagedSDLTexture::operator=(ManagedSDLTexture& other) -> ManagedSDLTexture& {
    texture_ = texture_ptr{other, nothing};
    src_clip_ = other.src_clip_;
    return *this;
}

auto ManagedSDLTexture::operator=(ManagedSDLTexture&& other) -> ManagedSDLTexture& {
    texture_.swap(other.texture_);
    src_clip_ = other.src_clip_;
    return *this;
}

auto ManagedSDLTexture::baseDim() const -> SDL_Point {
    SDL_Point dim;
    SDL_QueryTexture(*this, nullptr, nullptr, &dim.x, &dim.y);
    return dim;
}

auto ManagedSDLTexture::clip() const -> SDL_Rect const& { return src_clip_; }
auto ManagedSDLTexture::clipPos() const -> SDL_Point { return {src_clip_.x, src_clip_.y}; }
auto ManagedSDLTexture::clipDim() const -> SDL_Point { return {src_clip_.w, src_clip_.h}; }

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

ManagedSDLTexture::operator SDL_Texture*() { return texture_.get(); }
ManagedSDLTexture::operator SDL_Texture*() const { return texture_.get(); }

ManagedSDLTexture::operator bool() const { return texture_ != nullptr; }

auto ManagedSDLTexture::operator->() -> SDL_Texture* { return texture_.get(); }
