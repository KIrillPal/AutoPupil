#pragma once
#include "ClrBox.h"
#include <string>


ClrBox::ClrBox(sf::String txt, sf::FloatRect mb, uint16_t sz, sf::Font& tF)
{
    text = txt;
    bounds = mb;

    resultBox = sf::Text("A", tF, sz);
    resultBox.setPosition(mb.left, mb.top);
    resultBox.setFillColor(sf::Color::Black);
    charH = resultBox.getLocalBounds().height + 3;
    resultBox.setString(txt);
    visiblePos = {0, 0};
    updateText();
}

void ClrBox::setPosition(float x, float y)
{
    resultBox.setPosition(x, y);
    bounds.left = x, bounds.top = y;
}

void ClrBox::setFocused()
{
    if (isActive)
    {
        isFocused = true;
        last_text = text;
        timer = 0;
    }
}

void ClrBox::setMovingView(bool view)
{
    if (!hasMovingView && view)
    {
        viewTexture = new sf::RenderTexture();
        viewTexture->create(bounds.width, bounds.height);
        hasMovingView = view;
        redrawTexture();
    }
    else if (hasMovingView && !view)
        delete viewTexture;
    hasMovingView = view;
}

bool ClrBox::offFocused()
{
    bool res = isFocused;
    isFocused = false;
    isCursorShowed = false;
    return res;
}

void ClrBox::update()
{
    if (isFocused && ++timer % MOD == 0)
        timer = 0, isCursorShowed ^= 1;
}

void ClrBox::showAt(sf::RenderTexture * window, int16_t delta_h)
{
    if (colorMap.empty())
    {
        window->draw(resultBox);
    }
    else
    {
        sf::Text t;
        std::string ntxt = resultBox.getString();
        t.setFont(*resultBox.getFont());
        t.setCharacterSize(resultBox.getCharacterSize());
        int n = text.getSize(), j = 0;
        for (int i = 0; i < n; ++i)
        {
            t.setString(text[i]);
            if (colorMap.size() > i)
                t.setFillColor((*colorDictionary)[colorMap[i]]);
            else t.setFillColor(sf::Color::Black);
            t.setPosition(resultBox.findCharacterPos(j++));
            if (j < ntxt.size() && ntxt[j] == '\n') ++j;
            window->draw(t);
        }
        // window->draw(resultBox);
        /*if (isCursorShowed)
        {
            sf::RectangleShape cursorShape(sf::Vector2f(2, charH * 1.2));
            cursorShape.setPosition(cursorPos.x - 1, cursorPos.y + delta_h);
            cursorShape.setFillColor(sf::Color::Black);
            window->draw(cursorShape);
        }*/
    }
}

bool ClrBox::checkPressed(sf::FloatRect box, sf::Vector2f mindPos, float x, float y, bool outbox = false)
{
    sf::FloatRect bnd = bounds;
    bnd.left -= mindPos.x;
    bnd.top -= mindPos.y;
    if (outbox) bnd.width += 300;
    bnd.width += charH;
    if (bnd.height == 0) bnd.height = charH;
    if (isActive && bnd.contains(x - box.left, y - box.top))
    {
        x -= box.left - mindPos.x;
        y -= box.top - mindPos.y;
        //printf("%f %f\n", box.left, box.top);
        if (!isFocused) setFocused();
        sf::String str = resultBox.getString();
        float minPath = -1;
        int minCh = -1, n = str.getSize();
        sf::Vector2f curs;
        for (size_t i = 0, j = 0; i <= n; ++i)
        {
            if (str[i] == '\n') continue;
            sf::Vector2f chPos = resultBox.findCharacterPos(i);
            float path = (x - chPos.x) * (x - chPos.x) + (y - chPos.y - charH) * (y - chPos.y - charH);
            if (minPath == -1 || path < minPath)
            {
                minPath = path;
                minCh = j;
                curs = chPos;
            }
            ++j;
        }
        cursorPos = curs;
        cursor = minCh;
        updateCursor();
        return true;
    }
    return false;
}

bool ClrBox::checkKeyPressed(sf::Event::KeyEvent e, bool (*cmp)(char c))
{
    if (isActive && isFocused)
    {
        if (e.code == sf::Keyboard::Enter)
        {
            offFocused();
        }
        else if (e.code == sf::Keyboard::Escape)
        {
            offFocused();
            text = last_text;
            updateText();
            redrawTexture();
            return true;
        }
        else if (e.code == sf::Keyboard::BackSpace && cursor > 0)
        {
            text.erase(cursor - 1);
            --cursor;
            updateText();
            redrawTexture();
            return true;
        }
        else if (e.code == sf::Keyboard::Delete && cursor < text.getSize())
        {
            text.erase(cursor);
            updateText();
            redrawTexture();
            return true;
        }
        else if (e.code == sf::Keyboard::Left && cursor > 0)
        {
            --cursor;
            updateText();
        }
        else if (e.code == sf::Keyboard::Right && cursor < text.getSize())
        {
            ++cursor;
            updateText();
        }
        else if (e.code == sf::Keyboard::Up)
        {
            checkPressed(sf::FloatRect(), sf::Vector2f(), cursorPos.x, cursorPos.y - charH);
        }
        else if (e.code == sf::Keyboard::Down)
        {
            checkPressed(sf::FloatRect(), sf::Vector2f(), cursorPos.x, cursorPos.y + 2 * charH);
        }
        else if (e.code == sf::Keyboard::V && e.control)
        {
            return checkInput(sf::Clipboard::getString(), cmp);
        }
    }
    return false;
}

bool ClrBox::checkInput(sf::String key, bool(*cmp)(char))
{    bool correct = true;
    for (char c : key.toAnsiString())
        correct &= cmp(c);
    if (isActive && isFocused && correct)
    {
        sf::String mod = resultBox.getString();
        resultBox.setString(mod + key);
        auto bdw = resultBox.getGlobalBounds();
        resultBox.setString(mod + '\n' + key);
        auto bdh = resultBox.getGlobalBounds();
        resultBox.setString(mod);
        if (!hasMovingView && bdw.width > bounds.width && bdh.height > bounds.height)
            return false;
        text.insert(cursor, key);
        cursor += key.getSize();
        updateText();
        redrawTexture();
        return true;
    }
    return false;
}

void ClrBox::updateText()
{
    sf::String res;
    int p = 0, n = text.getSize(), cps = -1;
    while (p < n)
    {
        resultBox.setString(res + text[p]);
        if (resultBox.getGlobalBounds().width > bounds.width && !hasMovingView)
            res += '\n';
        if (p == cursor)
            cps = res.getSize();
        res += text[p];
        ++p;
    }
    resultBox.setString(res);
    cursorPos = resultBox.findCharacterPos(cps == -1 ? res.getSize() : cps);
    updateCursor();
}

void ClrBox::updateCursor()
{
    if (!hasMovingView) return;
    if (cursorPos.x >= bounds.left && cursorPos.x <= bounds.left + bounds.width) return;
    int v = visiblePos.x;
    if (cursorPos.x < bounds.left)
        visiblePos.x += cursorPos.x - bounds.left;
    else if (cursorPos.x > bounds.left + bounds.width)
        visiblePos.x += cursorPos.x - bounds.left - bounds.width;
    resultBox.setPosition(bounds.left - visiblePos.x, bounds.top);
    cursorPos.x += v - visiblePos.x;
    redrawTexture();
}

void ClrBox::redrawTexture()
{
    if (!hasMovingView) return;
    viewTexture->clear(sf::Color::Transparent);
    auto buf = resultBox.getPosition();
    resultBox.setPosition(-visiblePos.x, 0);
    showAt(viewTexture, 1);
    resultBox.setPosition(buf);
}

void ClrBox::setString(sf::String t)
{
	text = t;
	resultBox.setString(t);
	if (hasMovingView)
		redrawTexture();
}

ClrBox::~ClrBox()
{
    if (hasMovingView)
        delete viewTexture;
}