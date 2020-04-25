#ifndef HELPERS_MANAGEDTTFFONT_HPP_
#define HELPERS_MANAGEDTTFFONT_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>

struct ManagedTTFFont {
    using font_ptr = std::unique_ptr<TTF_Font, void(*)(TTF_Font*)>;

    font_ptr font_;

    ManagedTTFFont();
    explicit ManagedTTFFont(TTF_Font* ttf_font);
    explicit ManagedTTFFont(ManagedTTFFont& other);
    explicit ManagedTTFFont(ManagedTTFFont&& other);

    auto operator=(TTF_Font* ttf_font) -> ManagedTTFFont&;
    auto operator=(ManagedTTFFont& other) -> ManagedTTFFont&;
    auto operator=(ManagedTTFFont&& other) -> ManagedTTFFont&;

    operator TTF_Font*();
    explicit operator bool() const;
    auto operator->() -> TTF_Font*;
};

#endif
