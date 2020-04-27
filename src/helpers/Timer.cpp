#include <SDL2/SDL.h>
#include <limits>

#include "helpers/Timer.hpp"

auto Timer::update() -> void {
    if (!paused_) {
        time_ = std::min(SDL_GetTicks(), stop_time_);
    }
}

auto Timer::paused() -> bool {
    return paused_;
}

auto Timer::stopTime() -> Uint32 { return stop_time_; }
auto Timer::remaining()  -> Uint32 {
    update();
    return stop_time_ - time_;
}
auto Timer::finished() -> bool {
    update();
    return time_ >= stop_time_;
}

auto Timer::elapsed()  -> Uint32 {
    update();
    return time_ - begin_time_;
}

Timer::Timer() {
    time_ = SDL_GetTicks();
    begin_time_ = time_;
    stop_time_ = std::numeric_limits<decltype(stop_time_)>::max();
}
Timer::Timer(Uint32 duration) {
    time_ = SDL_GetTicks();
    begin_time_ = time_;
    duration_ = duration;
    stop_time_ = time_ + *duration_;
}

auto Timer::pause() -> void {
    if (!paused_) {
        update();
        paused_ = true;
    }
}
auto Timer::unpause() -> void {
    if (paused_) {
        auto time = SDL_GetTicks();
        auto diff = time - time_;
        time_ = time;

        if (duration_) {
            stop_time_ += diff;
        }

        begin_time_ += diff;
        paused_ = false;
    }
}

auto Timer::reset(bool pause)    -> void {
    paused_ = pause;
    time_ = SDL_GetTicks();
    begin_time_ = time_;
    if (duration_) {
        stop_time_ = time_ + *duration_;
    } else {
        stop_time_ = std::numeric_limits<decltype(stop_time_)>::max();
    }
}
