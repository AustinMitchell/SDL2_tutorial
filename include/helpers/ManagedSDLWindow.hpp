#ifndef HELPERS_MANAGEDSDLWINDOW_HPP_
#define HELPERS_MANAGEDSDLWINDOW_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>


struct ManagedSDLWindow {
    using window_ptr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;

    window_ptr window_;

    ManagedSDLWindow();
    explicit ManagedSDLWindow(SDL_Window* window);

    auto operator=(SDL_Window* window) -> ManagedSDLWindow&;

    operator SDL_Window*();
    explicit operator bool() const;
    auto operator->() -> SDL_Window*;
};

#endif
