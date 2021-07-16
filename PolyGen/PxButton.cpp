#pragma once
#include "PxButton.h"

PxButton::PxButton()
{

}

PxButton::PxButton(sf::FloatRect rect, sf::Color c, float fs, char * txt, sf::Font& f)
{
    // Set variables //
    control = rect;
    setColor(c);
    textSize = fs;
    isEnabled = true;
    // Prepare visible text //
    mainText = sf::Text(txt, f, 1);
    // Get font size //
    uint16_t first = 0, last = MAX_FONTSIZE;
    while (last - first > 1)
    {
        uint16_t middle = first + (last - first) / 2;
        mainText.setCharacterSize(middle);
        if (mainText.getLocalBounds().width < fs)
            first = middle;
        else last = middle;
    }
    // Set text options //
    mainText.setCharacterSize(first);
    textControl = mainText.getGlobalBounds();
    mainText.setFillColor(sf::Color::Black);
    mainText.setOrigin(textControl.left + textControl.width / 2,
        textControl.top + textControl.height / 2);
}

void PxButton::setColor(sf::Color c, float b, float o, float p)
{
    brightBox = b, brightOver = o, brightPress = p;
    boxColor.r = c.r * b;
    boxColor.g = c.g * b;
    boxColor.b = c.b * b;
    boxColor.a = c.a;
    overColor.r = c.r * o;
    overColor.g = c.g * o;
    overColor.b = c.b * o;
    overColor.a = c.a;
    pressColor.r = c.r * p;
    pressColor.g = c.g * p;
    pressColor.b = c.b * p;
    pressColor.a = c.a;
    offColor.r = c.r;
    offColor.g = c.g;
    offColor.b = c.b;
    offColor.a = c.a;

    update();
}

void PxButton::setSprite(sf::Sprite * sprt)
{
    backGround = sprt;
    backGround->setPosition(control.left, control.top);
    update();
}

void PxButton::setPosition(float x, float y)
{
    backGround->setPosition(x, y);
    control.left = x, control.top = y;
}

void PxButton::setCharacterSize(size_t size)
{
    mainText.setOrigin(0, 0);
    mainText.setCharacterSize(size);
    textControl = mainText.getGlobalBounds();
}

void PxButton::setIsEnabled(bool enable)
{
    isEnabled = enable;
    update();
}

void PxButton::update()
{
    if (backGround)
    {
        if (isEnabled)
        {
            if (isPressed) backGround->setColor(pressColor);
            else if (isOver) backGround->setColor(overColor);
            else backGround->setColor(boxColor);
        }
        else backGround->setColor(offColor);
    }
}

bool PxButton::checkPressed(sf::FloatRect box, sf::Vector2f mindPos, float x, float y)
{
    if (!isPressed && isEnabled)
    {
        sf::FloatRect real;
        real.left = control.left - mindPos.x;
        real.top = control.top - mindPos.y;
        real.width = control.width;
        real.height = control.height;
        if (real.contains(x - box.left, y - box.top))
        {
            isPressed = true;
            update();
            return true;
        }
    }
    return false;
}

bool PxButton::checkOver(sf::FloatRect box, sf::Vector2f mindPos, float x, float y)
{
    if (!isPressed && isEnabled)
    {
        sf::FloatRect real;
        real.left = control.left - mindPos.x;
        real.top = control.top - mindPos.y;
        real.width = control.width;
        real.height = control.height;
        if (real.contains(x - box.left, y - box.top))
        {
            isOver = true;
            update();
            return true;
        }
        else if (isOver)
        {
            isOver = false;
            update();
        }
    }
    return false;
}

bool PxButton::checkReleased(sf::FloatRect box, sf::Vector2f mindPos, float x, float y)
{
    if (isPressed && isEnabled)
    {
        sf::FloatRect real;
        real.left = control.left - mindPos.x;
        real.top = control.top - mindPos.y;
        real.width = control.width;
        real.height = control.height;
        //printf("%f %f %f %f\n", real.left, real.top, real.width, real.height);
        isPressed = false;
        update();
        if (real.contains(x - box.left, y - box.top))
            return true;
    }
    return false;
}

void PxButton::prepareBack(sf::Sprite * cornerL, sf::Sprite * cornerR, sf::Sprite * body)
{
    cornerL->setPosition(control.left, control.top);
    auto cornerSize = cornerR->getGlobalBounds();
    cornerR->setPosition(control.left + control.width - cornerSize.width, control.top);
    body->setPosition(control.left + cornerSize.width / 2, control.top);
    auto ts = body->getTexture()->getSize();
    body->setScale((control.width - cornerSize.width) / ts.x, control.height / ts.y);
    cornerL->setColor(backGround->getColor());
    cornerR->setColor(backGround->getColor());
    body->setColor(backGround->getColor());
}


PxButton::~PxButton()
{
    if (!hasSpecialSprite)
    {
        delete backGround->getTexture();
        delete backGround;
    }
}