#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <utility>
#include <optional>

#include "helpers/ManagedSDLWindow.hpp"

using window_ptr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;

ManagedSDLWindow::ManagedSDLWindow(): window_(nullptr, SDL_DestroyWindow) {}

ManagedSDLWindow::ManagedSDLWindow(SDL_Window* window): window_(window, SDL_DestroyWindow) {}

auto ManagedSDLWindow::operator=(SDL_Window* window) -> ManagedSDLWindow& {
    window_ = window_ptr{window, SDL_DestroyWindow};
    return *this;
}

ManagedSDLWindow::operator SDL_Window*() { return window_.get(); }

ManagedSDLWindow::operator bool() const { return window_ != nullptr; }

auto ManagedSDLWindow::operator->() -> SDL_Window*  { return window_.get(); }
