// Copyright 2020 Nathaniel Mitchell

#include <SDL2/SDL.h>
#include <iostream>
#include <memory>

#include "SDL_helpers.hpp"


using std::cout;


// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


auto run() -> bool;
auto init(ManagedSDLWindow&, ManagedSDLSurface&) -> bool;
auto loadMedia(ManagedSDLSurface&, std::string&) -> bool;


int main(__attribute__((unused))int argc, __attribute__((unused))char *argv[]) {
    run();
    SDL_Quit();
    return 0;
}


auto run() -> bool {
    auto window         = ManagedSDLWindow{};
    auto screen_surface = ManagedSDLSurface{};
    auto hello_image    = ManagedSDLSurface{};
    auto event          = SDL_Event{};
    auto image_name     = std::string{"images/t03/exit.bmp"};
    auto quit           = false;

    if (!init(window, screen_surface)) {
        cout << "Failed to initialize.\n";
        return false;
    }

    if (!loadMedia(hello_image, image_name)) {
        cout << "Failed to load media.\n";
        return false;
    }

    while (!quit) {
        //  Handle events on queue
        while (SDL_PollEvent(&event) != 0) {
            //  User requests quit
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        SDL_BlitSurface(hello_image, nullptr, screen_surface, nullptr);
        SDL_UpdateWindowSurface(window);
    }

    return true;
}


auto init(ManagedSDLWindow& window, ManagedSDLSurface& screen_surface) -> bool {
    //  Initialization flag
    bool success = true;

    //  Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        success = false;
    } else {
        //  Create window
        window = SDL_CreateWindow("SDL Tutorial",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH,
                                  SCREEN_HEIGHT,
                                  SDL_WINDOW_SHOWN);
        if (!window) {
            cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
            success = false;
        } else {
            //  Get window surface
            screen_surface = SDL_GetWindowSurface(window);
        }
    }

    return success;
}


bool loadMedia(ManagedSDLSurface& image_surface, std::string& image_name) {
    //  Loading success flag
    bool success = true;

    //  Load splash image
    image_surface = SDL_LoadBMP(image_name.c_str());
    if (!image_surface) {
        cout << "Unable to load image " << image_name << ". SDL Error: " << SDL_GetError() << "\n";
        success = false;
    }

    return success;
}
