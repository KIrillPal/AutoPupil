#pragma once
#include <SFML\Graphics.hpp>
#include <string>

class GraphMap
{
public:
    GraphMap(uint16_t width, uint16_t height, char* programName);
    GraphMap();
    ~GraphMap();
    sf::RenderWindow* getWindow();
    sf::RenderWindow window;
    void updateGraph(sf::FloatRect real_window, sf::FloatRect mind_window, std::pair<double, double>(*getclr)(double x));
    bool hasTwoGrs = false;
    sf::Color fclr, sclr;
private:
    uint16_t currentWidth, currentHeight;
    sf::FloatRect mindFrame, frame;
    void GraphMap::setPixel(uint16_t x, uint16_t y, size_t width, size_t height, sf::Color c);
    void GraphMap::setRound(uint16_t xa, uint16_t ya, size_t width, size_t height, sf::Color lc);
    void GraphMap::drawLines(float x, float y, sf::Vector2f lw, sf::FloatRect real_window,
        sf::FloatRect mind_window, sf::Color c, sf::Vector2f step, sf::Vector2f scale);
    void GraphMap::drawNumbers(float x, float y, sf::FloatRect real_window, sf::FloatRect mind_window,
        sf::Vector2f step, sf::Vector2f scale, bool axis);
};

