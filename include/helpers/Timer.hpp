#include <SDL2/SDL.h>

#include <optional>

class Timer {
 private:
    Uint32 time_;
    Uint32 stop_time_;
    Uint32 begin_time_;
    std::optional<Uint32> duration_;
    bool paused_;

    auto update() -> void;

 public:
    auto paused()   -> bool;
    auto finished() -> bool;
    auto stopTime() -> Uint32;
    auto remaining()  -> Uint32;
    auto elapsed()  -> Uint32;

    Timer();
    explicit Timer(Uint32 duration);

    auto pause()    -> void;
    auto unpause()  -> void;

    auto reset(bool pause=false) -> void;
};
