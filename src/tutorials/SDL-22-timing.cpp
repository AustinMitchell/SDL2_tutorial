// Copyright 2020 Nathaniel Mitchell

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <memory>
#include <utility>
#include <functional>
#include <optional>

#include <chrono>
#include <thread>
#include <sstream>

#include "SDL_helpers.hpp"
#include "SDL_components.hpp"

using std::cout;


// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


struct ProgramData {
    ManagedSDLWindow    window;
    ManagedSDLSurface   screen_surface;
    ManagedSDLRenderer  renderer;

    ManagedSDLTexture   texture_time;
    ManagedSDLTexture   texture_prompt;

    ManagedTTFFont      font;
};


auto run() -> bool;
auto loadData(ProgramData&) -> bool;


int main() {
    run();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}


auto run() -> bool {
    auto data       = ProgramData{};
    auto event      = SDL_Event{};
    auto quit       = false;
    auto last_frame = Uint32{0};

    auto black      = SDL_Colour{0, 0, 0, 0xff};
    auto start_time = Uint32{0};
    auto time_text  = std::stringstream{};

    if (!init()) {
        cout << "Failed to initialize.\n";
        return false;
    }

    if (!loadData(data)) {
        cout << "Failed to load data.\n";
        return false;
    }

    auto clip_prompt = SDL_Rect{(SCREEN_WIDTH-data.texture_prompt.clip().w) / 2,
                                0,
                                data.texture_prompt.clip().w,
                                data.texture_prompt.clip().h};

    auto clip_time   = SDL_Rect{(SCREEN_WIDTH-data.texture_time.clip().w) / 2,
                                (SCREEN_HEIGHT-data.texture_time.clip().h) / 2,
                                data.texture_time.clip().w,
                                data.texture_time.clip().h};

    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&event) != 0) {
            //  User requests quit
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                start_time = SDL_GetTicks();
                // cout << "keydown: " << start_time << "\n";

            }

            mouse::update(event);
        }

        time_text.str("");
        time_text << "Milliseconds since start time " << SDL_GetTicks() - start_time;

        // Render text
        data.texture_time = loadTextureFromText(data.renderer, time_text.str().c_str(), data.font, black);
        if (!data.texture_time) {
            cout << "Unable to render time texture.\n";
        }

        clip_time = SDL_Rect{(SCREEN_WIDTH-data.texture_time.clip().w) / 2,
                             (SCREEN_HEIGHT-data.texture_time.clip().h) / 2,
                             data.texture_time.clip().w,
                             data.texture_time.clip().h};

        // Clear screen
        SDL_SetRenderDrawColor(data.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(data.renderer);

        data.texture_prompt.render(data.renderer, &clip_prompt);
        data.texture_time.render(data.renderer, &clip_time);

        // Update screen
        SDL_RenderPresent(data.renderer);

        auto current_frame = SDL_GetTicks();
        std::this_thread::sleep_for(std::chrono::milliseconds(std::max(0, 30-static_cast<int>(current_frame-last_frame))));
        last_frame = current_frame;
    }

    return true;
}


auto loadData(ProgramData& data) -> bool {
    // Create window
    data.window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!data.window) {
        cout << "Window could not be created. SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    // create renderer
    data.renderer = SDL_CreateRenderer(data.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!data.renderer) {
        cout << "Renderer could not be created. SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    // Initialize renderer color
    SDL_SetRenderDrawColor(data.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Get window surface
    data.screen_surface = SDL_GetWindowSurface(data.window);

    data.font = loadFont("fonts/lazy.ttf", 28);

    data.texture_prompt = loadTextureFromText(data.renderer, "Press Enter to Reset Start Time.", data.font, SDL_Colour{0, 0, 0, 0xff});
    if (!data.texture_prompt) { return false; }

    return true;
}
