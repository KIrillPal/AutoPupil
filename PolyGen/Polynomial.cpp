#pragma once
#include <chrono>
#include <set>
#include "DrawingMap.h"
#include "ownUtilities.h"
#include "ScrollWindow.h"

const char* PROGRAM_NAME = "Snake";
const uint16_t WINDOW_W = 820, WINDOW_H = 600, DIAL_W = 200, DIAL_H = 120, LEFT_W = 265, MAX_PLAYERS = 10, FIELD_W = 500;
const float USS = 2, ProgramFrameTime = 1000 / 240; // 1000ms / 60frames
DrawingMap drawingMap(WINDOW_W, WINDOW_H, PROGRAM_NAME);
int windowMode = 1, selDialButton = 0, selCol = 0, newPlayerNum = 1, crtAI = 0, fieldSize = 20, apples = 1, length = 200;
float mouseX = 0, mouseY = 0;
char crtChW = 22 + 'A', crtChA = 0 + 'A', crtChS = 18 + 'A', crtChD = 3 + 'A';
auto gameFrameTime = std::chrono::steady_clock::now();
uint64_t frameTime = 0;
sf::String botNm[] = { "DU", "DO", "UM", "LA", "KO", "BE", "AK", "SE", "DA", "DO", "JE", 
"PO", "KE", "OW", "ULF", "RA", "PEN", "DOS", "KA", ""};
sf::Vector2f dialPos(WINDOW_W - DIAL_W * USS, WINDOW_H - DIAL_H * USS);
sf::Color crtColors[7] = {sf::Color::Green, sf::Color::Red, sf::Color::Blue,
sf::Color::Magenta, sf::Color::Cyan, sf::Color::Yellow, sf::Color::White};
sf::Font bF, tF;

sf::RenderWindow *window;
ownUtilities owt;
// Load menu /
ScrollWindow *loadScroll, *botScroll;
TextBox *ldSize, *ldApples, *ldLength, *ldSet;
PxButton *ldPlayer, *ldBot, *ldLeft, *ldRight;
sf::Sprite *ldTb, *ldSt, *ldAp, *ldLn;

// Player list //
std::vector<DrawingMap::Snake *> players;
std::vector<DrawingMap::Snake> oldPlayers;
std::vector<TextBox *> nameList;
std::vector<PxButton *> closeList;
sf::Sprite * colorFlag;
std::vector<sf::Sprite *> closeSprites(10);
// Dialog Window //

ScrollWindow *dialScroll;
TextBox *crtName, *crtSN, *crtC;
PxButton *crtClose, *crtOK, *crtColor, *crtW, *crtA, *crtS, *crtD;
sf::FloatRect crtTextBox;
sf::Sprite * crtBack, *crtTb;

// Game menu //
sf::Sprite * mainField;
PxButton * playBox;

int64_t parse(TextBox * tb)
{
    return std::stoll(tb->text.toAnsiString());
}

void enOK()
{
    crtOK->setColor(sf::Color::Green);
    crtOK->isEnabled = true;
}

void disOK()
{
    crtOK->setColor(sf::Color(230, 230, 230, 255));
    crtOK->isEnabled = false;
}

void enPLay()
{
    playBox->setColor(sf::Color(255, 200, 0));
    playBox->isEnabled = true;
    playBox->mainText.setString("PLAY");
}

void bluePLay()
{
    playBox->setColor(sf::Color(0, 38, 255));
    playBox->isEnabled = true;
    playBox->mainText.setString("CLOSE  ");
}

void disPlay()
{
    crtOK->setColor(sf::Color(230, 230, 230, 255));
    crtOK->isEnabled = false;
}

void tbOffFocused(TextBox * tb, int mink, int &last)
{
    if (tb->text == "" || parse(tb) < mink)
        tb->text = std::to_string(last), tb->updateCursor();
    else last = parse(tb);
}

void restartGame()
{
    drawingMap.setGame(fieldSize, players);
    mainField = drawingMap.drawGame(300, 80, FIELD_W);
}

void openDialog(int ai)
{
    enOK();
    dialScroll->mindPos = sf::Vector2f(WINDOW_W / 4, WINDOW_H / 4);
    windowMode = 3, selDialButton = selCol = 0, crtAI = ai;
    if (ai == 0)
    {
        crtName->text = "PLAYER" + std::to_string(newPlayerNum);
        selCol = rand() % 6 + 1;
    }
    else crtName->text = botNm[rand() % 19] + botNm[rand() % 20] + botNm[rand() % 20];
    crtW->isEnabled = crtA->isEnabled = crtS->isEnabled = crtD->isEnabled = !ai;
    crtColor->setColor(crtColors[selCol]);
    crtName->updateCursor();
}

void clearDialButtons()
{
    if (selDialButton == 1) crtW->setColor(sf::Color(225, 200, 0));
    else if (selDialButton == 2) crtA->setColor(sf::Color(225, 200, 0));
    else if (selDialButton == 3) crtS->setColor(sf::Color(225, 200, 0));
    else if (selDialButton == 4) crtD->setColor(sf::Color(225, 200, 0));
}

void addPlayer(sf::String playerName, int color, int ai)
{
    auto sn = new DrawingMap::Snake();
    sn->name = playerName;
    sn->dir = { 1, 0 };
    sn->from = { 1, 0 };
    sn->color = crtColors[color];
    int rnd = rand() % 50;
    sn->color.r += (sn->color.r < 128 ? 1 : -1) * rnd;
    sn->color.g += (sn->color.g < 128 ? 1 : -1) * rnd;
    sn->color.b += (sn->color.b < 128 ? 1 : -1) * rnd;
    sn->ai_mode = ai;
    sn->W = crtChW, sn->A = crtChA;
    sn->S = crtChS, sn->D = crtChD;
    nameList[players.size()]->text = playerName;
    nameList[players.size()]->updateCursor();
    nameList[players.size()]->resultBox.setFillColor(sf::Color(255, 255, 255));
    closeList[players.size()]->setColor(sn->color);
    players.push_back(sn);
    if (players.size() == MAX_PLAYERS)
        ldPlayer->isEnabled = ldBot->isEnabled = false;
    restartGame();
}

void deletePlayer(int k)
{
    if (players[k]) delete players[k];
    if (players.size() == MAX_PLAYERS)
        ldPlayer->isEnabled = ldBot->isEnabled = true;
    players.erase(players.begin() + k);
    for (size_t i = k; i < players.size(); ++i)
    {
        nameList[i]->text = players[i]->name;
        nameList[i]->updateCursor();
        closeList[i]->setColor(players[i]->color);
    }
    nameList[players.size()]->text = "Player " + std::to_string(players.size() + 1);
    nameList[players.size()]->resultBox.setFillColor(sf::Color(255, 255, 255, 128));
    nameList[players.size()]->updateCursor();
    restartGame();
}

void initLoadMenu()
{
    loadScroll = new ScrollWindow(sf::FloatRect(0, 0, WINDOW_W, WINDOW_H),
        sf::FloatRect(0, 0, LEFT_W, WINDOW_H));
    ldTb = owt.createBackground(sf::FloatRect(70, 20, 70, 30), "");
    ldSize = new TextBox("20", sf::FloatRect(120, 20, 50, 30), 50, tF);
    ldSize->isActive = true;
    ldLeft = new PxButton(sf::FloatRect(20, 32, 18, 18), sf::Color::Red, 1, "", bF);
    ldLeft->setSprite(owt.createBackground(ldLeft->control, "textures/left.png"));
    ldLeft->brightBox = 1, ldLeft->brightOver = 0.9, ldLeft->brightPress = 0.8;
    ldLeft->update();
    ldRight = new PxButton(sf::FloatRect(225, 32, 18, 18), sf::Color::Red, 1, "", bF);
    ldRight->setSprite(owt.createBackground(ldRight->control, "textures/right.png"));
    ldRight->brightBox = 1, ldRight->brightOver = 0.9, ldRight->brightPress = 0.8;
    ldRight->update();
    ldSt = owt.createBackground(sf::FloatRect(20, 80, 250, 50), "textures/settings.png");
    ldSet = new TextBox("APPLES:\n\nFREQUENCY:   \nPLAYERS:\n", sf::FloatRect(20, 150, 250, 200), 25, tF);
    ldSet->resultBox.setFillColor(sf::Color::White);
    ldAp = owt.createBackground(sf::FloatRect(200, 142, 30, 18), "");
    ldApples = new TextBox("1", sf::FloatRect(212, 145, 30, 18), 25, tF);
    ldApples->isActive = true;
    ldLn = owt.createBackground(sf::FloatRect(200, 202, 30, 18), "");
    ldLength = new TextBox("200", sf::FloatRect(212, 205, 40, 18), 25, tF);
    ldLength->isActive = true;
    ldPlayer = new PxButton(sf::FloatRect(185, 265, 18, 18), sf::Color(255, 200, 0), 10, "", tF);
    ldPlayer->setSprite(owt.createBackground(ldPlayer->control, "textures/player-add.png"));
    ldBot = new PxButton(sf::FloatRect(225, 265, 18, 18), sf::Color(255, 200, 0), 10, "", tF);
    ldBot->setSprite(owt.createBackground(ldBot->control, "textures/bot-add.png"));
    botScroll = new ScrollWindow(sf::FloatRect(0, 300, LEFT_W, 600),
        sf::FloatRect(0, 0, LEFT_W, WINDOW_H * 1.5));
    playBox = new PxButton(sf::FloatRect(500, 15, 50, 25), sf::Color(255, 200, 0), 20, "PLAY", bF);
    playBox->setSprite(owt.createBackground(playBox->control, ""));
}

void initPlayerList()
{
    colorFlag = owt.createBackground(sf::FloatRect(0, 0, 18, 18), "textures/player-back.png", false);
    for (size_t i = 0; i < MAX_PLAYERS; ++i)
    {
        TextBox * nm = new TextBox("Player " + std::to_string(i+1), sf::FloatRect(50, 20 + i * 50, 170, 18), 20, tF);
        nm->resultBox.setFillColor(sf::Color(255, 255, 255, 128));
        PxButton * cls = new PxButton(sf::FloatRect(240, 24 + i * 50, 9, 9), sf::Color(255, 200, 0), 5, "  ", tF);
        cls->setSprite(owt.createBackground(cls->control, "textures/delete.png"));
        cls->mainText.setFillColor(sf::Color::White);
        nameList.push_back(nm);
        closeList.push_back(cls);
    }
}

void initDial()
{
    dialScroll = new ScrollWindow(sf::FloatRect(0, 0, WINDOW_W, WINDOW_H),
        sf::FloatRect(WINDOW_W / 2, WINDOW_H / 2, WINDOW_W * 2 - DIAL_W * USS, WINDOW_H * 2 - DIAL_H * USS));
    crtTextBox = sf::FloatRect(dialPos.x + 120, dialPos.y + 13, 132, 20);
    crtName = new TextBox("PLAYER", sf::FloatRect(dialPos.x + 134, dialPos.y + 20, 132 * USS, 10), 20, bF);
    crtName->resultBox.setFillColor(sf::Color::White);
    crtName->isActive = true;
    crtSN = new TextBox("ENTER NAME:\nCOLOR:    \nCONRTOLS:", sf::FloatRect(dialPos.x + 20, dialPos.y + 20, 100, 30), 20, tF);
    crtSN->resultBox.setFillColor(sf::Color::White);
    crtBack = owt.createBackground(sf::FloatRect(dialPos.x, dialPos.y, DIAL_W, DIAL_H), "");
    crtBack->setColor(sf::Color(40, 40, 40, 255));
    crtTb = owt.createBackground(crtTextBox, "");
    crtTb->setColor(sf::Color(70, 70, 70, 255));
    crtClose = new PxButton(sf::FloatRect(dialPos.x + (DIAL_W - 89) * USS,
        dialPos.y + (DIAL_H - 23) * USS, 40, 16), sf::Color::Red, 25, "CLOSE", bF);
    crtClose->mainText.setFillColor(sf::Color::White);
    crtClose->setSprite(owt.createBackground(crtClose->control, ""));
    crtOK = new PxButton(sf::FloatRect(dialPos.x + (DIAL_W - 47) * USS,
        dialPos.y + (DIAL_H - 23) * USS, 40, 16), sf::Color::Green, 15, "OK!", bF);
    crtOK->mainText.setFillColor(sf::Color::White);
    crtOK->setSprite(owt.createBackground(crtOK->control, ""));
    crtColor = new PxButton(sf::FloatRect(dialPos.x + 120,
        dialPos.y + 64, 132, 15), sf::Color::White, 50, "", bF);
    crtColor->setSprite(owt.createBackground(crtColor->control, "textures/snake-color.png"));
    crtColor->setColor(sf::Color::Green);
    crtW = new PxButton(sf::FloatRect(dialPos.x + 220, dialPos.y + 100, 30, 13), sf::Color(255, 200, 0), 6, "W", tF);
    crtW->setSprite(owt.createBackground(crtW->control, "textures/key-button.png"));
    crtA = new PxButton(sf::FloatRect(dialPos.x + 158, dialPos.y + 128, 30, 13), sf::Color(255, 200, 0), 5, "A", tF);
    crtA->setSprite(owt.createBackground(crtA->control, "textures/key-button.png"));
    crtS = new PxButton(sf::FloatRect(dialPos.x + 220, dialPos.y + 128, 30, 13), sf::Color(255, 200, 0), 4, "S", tF);
    crtS->setSprite(owt.createBackground(crtS->control, "textures/key-button.png"));
    crtD = new PxButton(sf::FloatRect(dialPos.x + 282, dialPos.y + 128, 30, 13), sf::Color(255, 200, 0), 4, "D", tF);
    crtD->setSprite(owt.createBackground(crtD->control, "textures/key-button.png"));
}

bool cmp(DrawingMap::Snake * a, DrawingMap::Snake * b)
{
    return a->path.size() > b->path.size();
}

int main()
{
    bF.loadFromFile(owt.fontFamily);
    tF.loadFromFile(owt.textFontFamily);
    window = drawingMap.getWindow();
    initDial();
    initLoadMenu();
    initPlayerList();
    auto lastFrameTime = std::chrono::steady_clock::now();
    restartGame();
    while (window->isOpen())
    {
        sf::Event e;
        while (window->pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                delete crtClose; delete crtOK;
                delete crtW; delete crtA;
                delete crtS; delete crtD;
                delete crtColor; delete crtName;
                delete dialScroll; delete crtTb;
                delete ldApples; delete ldSize;
                delete ldLength; delete ldPlayer;
                delete ldBot; delete loadScroll;
                delete botScroll; delete ldTb;
                delete ldLeft; delete ldRight;
                delete ldSt;
                delete playBox;
                window->close();
                return 0;
            }
            int X = e.mouseButton.x, Y = e.mouseButton.y;
            if (windowMode == 1)
            {
                if (e.type == sf::Event::MouseMoved)
                {
                    botScroll->checkMove(mouseX - e.mouseMove.x, mouseY - e.mouseMove.y);
                    ldBot->checkOver(loadScroll->control, loadScroll->mindPos, e.mouseMove.x, e.mouseMove.y);
                    ldPlayer->checkOver(loadScroll->control, loadScroll->mindPos, e.mouseMove.x, e.mouseMove.y);
                    ldLeft->checkOver(loadScroll->control, loadScroll->mindPos, e.mouseMove.x, e.mouseMove.y);
                    ldRight->checkOver(loadScroll->control, loadScroll->mindPos, e.mouseMove.x, e.mouseMove.y);
                    for (size_t i = 0; i < players.size(); ++i)
                        closeList[i]->checkOver(botScroll->control, botScroll->mindPos, 
                            e.mouseMove.x, e.mouseMove.y);
                }
                else if (e.type == sf::Event::MouseWheelScrolled)
                {
                }
                else if (e.type == sf::Event::MouseButtonPressed)
                {
                    ldBot->checkPressed(loadScroll->control, loadScroll->mindPos, X, Y);
                    ldPlayer->checkPressed(loadScroll->control, loadScroll->mindPos, X, Y);
                    ldLeft->checkPressed(loadScroll->control, loadScroll->mindPos, X, Y);
                    ldRight->checkPressed(loadScroll->control, loadScroll->mindPos, X, Y);
                    if (ldSize->offFocused())
                    {
                        tbOffFocused(ldSize, 10, fieldSize);
                        restartGame();
                    }
                    if (ldApples->offFocused())
                        tbOffFocused(ldApples, 1, apples);
                    if (ldLength->offFocused())
                        tbOffFocused(ldLength, 1, length);
                    ldSize->checkPressed(loadScroll->control, loadScroll->mindPos, X, Y, false);
                    ldApples->checkPressed(loadScroll->control, loadScroll->mindPos, X, Y, false);
                    ldLength->checkPressed(loadScroll->control, loadScroll->mindPos, X, Y, false);
                    playBox->checkPressed(loadScroll->control, loadScroll->mindPos, X, Y);
                    if (botScroll->checkPressed(X, Y))
                        for (size_t i = 0; i < players.size(); ++i)
                            closeList[i]->checkPressed(botScroll->control, botScroll->mindPos, X, Y);
                }
                else if (e.type == sf::Event::MouseButtonReleased)
                {
                    if (ldLeft->checkReleased(loadScroll->control, loadScroll->mindPos, X, Y))
                        ldSize->text = std::to_string(--fieldSize), ldSize->updateCursor(), restartGame();
                    if(ldRight->checkReleased(loadScroll->control, loadScroll->mindPos, X, Y))
                        ldSize->text = std::to_string(++fieldSize), ldSize->updateCursor(), restartGame();
                    if (ldBot->checkReleased(loadScroll->control, loadScroll->mindPos, X, Y))
                        openDialog(1);
                    if(ldPlayer->checkReleased(loadScroll->control, loadScroll->mindPos, X, Y))
                        openDialog(0);
                    if (playBox->checkReleased(loadScroll->control, loadScroll->mindPos, X, Y))
                    {
                        gameFrameTime = std::chrono::steady_clock::now();
                        windowMode = 2;
                        oldPlayers.clear();
                        bluePLay();
                        for (int i = 0; i < players.size(); ++i)
                        {
                            oldPlayers.push_back(*players[i]);
                            closeSprites[i] = closeList[i]->backGround;
                            closeList[i]->backGround = nullptr;
                        }
                        restartGame();
                        for (int i = 0; i < apples; ++i)
                            drawingMap.addApple();
                    }
                    for (size_t i = 0; i < players.size(); ++i)
                        if (closeList[i]->checkReleased(botScroll->control, botScroll->mindPos, X, Y))
                            deletePlayer(i);
                    botScroll->isPressed = false;
                }
                else if (e.type == sf::Event::KeyPressed)
                {
                    if (ldSize->checkKeyPressed(e.key))
                    {
                        tbOffFocused(ldSize, 10, fieldSize);
                        if (e.key.code == sf::Keyboard::Enter)
                            restartGame();
                    }
                    if (ldApples->checkKeyPressed(e.key))
                        tbOffFocused(ldApples, 1, apples);
                    if (ldLength->checkKeyPressed(e.key))
                        tbOffFocused(ldLength, 1, length);
                    if (e.key.code == sf::Keyboard::F2)
                    {
                        
                    }
                    else if (e.key.code == sf::Keyboard::F3)
                    {
                        drawingMap.addApple();
                        mainField = drawingMap.drawGame(300, 80, FIELD_W);
                        bluePLay();
                    }
                }
                else if (e.type == sf::Event::TextEntered)
                {
                    if (e.text.unicode >= '0' && e.text.unicode <= '9')
                    {
                        ldSize->checkInput(e.text.unicode);
                        ldApples->checkInput(e.text.unicode);
                        ldLength->checkInput(e.text.unicode);
                    }
                }
            }
            else if (windowMode == 2)
            {
                if (e.type == sf::Event::KeyPressed)
                    drawingMap.checkKey(e.key.code);
                else if (e.type == sf::Event::MouseButtonPressed)
                {
                    playBox->checkPressed(loadScroll->control, loadScroll->mindPos, X, Y);
                }
                else if (e.type == sf::Event::MouseButtonReleased)
                {
                    if (playBox->checkReleased(loadScroll->control, loadScroll->mindPos, X, Y))
                    {
                        enPLay();
                        windowMode = 1;
                        for (int i = 0; i < players.size(); ++i)
                        {
                            closeList[i]->backGround = closeSprites[i];
                            closeList[i]->mainText.setString(" ");
                            delete players[i];
                        }
                        players.clear();
                        for (int i = 0; i < oldPlayers.size(); ++i)
                        {
                            crtChW = oldPlayers[i].W, crtChA = oldPlayers[i].A;
                            crtChS = oldPlayers[i].S, crtChD = oldPlayers[i].D;
                            addPlayer(oldPlayers[i].name, 0, oldPlayers[i].ai_mode);
                            players[i]->color = oldPlayers[i].color;
                        }
                        restartGame();
                    }
                }
            }
            else if (windowMode == 3)
            {
                if (e.type == sf::Event::MouseMoved)
                {
                    dialScroll->checkMove(mouseX - e.mouseMove.x, mouseY - e.mouseMove.y);
                }
                else if (e.type == sf::Event::MouseButtonPressed)
                {
                    if (e.mouseButton.button != sf::Mouse::Button::Left) continue;
                    clearDialButtons();
                    selDialButton = 0;
                    crtName->offFocused();
                    if (crtColor->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y))
                        selCol = (selCol + 1) % 7, crtColor->setColor(crtColors[selCol]);
                    crtClose->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y);
                    crtOK->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y);
                    dialScroll->checkPressed(X, Y);
                    if (crtTb->getGlobalBounds().contains(X, Y));
                    crtName->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y, true);
                    if (crtW->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y))
                        crtW->setColor(sf::Color::White), selDialButton = 1;
                    else if (crtA->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y))
                        crtA->setColor(sf::Color::White), selDialButton = 2;
                    else if (crtS->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y))
                        crtS->setColor(sf::Color::White), selDialButton = 3;
                    else if (crtD->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y))
                        crtD->setColor(sf::Color::White), selDialButton = 4;
                }
                else if (e.type == sf::Event::MouseButtonReleased)
                {
                    crtW->checkReleased(dialScroll->control, dialScroll->mindPos, X, Y);
                    crtA->checkReleased(dialScroll->control, dialScroll->mindPos, X, Y);
                    crtS->checkReleased(dialScroll->control, dialScroll->mindPos, X, Y);
                    crtD->checkReleased(dialScroll->control, dialScroll->mindPos, X, Y);
                    crtColor->checkReleased(dialScroll->control, dialScroll->mindPos, X, Y);
                    if (crtClose->checkReleased(dialScroll->control, dialScroll->mindPos, X, Y))
                    {
                        windowMode = 1;
                    }
                    else if (crtOK->checkReleased(dialScroll->control, dialScroll->mindPos, X, Y))
                    {
                        addPlayer(crtName->text, selCol, crtAI);
                        if (crtAI == 0) ++newPlayerNum;
                        windowMode = 1;
                    }
                    dialScroll->isPressed = false;
                }
                else if (e.type == sf::Event::KeyPressed)
                {
                    if (selDialButton == 1) {
                        crtChW = e.key.code + 'A', crtW->mainText.setString(crtChW);
                        crtW->setColor(sf::Color(255, 200, 0));
                        crtA->setColor(sf::Color::White);
                        selDialButton = 2;
                    } else if (selDialButton == 2) {
                        crtChA = e.key.code + 'A', crtA->mainText.setString(crtChA);
                        crtA->setColor(sf::Color(255, 200, 0));
                        crtS->setColor(sf::Color::White);
                        selDialButton = 3;
                    } else if (selDialButton == 3) {
                        crtChS = e.key.code + 'A', crtS->mainText.setString(crtChS);
                        crtS->setColor(sf::Color(255, 200, 0));
                        crtD->setColor(sf::Color::White);
                        selDialButton = 4;
                    } else if (selDialButton == 4) {
                        crtChD = e.key.code + 'A', crtD->mainText.setString(crtChD);
                        crtD->setColor(sf::Color(255, 200, 0));
                        selDialButton = 0;
                    }
                    crtName->checkKeyPressed(e.key);
                    if (crtName->text == "") disOK();
                }
                else if (e.type == sf::Event::TextEntered)
                {
                    crtName->checkInput(e.text.unicode);
                    if (crtName->text != "") enOK();
                }
            }
            if (e.type == sf::Event::MouseMoved)
            {
                mouseX = e.mouseMove.x;
                mouseY = e.mouseMove.y;
            }
        }
        ldLeft->isEnabled = (ldSize->text != "" && parse(ldSize) > 10);
        ldRight->isEnabled = (ldSize->text != "" && parse(ldSize) < 99);
    // Ending of frame //
        auto timeInterval = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastFrameTime);
        if (windowMode == 2)
        {
            auto gameInterval = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - gameFrameTime);
            if (gameInterval.count() > length)
            {
                gameFrameTime = std::chrono::steady_clock::now();
                drawingMap.forward();
                std::sort(players.begin(), players.end(), cmp);
                for (int i = 0; i < players.size(); ++i)
                {
                    nameList[i]->text = players[i]->name;
                    nameList[i]->updateCursor();
                    if (players[i]->path.size())
                    {
                        closeList[i]->backGround = nullptr;
                        closeList[i]->mainText.setString(std::to_string(players[i]->path.size()));
                        nameList[i]->resultBox.setFillColor(sf::Color::White);
                    }
                    else
                    {
                        closeList[i]->backGround = closeSprites[i];
                        closeList[i]->backGround->setColor(sf::Color::Black);
                        closeList[i]->mainText.setString(" ");
                        nameList[i]->resultBox.setFillColor(players[i]->color);
                    }
                }
                mainField = drawingMap.drawGame(300, 80, FIELD_W);
            }
        }
        if (timeInterval.count() > ProgramFrameTime)
        {
            lastFrameTime = std::chrono::steady_clock::now();
            ++frameTime;
        }
        else continue;
        window->clear(sf::Color(5, 5, 10, 255));
        loadScroll->openDrawing();
        if (windowMode == 1 || windowMode == 3)
        {
            botScroll->control.top = 300;
            ldSize->update();
            ldApples->update();
            ldLength->update();
            loadScroll->draw(ldTb);
            loadScroll->draw(ldSize, 6);
            loadScroll->draw(ldLeft);
            loadScroll->draw(ldRight);
            loadScroll->draw(ldSt);
            loadScroll->draw(ldSet, 0);
            loadScroll->draw(ldAp);
            loadScroll->draw(ldApples, 3);
            loadScroll->draw(ldLn);
            loadScroll->draw(ldLength, 3);
            loadScroll->draw(ldPlayer);
            loadScroll->draw(ldBot);
        }
        loadScroll->draw(playBox);
        loadScroll->draw(mainField);
        if (windowMode == 2)
        {
            botScroll->control.top = 40;
           // botScroll->control.height = 100;
        }
        loadScroll->uniteDrawing(window);
        // bots //
        botScroll->update();
        botScroll->openDrawing();
        colorFlag->setPosition(10, 8);
        for (size_t i = 0; i < MAX_PLAYERS; ++i)
        {
            botScroll->draw(nameList[i], 0);
            if (players.size() > i)
            {
                if(players[i]->path.size() < 10)
                    botScroll->draw(closeList[i], -15, 8);
                else if (players[i]->path.size() < 100)
                    botScroll->draw(closeList[i], -20, 8);
                else if (players[i]->path.size() < 1000)
                    botScroll->draw(closeList[i], -25, 8);
                colorFlag->setColor(players[i]->color);
            }
            else colorFlag->setColor(sf::Color(255, 255, 255, 128));
            botScroll->draw(colorFlag);
            colorFlag->move(0, 50);
        }
        botScroll->uniteDrawing(window);
        if (windowMode == 3)
        {
            // Update shapes //
            crtClose->checkOver(dialScroll->control, dialScroll->mindPos, mouseX, mouseY);
            crtOK->checkOver(dialScroll->control, dialScroll->mindPos, mouseX, mouseY);
            // Drawing shapes //

            crtName->update();
            dialScroll->openDrawing();
            dialScroll->update();
            dialScroll->draw(crtBack);
            dialScroll->draw(crtTb);
            dialScroll->draw(crtSN, 0);
            dialScroll->draw(crtName, 3);
            dialScroll->draw(crtColor);
            if (crtAI == 0)
            {
                dialScroll->draw(crtW);
                dialScroll->draw(crtA);
                dialScroll->draw(crtS);
                dialScroll->draw(crtD);
            }
            dialScroll->draw(crtOK);
            dialScroll->draw(crtClose);
            dialScroll->uniteDrawing(window);
        }
        window->display();
    }
}