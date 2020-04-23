#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <utility>
#include <optional>

#include <SDL_helpers.hpp>

using font_ptr = std::unique_ptr<TTF_Font, void(*)(TTF_Font*)>;

ManagedTTFFont::ManagedTTFFont(): font_(nullptr, TTF_CloseFont) {}

ManagedTTFFont::ManagedTTFFont(TTF_Font* font): font_(font, TTF_CloseFont) {}

auto ManagedTTFFont::operator=(TTF_Font* font) -> ManagedTTFFont& {
    font_ = font_ptr{font, TTF_CloseFont};
    return *this;
}

ManagedTTFFont::operator TTF_Font*() { return font_.get(); }

ManagedTTFFont::operator bool() { return font_ != nullptr; }

auto ManagedTTFFont::operator->() -> TTF_Font*  { return font_.get(); }
