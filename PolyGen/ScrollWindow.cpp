#pragma once
#include "ScrollWindow.h"
#include <ShObjIdl.h>
#include <windows.h>

sf::Sprite backGround;

ScrollWindow::ScrollWindow()
{
}

ScrollWindow::ScrollWindow(sf::FloatRect real, sf::FloatRect mind)
{
    // Set bounds //
    control = real;
    mindPos.x = mind.left, mindPos.y = mind.top;
    mindSize.x = mind.width, mindSize.y = mind.height;
    mainField.create(control.width, control.height); // Exc
                                                     // Set variables //
    speed = sf::Vector2f(0, 0);
}

ScrollWindow::~ScrollWindow()
{

}

void ScrollWindow::update()
{
    // Update coords //
    mindPos.x += speed.x;
    mindPos.y += speed.y;
    if (mindPos.x < 0) mindPos.x = 0;
    if (mindPos.y < 0) mindPos.y = 0;
    if (mindPos.x + control.width >= mindSize.x)
        mindPos.x = mindSize.x - control.width;
    if (mindPos.y + control.height >= mindSize.y)
        mindPos.y = mindSize.y - control.height;
    // Update speed //
    if (isPressed)
        speed.x *= deaccl.x, speed.y *= deaccl.y;
    else speed.x *= deaccl_f.x, speed.y *= deaccl_f.y;
}

void ScrollWindow::moveWindow(float dx, float dy)
{
    speed.x += dx * accl.x;
    speed.y += dy * accl.y;
}

void ScrollWindow::openDrawing()
{
    mainField.clear(sf::Color::Transparent);
    mainField.setSmooth(false);
}

bool ScrollWindow::checkPressed(float x, float y)
{
    if (!isPressed && control.contains(x, y))
    {
        isPressed = true;
        return true;
    }
    return false;
}

bool ScrollWindow::checkMove(float dx, float dy)
{
    if (isPressed)
    {
        moveWindow(dx, dy);
        return true;
    }
    return false;
}

bool ScrollWindow::checkOver(float x, float y)
{
	return control.contains(x, y);
}

void ScrollWindow::draw(PxButton * btn, int padding, int dx, int dy, float uss)
{
    if (btn->backGround)
    {
        btn->backGround->setPosition(btn->control.left - mindPos.x, btn->control.top - mindPos.y);
        mainField.draw(*(btn->backGround));
    }
    if (btn->mainText.getString() != "")
    {
        sf::FloatRect txtsz = btn->mainText.getLocalBounds();
        float tx = btn->control.left + btn->control.width * uss * (padding + 1) / 2 - mindPos.x;
        float ty = btn->control.top + btn->control.height * uss / 2 - mindPos.y;
        btn->mainText.setPosition(tx + dx, ty + dy);
        btn->mainText.setOrigin(btn->textControl.left + btn->textControl.width * (padding + 1) / 2,
            btn->textControl.top + btn->textControl.height / 2);
        mainField.draw(btn->mainText);
    }
}

void ScrollWindow::draw(sf::FloatRect rect, sf::Color c)
{
    sf::RectangleShape bck;
    bck.setFillColor(sf::Color(0, 0, 0, 30));
    bck.setPosition(rect.left - mindPos.x, rect.top - mindPos.y);
    bck.setSize(sf::Vector2f(rect.width, rect.height));
    mainField.draw(bck);
}

void ScrollWindow::draw(sf::Sprite * spr)
{
    sf::Vector2f buf = spr->getPosition();
    spr->setPosition(buf.x - mindPos.x, buf.y - mindPos.y);
    mainField.draw(*spr);
    spr->setPosition(buf);
}

void ScrollWindow::draw(sf::Sprite * spr, int x, int y, float x_scale, float y_scale)
{
    sf::Vector2f buf = spr->getPosition();
    if(x_scale && y_scale) spr->setScale(x_scale, y_scale);
    spr->setPosition(x - mindPos.x, y - mindPos.y);
    mainField.draw(*spr);
    spr->setPosition(buf);
}

void ScrollWindow::draw(TextBox * txb, int16_t delta_h)
{
    sf::FloatRect buf = txb->bounds;
    txb->resultBox.setPosition(buf.left - mindPos.x, buf.top - mindPos.y);
    mainField.draw(txb->resultBox);
    txb->resultBox.setPosition(buf.left, buf.top);
    if (txb->isCursorShowed)
    {
        sf::RectangleShape cursorShape(sf::Vector2f(2, txb->charH * 1.2));
        cursorShape.setPosition(txb->cursorPos.x - 1 - mindPos.x, txb->cursorPos.y + delta_h - mindPos.y);
        cursorShape.setFillColor(txb->resultBox.getFillColor());
        mainField.draw(cursorShape);
    }
}

void ScrollWindow::draw(ClrBox * txb, int16_t delta_h, std::string colorMap)
{
    sf::FloatRect box = txb->bounds;
    if (!txb->hasMovingView)
    {
        sf::Vector2f buf = txb->resultBox.getPosition();
        txb->resultBox.setPosition(box.left - mindPos.x - txb->visiblePos.x, box.top - mindPos.y);
		//printf("%s\n",txb->resultBox.getString().toAnsiString());
        if (colorMap.empty()) mainField.draw(txb->resultBox);
        else txb->showAt(&mainField, delta_h);
        txb->resultBox.setPosition(buf);
    }
    else
    {
        auto spr = sf::Sprite(txb->viewTexture->getTexture());
        spr.setPosition(box.left - mindPos.x, box.top - mindPos.y);
        spr.setOrigin(0, box.height);
        spr.setScale(1, -1);
        mainField.draw(spr);
    }
    if (colorMap.empty() && txb->isCursorShowed)
    {
        sf::RectangleShape cursorShape(sf::Vector2f(2, txb->charH * 1.2));
        cursorShape.setPosition(txb->cursorPos.x - 1 - mindPos.x, txb->cursorPos.y + delta_h - mindPos.y);
        cursorShape.setFillColor(sf::Color::Black);
        mainField.draw(cursorShape);
    }
}

void ScrollWindow::uniteDrawing(sf::RenderWindow * window)
{
    mainField.display();
    sf::Sprite realSprite(mainField.getTexture());
    realSprite.setPosition(control.left, control.top);
    window->draw(realSprite);
}

std::pair<int, std::wstring > ScrollWindow::openFile()
{
    IFileOpenDialog *pFile = NULL;
    IShellItem *pItem = NULL;

    HRESULT hr = CoCreateInstance(__uuidof(FileOpenDialog), NULL,
        CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFile));
    if (FAILED(hr)) return{ -4, L"" };
    hr = pFile->Show(NULL);
    if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED))
    {
        hr = S_OK;
        return { 1, L"" };
    }
    else if (FAILED(hr)) return { -2, L"" };
    else hr = pFile->GetResult(&pItem);
    if (FAILED(hr)) return{ -1, L"" };
    if (SUCCEEDED(hr)) {
        LPWSTR nameBuffer;
        if (!pItem) return { -3, L"" };
        pItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &nameBuffer);
        std::wstring wname;
        wname = std::wstring(nameBuffer);
        CoTaskMemFree(nameBuffer);
        pItem->Release();
        return {0, wname};
    }
}
