#ifndef HELPERS_HELPERS_HPP_
#define HELPERS_HELPERS_HPP_

#include <optional>

#include "ManagedSDLRenderer.hpp"
#include "ManagedSDLSurface.hpp"
#include "ManagedSDLTexture.hpp"
#include "ManagedSDLWindow.hpp"
#include "ManagedTTFFont.hpp"


auto loadSurface(char const*, ManagedSDLSurface&) -> SDL_Surface*;
auto loadTextureFromFile(ManagedSDLRenderer&, char const*, std::optional<SDL_Colour>color_key={}) -> SDL_Texture*;
auto loadTextureFromText(ManagedSDLRenderer&, char const*, ManagedTTFFont&, SDL_Colour colour={0, 0, 0, 0}) -> SDL_Texture*;
auto loadFont(char const*, int) -> TTF_Font*;
auto init() -> bool;

#endif
