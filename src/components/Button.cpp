#include "components/Button.hpp"

auto Button::isHovering() -> bool { return hovering_; }
auto Button::isClicking() -> bool { return clicking_; }
auto Button::isClicked()  -> bool { return clicked_; }

auto Button::pos()  const -> SDL_Point { return {rect_.x, rect_.y}; }
auto Button::dim()  const -> SDL_Point { return {rect_.w, rect_.h}; }
auto Button::rect() const -> SDL_Rect const&  { return rect_; }

auto Button::setPos(SDL_Point pos) -> void { rect_.x = pos.x; rect_.y = pos.y; }
auto Button::setDim(SDL_Point dim) -> void { rect_.w = dim.x; rect_.h = dim.y; }
auto Button::setRect(SDL_Rect const& rect) -> void { rect_ = rect; }

auto Button::update() -> void {
    clicked_ = false;

    if (mouse::x() > rect_.x && mouse::x() < rect_.x+rect_.w && mouse::y() > rect_.y && mouse::y() < rect_.y+rect_.h) {
        if (mouse::pressed()) {
            if (hovering_) {
                // Mouse has entered and now mouse is clicked
                clicking_ = true;
            } else {
                // Mouse entered already clicked.
                hovering_ = false;
                clicking_ = false;
                clicked_  = false;
            }
        } else if (!mouse::pressed() && clicking_) {
            // Mouse was down last update, and now it is released.
            clicking_ = false;
            clicked_ = true;
        } else {
            // Otherwise mouse is simply hovering
            hovering_ = true;
            clicking_ = false;
        }
    } else if (mouse::pressed() && clicking_) {
        // Mouse was down and in bounds last frame, hold clicking state
        hovering_ = false;
    } else {
        // Mouse is up and out of bounds
        hovering_ = false;
        clicking_ = false;
        clicked_  = false;
    }

    if (clicking_) {
        current_texture_ = &texture_clicking_;
    } else if (hovering_) {
        current_texture_ = &texture_hovering_;
    } else {
        current_texture_ = &texture_default_;
    }
}

auto Button::render(SDL_Renderer* renderer) -> void {
    current_texture_->render(renderer, &rect_);
}
