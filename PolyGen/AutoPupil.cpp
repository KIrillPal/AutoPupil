#pragma once
#include <chrono>
#include <set>
#include <iostream>
#include "MainMap.h"
#include "ScrollWindow.h"
#include "ownUtilities.h"
#include "DataBase.h"

float SCROLL_OFFSET = 0.9f;

const char* PROGRAM_NAME = "AutoPupil";
uint16_t WINDOW_W = 820, WINDOW_H = 600;
const uint16_t LEFT_W = 265, MAX_PLAYERS = 10, FIELD_W = 500;
const float USS = 2, ProgramFrameTime = 1000 / 240; // 1000ms / 60frames
MainMap mainMap(WINDOW_W, WINDOW_H, PROGRAM_NAME);
DataBase mainDB;
auto gameFrameTime = std::chrono::steady_clock::now();
int windowMode = 1, length = 200, doubleClickTimer = -1;
float mouseX = 0, mouseY = 0;
uint64_t frameTime = 0;
sf::Font bF, tF;

sf::RenderWindow *window;
ownUtilities owt;

sf::FloatRect mindFrame(-6, -6, 12, 12), frame(0, 0, 228, 228);

int main()
{
    bF.loadFromFile(owt.fontFamily);
    tF.loadFromFile(owt.textFontFamily);
	mainDB.df.loadFromFile(owt.textFontFamily);
	mainDB.bF.loadFromFile(owt.fontFamily);
    mainDB.xF.loadFromFile(mainMap.fontFamily);
    mainMap.dataBase = &mainDB;
    mainMap.initBoxes(WINDOW_W, WINDOW_H, mainDB.xF, bF, &owt);
    window = mainMap.getWindow();
    window->setFramerateLimit(60);
	BigFixed::set_precision(20);
	BigFixed::E = BigFixed(mainDB.variables["e"]);
	BigFixed::PI = BigFixed(mainDB.variables["pi"]);
    while (window->isOpen())
    {
        sf::Event e;
        while (window->pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window->close();
                return 0;
            }
            int X = e.mouseButton.x, Y = e.mouseButton.y;
            if (windowMode == 1)
            {
                if (e.type == sf::Event::MouseMoved) { 
                    if (mainDB.windowMode < 2)
                        mainMap.checkMoved(e.mouseMove.x, e.mouseMove.y, mouseX - e.mouseMove.x, mouseY - e.mouseMove.y);
                }
                else if (e.type == sf::Event::MouseWheelScrolled) {
                }
                else if (e.type == sf::Event::MouseButtonPressed) {
                    if(mainDB.windowMode < 2 || mainDB.windowMode == 4)
                        mainMap.checkPressed(X, Y);
                }
                else if (e.type == sf::Event::MouseButtonReleased) {
                    if (doubleClickTimer <= 0)
						doubleClickTimer = 20;
                    if (mainDB.windowMode < 2 || mainDB.windowMode == 4)
                        mainMap.checkReleased(X, Y);
                }
                else if (e.type == sf::Event::KeyPressed) { 
                    mainMap.checkKeyPressed(e.key);
                }
                else if (e.type == sf::Event::TextEntered)
                { 
                    mainMap.checkTextEntered(e.text);
                }
            }
            else if (windowMode == 2)
            {
                if (e.type == sf::Event::KeyPressed) {}
                else if (e.type == sf::Event::MouseButtonPressed) { }
                else if (e.type == sf::Event::MouseButtonReleased) { }
            }
            else if (windowMode == 3)
            {
                if (e.type == sf::Event::MouseMoved) { }
                else if (e.type == sf::Event::MouseButtonPressed) { }
                else if (e.type == sf::Event::MouseButtonReleased) { }
                else if (e.type == sf::Event::KeyPressed) { }
                else if (e.type == sf::Event::TextEntered) { }
            }
            if (e.type == sf::Event::MouseMoved)
            {
                mouseX = e.mouseMove.x;
                mouseY = e.mouseMove.y;
            }
            else if (e.type == sf::Event::Resized)
            {
                WINDOW_W = e.size.width, WINDOW_H = e.size.height;
                window->setView(sf::View(sf::FloatRect(0, 0, WINDOW_W, WINDOW_H)));
                mainMap.resizeBoxes(WINDOW_W, WINDOW_H);
            }
        }
        --doubleClickTimer;
        window->clear(sf::Color(215, 215, 215, 255));
        mainMap.drawBoxes();
        window->display();
    }
}