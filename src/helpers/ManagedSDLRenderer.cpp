#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <utility>
#include <optional>

#include "helpers/ManagedSDLRenderer.hpp"

using renderer_ptr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;

ManagedSDLRenderer::ManagedSDLRenderer(): renderer_(nullptr, SDL_DestroyRenderer) {}

ManagedSDLRenderer::ManagedSDLRenderer(SDL_Renderer* renderer): renderer_(renderer, SDL_DestroyRenderer) {}

auto ManagedSDLRenderer::operator=(SDL_Renderer* renderer) -> ManagedSDLRenderer& {
    renderer_ = renderer_ptr{renderer, SDL_DestroyRenderer};
    return *this;
}

ManagedSDLRenderer::operator SDL_Renderer*() { return renderer_.get(); }

ManagedSDLRenderer::operator bool() const { return renderer_ != nullptr; }

auto ManagedSDLRenderer::operator->() -> SDL_Renderer*  { return renderer_.get(); }
