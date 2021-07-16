#pragma once
#include <SFML\Graphics.hpp>
#include "TextBox.h"
class PxButton
{
public:
    // Constants //
    // Fields //
    float brightOver = 0.8, brightPress = 0.6, brightBox = 0.7, MAX_FONTSIZE = 200;
    sf::Sprite * backGround = nullptr;
    sf::Font mainFont;
    sf::Text mainText;
    sf::Color boxColor = sf::Color::White,
        overColor = sf::Color::White,
        pressColor = sf::Color::White, 
        offColor = sf::Color(255, 255, 255, 128);
    sf::FloatRect control;
    int meta = 0;
    bool isPressed = false, isOver = false, hasSpecialSprite = false, isEnabled = false;
    float fontSize, textSize;
    sf::FloatRect textControl;
    // Methods //
    PxButton();
    PxButton(sf::FloatRect rect, sf::Color c, float fs, char * txt, sf::Font& f);
    void setColor(sf::Color c, float b = 0.7, float o = 0.8, float p = 0.6);
    void setSprite(sf::Sprite * sprt);
    void setPosition(float x, float y);
    void setCharacterSize(size_t size);
    void setIsEnabled(bool enable);
    void update();
    bool checkPressed(sf::FloatRect box, sf::Vector2f mindPos, float x, float y);
    bool checkOver(sf::FloatRect box, sf::Vector2f mindPos, float x, float y);
    bool checkReleased(sf::FloatRect box, sf::Vector2f mindPos, float x, float y);
    void prepareBack(sf::Sprite * cornerL, sf::Sprite * cornerR, sf::Sprite * body);
    ~PxButton();
};