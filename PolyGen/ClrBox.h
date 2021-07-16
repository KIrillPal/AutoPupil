#pragma once
#include <SFML\Graphics.hpp>
#include <map>
class ClrBox
{
public:
    // Constants //
    const uint16_t MOD = 20;
    // Fields //
    std::string colorMap = "";
    sf::String text, mod_text, last_text = "";
    sf::Text resultBox;
    sf::Font mainFont;
    sf::FloatRect bounds;
    sf::RenderTexture * viewTexture;
    bool isActive = false, isFocused = false, isCursorShowed = false, hasMovingView = false;
    std::map<char, sf::Color> * colorDictionary;
    sf::Vector2f cursorPos, visiblePos;
    uint16_t timer = 0, cursor = 0, charH = 20;
    int movingStep = 10;
    // Methods //
    ClrBox(sf::String txt, sf::FloatRect mb, uint16_t sz, sf::Font& tF);
    void setPosition(float x, float y);
    void setFocused();
    void setMovingView(bool view);
    bool offFocused();
    void update();
    void showAt(sf::RenderTexture * window, int16_t delta_h);
    bool checkPressed(sf::FloatRect box, sf::Vector2f mindPos, float x, float y, bool outbox);
    bool checkKeyPressed(sf::Event::KeyEvent e, bool(*cmp)(char c) = [](char c) {return false;});
    bool checkInput(sf::String key, bool (*f)(char));
    void updateText();
    void updateCursor();
    void redrawTexture();
	void setString(sf::String t);
    ClrBox();
    ~ClrBox();
};

