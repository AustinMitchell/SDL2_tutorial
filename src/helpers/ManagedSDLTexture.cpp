#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <utility>
#include <optional>

#include <SDL_helpers.hpp>

using texture_ptr = std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>;

ManagedSDLTexture::ManagedSDLTexture(): texture_(nullptr, SDL_DestroyTexture) {}

ManagedSDLTexture::ManagedSDLTexture(SDL_Texture* texture): texture_(texture, SDL_DestroyTexture) {
    SDL_QueryTexture(texture, nullptr, nullptr, &render_quad.w, &render_quad.h);
}

auto ManagedSDLTexture::operator=(SDL_Texture* texture) -> ManagedSDLTexture& {
    texture_ = texture_ptr{texture, SDL_DestroyTexture};
    SDL_QueryTexture(texture, nullptr, nullptr, &render_quad.w, &render_quad.h);
    return *this;
}

auto ManagedSDLTexture::width()  -> int { return render_quad.w; }
auto ManagedSDLTexture::height() -> int { return render_quad.h; }

auto ManagedSDLTexture::setColour(SDL_Colour const& c) -> void {
    SDL_SetTextureColorMod(*this, c.r, c.g, c.b);
}

auto ManagedSDLTexture::setBlendMode(SDL_BlendMode blending) -> void {
    SDL_SetTextureBlendMode(*this, blending);
}

auto ManagedSDLTexture::setAlpha(uint8_t alpha) -> void {
    SDL_SetTextureAlphaMod(*this, alpha);
}

auto ManagedSDLTexture::render(ManagedSDLRenderer& renderer,
                               int x,
                               int y,
                               SDL_Rect* clip,
                               double angle,
                               SDL_Point* center,
                               SDL_RendererFlip flip) -> void {
    render_quad.x = x;
    render_quad.y = y;

    if (clip) {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }

    SDL_RenderCopyEx(renderer, *this, clip, &render_quad, angle, center, flip);
}
auto ManagedSDLTexture::render(ManagedSDLRenderer& renderer, int x, int y, SDL_RendererFlip flip) -> void {
    render(renderer, x, y, nullptr, 0.0, nullptr, flip);
}
auto ManagedSDLTexture::render(ManagedSDLRenderer& renderer, int x, int y, double angle, SDL_Point* center) -> void {
    render(renderer, x, y, nullptr, angle, center, SDL_FLIP_NONE);
}
auto ManagedSDLTexture::render(ManagedSDLRenderer& renderer, int x, int y, SDL_Rect* clip) -> void {
    render(renderer, x, y, clip, 0.0, nullptr, SDL_FLIP_NONE);
}
auto ManagedSDLTexture::render(ManagedSDLRenderer& renderer, int x, int y) -> void {
    render(renderer, x, y, nullptr, 0.0, nullptr, SDL_FLIP_NONE);
}

ManagedSDLTexture::operator SDL_Texture*() { return texture_.get(); }

ManagedSDLTexture::operator bool() { return texture_ != nullptr; }

auto ManagedSDLTexture::operator->() -> SDL_Texture* { return texture_.get(); }
