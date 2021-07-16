#pragma once
#include <SFML\Graphics.hpp>
class ownUtilities
{
public:
    // Constants //
    const sf::String fontFamily = "fonts/Nunito-Regular.ttf";
    const sf::String textFontFamily = "fonts/Bungee-Regular.ttf";
    const sf::String mathFontFamily = "fonts/16654.otf";
    const float USS = 2;
    // Fields //
    // Methods //
    sf::Sprite * createBackground(sf::FloatRect box, sf::String texturePath, bool scaled = true);
    ownUtilities();
    ~ownUtilities();
};

