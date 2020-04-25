#include "components/TextureComponent.hpp"

TextureComponent::TextureComponent():   texture_(nullptr),
                                        rect_({0, 0, 0, 0}),
                                        hovering_(false),
                                        clicking_(false),
                                        clicked_(false) {}

auto TextureComponent::texture() -> ManagedSDLTexture& { return texture_; }

auto TextureComponent::isHovering() const -> bool { return hovering_; }
auto TextureComponent::isClicking() const -> bool { return clicking_; }
auto TextureComponent::isClicked()  const -> bool { return clicked_; }

auto TextureComponent::pos()  const -> SDL_Point { return {rect_.x, rect_.y}; }
auto TextureComponent::dim()  const -> SDL_Point { return {rect_.w, rect_.h}; }
auto TextureComponent::rect() const -> SDL_Rect const&  { return rect_; }

auto TextureComponent::setPos(SDL_Point pos) -> TextureComponent& { rect_.x = pos.x; rect_.y = pos.y; return *this; }
auto TextureComponent::setDim(SDL_Point dim) -> TextureComponent& { rect_.w = dim.x; rect_.h = dim.y; return *this; }
auto TextureComponent::setRect(SDL_Rect const& rect) -> TextureComponent& { rect_ = rect; return *this; }

auto TextureComponent::setDimToTexture() -> TextureComponent& { setDim(texture_.clipDim()); return *this; }

auto TextureComponent::update() -> void {
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
}


auto TextureComponent::render(SDL_Renderer* renderer) -> void {
    texture_.render(renderer, &rect_);
}

auto TextureComponent::operator=(ManagedSDLTexture& managed_texture)  -> TextureComponent& {
    texture_ = managed_texture;
    return *this;
}
auto TextureComponent::operator=(ManagedSDLTexture&& managed_texture) -> TextureComponent& {
    texture_ = managed_texture;
    return *this;
}
auto TextureComponent::operator=(TextureComponent& other)  -> TextureComponent& {
    texture_ = other.texture_;
    rect_    = other.rect_;
    return *this;
}
auto TextureComponent::operator=(TextureComponent&& other) -> TextureComponent& {
    texture_ = std::move(other.texture_);
    rect_    = other.rect_;
    return *this;
}
