#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <utility>
#include <optional>

#include "helpers/ManagedTTFFont.hpp"

using font_ptr = std::unique_ptr<TTF_Font, void(*)(TTF_Font*)>;

static void nothing(TTF_Font*) {}

ManagedTTFFont::ManagedTTFFont():                       font_(nullptr, nothing) {}
ManagedTTFFont::ManagedTTFFont(TTF_Font* ttf_font):     font_(ttf_font, TTF_CloseFont) {}
ManagedTTFFont::ManagedTTFFont(ManagedTTFFont& other):  font_(other, nothing) {}
ManagedTTFFont::ManagedTTFFont(ManagedTTFFont&& other): font_(std::move(other.font_)) {
    other.font_.get_deleter() = nothing;
}

auto ManagedTTFFont::operator=(TTF_Font* ttf_font) -> ManagedTTFFont& {
    font_ = font_ptr{ttf_font, TTF_CloseFont};
    return *this;
}
auto ManagedTTFFont::operator=(ManagedTTFFont& other) -> ManagedTTFFont& {
    font_ = font_ptr{other, nothing};
    return *this;
}
auto ManagedTTFFont::operator=(ManagedTTFFont&& other) -> ManagedTTFFont& {
    font_.swap(other.font_);
    return *this;
}

ManagedTTFFont::operator TTF_Font*() { return font_.get(); }

ManagedTTFFont::operator bool() const { return font_ != nullptr; }

auto ManagedTTFFont::operator->() -> TTF_Font*  { return font_.get(); }
