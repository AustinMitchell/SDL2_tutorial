#ifndef COMPONENTS_BUTTON_HPP_
#define COMPONENTS_BUTTON_HPP_

#include <utility>

#include "SDL_helpers.hpp"

class Button {
 private:

    ManagedSDLTexture texture_default_;
    ManagedSDLTexture texture_hovering_;
    ManagedSDLTexture texture_clicking_;
    ManagedSDLTexture* current_texture_;

    SDL_Rect rect_;

    bool hovering_;
    bool clicking_;
    bool clicked_;

 public:
    template<typename ManagedSDLTexture_T>
    Button(ManagedSDLTexture_T&& texture_default,
           ManagedSDLTexture_T&& texture_hovering,
           ManagedSDLTexture_T&& texture_clicking,
           SDL_Rect const& area);

    auto isHovering() -> bool;
    auto isClicking() -> bool;
    auto isClicked()  -> bool;

    auto pos()  const -> SDL_Point;
    auto dim()  const -> SDL_Point;
    auto rect() const -> SDL_Rect const&;

    auto setPos(SDL_Point pos) -> void;
    auto setDim(SDL_Point dim) -> void;
    auto setRect(SDL_Rect const& rect) -> void;

    auto update() -> void;

    auto render(SDL_Renderer* renderer) -> void;
};

template<typename ManagedSDLTexture_T>
Button::Button(ManagedSDLTexture_T&& texture_default,
               ManagedSDLTexture_T&& texture_hovering,
               ManagedSDLTexture_T&& texture_clicking,
               SDL_Rect const& area):   texture_default_(std::forward<ManagedSDLTexture_T>(texture_default)),
                                        texture_hovering_(std::forward<ManagedSDLTexture_T>(texture_hovering)),
                                        texture_clicking_(std::forward<ManagedSDLTexture_T>(texture_clicking)),
                                        current_texture_(&texture_default),
                                        rect_(area),
                                        hovering_(false),
                                        clicking_(false),
                                        clicked_(false) {}

#endif
