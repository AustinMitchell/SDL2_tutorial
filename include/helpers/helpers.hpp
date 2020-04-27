#ifndef HELPERS_HELPERS_HPP_
#define HELPERS_HELPERS_HPP_

#include <optional>

#include "ManagedResource.hpp"
#include "ManagedSDLTexture.hpp"


auto loadSurface(char const*, ManagedSDLSurface&) -> SDL_Surface*;
auto loadTextureFromFile(ManagedSDLRenderer&, char const*, std::optional<SDL_Colour>color_key={}) -> SDL_Texture*;
auto loadTextureFromText(ManagedSDLRenderer&, char const*, ManagedTTFFont&, SDL_Colour const& colour={0, 0, 0, 0xff}) -> SDL_Texture*;
auto loadFont(char const*, int) -> TTF_Font*;
auto init() -> bool;

#endif
