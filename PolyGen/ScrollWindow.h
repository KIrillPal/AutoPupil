#pragma once
#include <SFML\Graphics.hpp>
#include "PxButton.h"
#include "ClrBox.h"
class ScrollWindow
{
public:
    // Constants //
    const sf::Vector2f accl = sf::Vector2f(0.32f, 0.32f);
    const sf::Vector2f deaccl_f = sf::Vector2f(0.96f, 0.96f);
    const sf::Vector2f deaccl = sf::Vector2f(0.68f, 0.68f);
    // Fields //
    sf::Vector2f mindPos, mindSize, speed, mindBounds;
    sf::FloatRect control;
    sf::RenderTexture mainField;
    // Methods //
    ScrollWindow();
    ScrollWindow(sf::FloatRect real, sf::FloatRect mind);
    void update();
    void moveWindow(float dx, float dy);
    void draw(PxButton * btn, int padding = 0, int dx = 0, int dy = 0, float uss = 1);
    void draw(TextBox * txb, int16_t delta_h);
    void draw(ClrBox * txb, int16_t delta_h, std::string colorMap = "");
    void draw(sf::FloatRect rect, sf::Color c);
    void draw(sf::Sprite * spr);
    void draw(sf::Sprite * spr, int x, int y, float x_scale = 0, float y_scale = 0);
    void openDrawing();
    bool checkPressed(float x, float y);
	bool checkMove(float dx, float dy);
	bool checkOver(float x, float y);
    void uniteDrawing(sf::RenderWindow * window);
    std::pair<int, std::wstring > openFile();
    bool isPressed = false;
    ~ScrollWindow();
};