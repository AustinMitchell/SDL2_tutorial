#ifndef COMPONENTS_TEXTURECOMPONENT_HPP_
#define COMPONENTS_TEXTURECOMPONENT_HPP_

#include <utility>

#include "SDL_helpers.hpp"

class TextureComponent {
 protected:

    ManagedSDLTexture texture_;

    SDL_Rect rect_;

    bool hovering_;
    bool clicking_;
    bool clicked_;

 public:
    TextureComponent();
    template<typename Texture_T>
    TextureComponent(Texture_T&& texture, SDL_Rect const& area);

    auto texture() -> ManagedSDLTexture&;

    auto isHovering() const -> bool;
    auto isClicking() const -> bool;
    auto isClicked()  const -> bool;

    auto pos()  const -> SDL_Point;
    auto dim()  const -> SDL_Point;
    auto rect() const -> SDL_Rect const&;

    auto setPos(SDL_Point const &pos)  -> TextureComponent&;
    auto setPos(SDL_Rect  const &rect) -> TextureComponent&;
    auto setDim(SDL_Point const &dim)  -> TextureComponent&;
    auto setDim(SDL_Rect  const &rect) -> TextureComponent&;
    auto setRect(SDL_Rect const& rect) -> TextureComponent&;

    auto setDimToTexture() -> TextureComponent&;

    auto update() -> void;

    auto render(SDL_Renderer* renderer) -> void;

    auto operator=(ManagedSDLTexture& texture)  -> TextureComponent&;
    auto operator=(ManagedSDLTexture&& texture) -> TextureComponent&;
    auto operator=(TextureComponent& texture)   -> TextureComponent&;
    auto operator=(TextureComponent&& texture)  -> TextureComponent&;


};

template<typename Texture_T>
TextureComponent::TextureComponent(Texture_T&& texture,
                                   SDL_Rect const&       area):
                                                                texture_(std::forward<Texture_T>(texture)),
                                                                rect_(area),
                                                                hovering_(false),
                                                                clicking_(false),
                                                                clicked_(false) {}

#endif
