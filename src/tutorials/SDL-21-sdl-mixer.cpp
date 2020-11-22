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

#include <chrono> // NOLINT [build/c++11]
#include <thread> // NOLINT [build/c++11]

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
    ManagedSDLTexture   background;

    ManagedMixMusic music;

    ManagedMixChunk sound_scratch;
    ManagedMixChunk sound_high;
    ManagedMixChunk sound_medium;
    ManagedMixChunk sound_low;
};


auto run() -> bool;
auto loadData(ProgramData&) -> bool;


int main(__attribute__((unused))int argc, __attribute__((unused))char *argv[]) {
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

    if (!init()) {
        cout << "Failed to initialize.\n";
        return false;
    }

    if (!loadData(data)) {
        cout << "Failed to load data.\n";
        return false;
    }

    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&event) != 0) {
            //  User requests quit
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    // Play high sound effect
                    case SDLK_1:
                        Mix_PlayChannel(-1, data.sound_high, 0);
                        break;

                    // Play medium sound effect
                    case SDLK_2:
                        Mix_PlayChannel(-1, data.sound_medium, 0);
                        break;

                    // Play low sound effect
                    case SDLK_3:
                        Mix_PlayChannel(-1, data.sound_low, 0);
                        break;

                    // Play scratch sound effect
                    case SDLK_4:
                        Mix_PlayChannel(-1, data.sound_scratch, 0);
                        break;

                    case SDLK_9:
                        // If there is no music playing
                        if (Mix_PlayingMusic() == 0) {
                            // Play the music
                            Mix_PlayMusic(data.music, -1);
                        } else {
                            // If the music is paused
                            if (Mix_PausedMusic() == 1) {
                                // Resume the music
                                Mix_ResumeMusic();
                            } else {
                                // Pause the music
                                Mix_PauseMusic();
                            }
                        }
                        break;

                    case SDLK_0:
                        // Stop the music
                        Mix_HaltMusic();
                        break;
                }
            }

            mouse::update(event);
        }

        // Clear screen
        SDL_SetRenderDrawColor(data.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(data.renderer);

        data.background.render(data.renderer);

        // Update screen
        SDL_RenderPresent(data.renderer);

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
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

    data.background = loadTextureFromFile(data.renderer, "images/t21/prompt.png");
    if (!data.background) { return false; }

    data.music = Mix_LoadMUS("sounds/t21/beat.wav");
    if (!data.music) { return false; }

    data.sound_scratch = Mix_LoadWAV("sounds/t21/scratch.wav");
    if (!data.sound_scratch) { return false; }

    data.sound_high = Mix_LoadWAV("sounds/t21/high.wav");
    if (!data.sound_high) { return false; }

    data.sound_medium = Mix_LoadWAV("sounds/t21/medium.wav");
    if (!data.sound_medium) { return false; }

    data.sound_low = Mix_LoadWAV("sounds/t21/low.wav");
    if (!data.sound_low) { return false; }


    return true;
}
