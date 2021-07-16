#pragma once
#include "MainMap.h"
#include <queue>
#include <cstring>
#include <algorithm>
sf::RenderWindow window;

MainMap::MainMap(uint16_t width, uint16_t height, const char* programName)
{
    window.create(sf::VideoMode(width, height), programName);
    currentWidth = width;
    currentHeight = height;
}

sf::RenderWindow* MainMap::getWindow()
{
    return &window;
}

void MainMap::initBoxes(uint16_t WINDOW_W, uint16_t WINDOW_H, sf::Font &xF, sf::Font &bF, ownUtilities *ow)
{
    xf = xF, tf = bF;
    bf.loadFromFile(boldFontFamily);
    owt = ow;
    frontColors['v'] = sf::Color(0, 0, 0);
    frontColors['s'] = sf::Color(0, 0, 0);
    frontColors['p'] = sf::Color(0, 101, 175);
	frontColors['('] = sf::Color(196, 84, 121);
	frontColors[')'] = sf::Color(196, 84, 121);
    frontColors['-'] = sf::Color(184, 184, 184);
    MI_W = WINDOW_W - MI_L - MI_R;
	miScroll = new ScrollWindow(sf::FloatRect(MI_L, MI_CH + predHeight, WINDOW_W - MI_L - MI_R + MI_A, MI_H),
		sf::FloatRect(0, 0, WINDOW_W - MI_L - MI_R + MI_A, MI_H));
    initInputBoxes(WINDOW_W, WINDOW_H);
    initOperations(WINDOW_W, WINDOW_H);
    updateMainColorMap();
}

void MainMap::initInputBoxes(uint16_t WINDOW_W, uint16_t WINDOW_H)
{
    miInputBorders = owt->createBackground(sf::FloatRect(10, 10, MI_W - 20, 48), "", 0);
	miInputBorders->setColor(sf::Color(40, 70, 70));

	miLabelBorders = owt->createBackground(sf::FloatRect(10, 10, MI_W - 20, 48), "", 0);
	miLabelBorders->setColor(sf::Color(120, 135, 135));

    miInputBack = owt->createBackground(sf::FloatRect(13, 13, MI_W - 26, 42), "textures/white.png", 0);
    miInputBack->setScale((MI_W - 26) / 16.f, 42 / 16.f);

    miInput = new ClrBox("", sf::FloatRect(28, 18, MI_W - 55, 42), 24, bf);
    miInput->isActive = true;
    miInput->setMovingView(true);
    miInput->colorDictionary = &frontColors;

	miLabelPn = new ClrBox("", sf::FloatRect(28, 82, MI_W - 55, 42), 24, bf);
	miLabelPn->setMovingView(true);
	miLabelPn->isActive = true;

	miAnswer = new ClrBox("", sf::FloatRect(28, 212, MI_W - 55, 42), 24, bf);
	miAnswer->setMovingView(true);
	miAnswer->isActive = true;

	precInput = new ClrBox("20", sf::FloatRect(MI_W - PREC_W - 5, 10, PREC_W - 10, 35), 16, bf);
	precInput->isActive = true;

	precBack = new PxButton(sf::FloatRect(MI_W - PREC_W - 10, 8, PREC_W, 25), sf::Color::White, 10, "", bf);
	precBack->setSprite(owt->createBackground(sf::FloatRect(MI_W - PREC_W, 10, PREC_W, 25), "textures/white.png", 0));
	precBack->setColor(sf::Color::White, 1, 0.9, 0.8);
	precBack->backGround->setScale(0, 0);

    miInputLb = new ClrBox("Write by keyboard or paste from library", sf::FloatRect(14, 58, MI_W, 42), 12, tf);
    miInputLb->resultBox.setFillColor(sf::Color(0, 104, 104));

    miFormatBtn = new PxButton(sf::FloatRect(MI_W, 22, 24, 24), sf::Color::White, 12, "", bf);
    miFormatBtn->setColor(sf::Color::White, 1, 0.93, 0.85);
    formatOff = owt->createBackground(miFormatBtn->control, "textures/format_off.png", 0);
	formatOn = owt->createBackground(miFormatBtn->control, "textures/format_on.png", 0);
	copyPn = owt->createBackground(miFormatBtn->control, "textures/copy.png", 0);
	copyAn = owt->createBackground(miFormatBtn->control, "textures/copy.png", 0);
    miFormatBtn->setSprite(formatOn);
    miFormatBtn->hasSpecialSprite = true;

	miCopyPn = new PxButton(sf::FloatRect(MI_W, 86, 94, 24), sf::Color::White, 12, "", bf);
	miCopyPn->setColor(sf::Color::White, 1, 0.93, 0.85);
	miCopyPn->setSprite(copyPn);
	miCopyPn->hasSpecialSprite = true;

	miCopyAns = new PxButton(sf::FloatRect(MI_W, 216, 24, 24), sf::Color::White, 12, "", bf);
	miCopyAns->setColor(sf::Color::White, 1, 0.93, 0.85);
	miCopyAns->setSprite(copyAn);
	miCopyAns->hasSpecialSprite = true;
}

void MainMap::initOperations(uint16_t WINDOW_W, uint16_t WINDOW_H)
{
	bigCornerL = owt->createBackground(sf::FloatRect(0, 0, 56, 56), "textures/big_btn.png", 0);
	bigCornerR = owt->createBackground(sf::FloatRect(0, 0, 56, 56), "textures/big_btn.png", 0);
	bigBody = owt->createBackground(sf::FloatRect(0, 0, 56, 56), "textures/white.png", 0);

	minCornerL = owt->createBackground(sf::FloatRect(0, 0, MB_H, MB_H), "textures/min_btn.png", 0);
	minCornerR = owt->createBackground(sf::FloatRect(0, 0, MB_H, MB_H), "textures/min_btn.png", 0);
	minBody = owt->createBackground(sf::FloatRect(0, 0, MB_H, MB_H), "textures/white.png", 0);

    opScroll = new ScrollWindow(sf::FloatRect(MI_L, MI_CH, WINDOW_W - MI_L - MI_R + 60, OP_H),
        sf::FloatRect(0, 0, WINDOW_W - MI_L - MI_R + 60, OP_H));
    opHeader = new ClrBox("Computing expression", sf::FloatRect(10, 10, 300, 40), 18, bf);

    opStart = new PxButton(sf::FloatRect(MI_L + (MI_W - 300) / 2, 163, 300, 56), sf::Color(255, 0, 85), 140, "GET ANSWER", bf);
    opStart->mainText.setFillColor(sf::Color::White);
    opStart->setSprite(owt->createBackground(sf::FloatRect(0, 0, 56, 56), "textures/white.png", 0));
    opStart->backGround->setScale(0, 0);
    opStart->setColor(sf::Color(206, 0, 68), 1, 0.9, 0.8);
    opStart->offColor = sf::Color(165, 0, 54);

	miVarBack = owt->createBackground(sf::FloatRect(MI_L + 13, 310, MI_W, VI_H), "textures/white.png", 0);
	miVarBack->setScale((MI_W - MI_L - 16) / 16.f, VI_H / 16.f);
	miVarBack->setColor(sf::Color(244, 244, 244));

	varLine = owt->createBackground(sf::FloatRect(MI_L + 23, 350, MI_W - 20, 1), "textures/white.png", 0);
	varLine->setScale((MI_W - MI_L - 36) / 16.f, 1 / 16.f);
	varLine->setColor(sf::Color(0, 0, 0, 120));

	viAddvar = new ClrBox("Add variable", sf::FloatRect(25, 310, 300, 40), 18, bf);

	initVarField(WINDOW_W, WINDOW_H);
}

void MainMap::initVarField(uint16_t WINDOW_W, uint16_t WINDOW_H)
{
	avScroll = new ScrollWindow(sf::FloatRect(150, 318, MI_W - 164, 25),
		sf::FloatRect(0, 0, MI_W - MI_L - 56, 25));
	viScroll = new ScrollWindow(sf::FloatRect(MI_L + 23, 360, MI_W - MI_L - 36, WINDOW_H - 360),
		sf::FloatRect(0, 0, MI_W - MI_L - 36, WINDOW_H - 360));

	midCornerL = owt->createBackground(sf::FloatRect(0, 0, XB_H, XB_H), "textures/mid_btn.png", 0);
	midCornerR = owt->createBackground(sf::FloatRect(0, 0, XB_H, XB_H), "textures/mid_btn.png", 0);
	midBody = owt->createBackground(sf::FloatRect(0, 0, XB_H, XB_H), "textures/white.png", 0);
}

void MainMap::drawBoxes()
{
	if (avTimeClr < avTime)
		avTimeClr = std::min(avTimeClr + 1, 20);
	else avTimeClr = avTime;
	if (avTime > 0) --avTime;
	if (avTime == 20)
	{
		float w = dataBase->update_varButtons();
		avScroll->mindSize.x = std::max(avScroll->control.width, w);
		avScroll->mindPos.x = 0;
	}
	avScroll->update();
	viScroll->update();

	if (selectedVar) 
		selectedVar->update();
    miScroll->openDrawing();
    miScroll->draw(miInputBorders, 10, 10);
	miScroll->draw(miInputBack, 13, 13);
	miScroll->draw(miLabelBorders, 10, 74);
	miScroll->draw(miInputBack, 13, 77);
	miScroll->draw(miLabelBorders, 10, 204);
	miScroll->draw(miInputBack, 13, 207);

	miInput->update();
	miLabelPn->update();
	miAnswer->update();
	precInput->update();
    miScroll->draw(miInput, 4);
	miScroll->draw(miInputLb, 1);
	miScroll->draw(miLabelPn, 4);
	miScroll->draw(miAnswer, 4);
	miScroll->draw(miFormatBtn);
	miScroll->draw(miCopyPn);
	miScroll->draw(miCopyAns);

    miScroll->uniteDrawing(&window);
	window.draw(*miVarBack);
	window.draw(*varLine);
	
    opScroll->openDrawing();
	opScroll->draw(opHeader, 0);
    opStart->setPosition(MI_L + (MI_W - 300) / 2, 163);
	drawBigButton(opScroll, opStart, bigCornerL, bigCornerR, bigBody);
	opScroll->draw(viAddvar, 0);

	drawBigButton(opScroll, precBack, minCornerL, minCornerR, minBody);
	opScroll->draw(precInput, 2);
    opScroll->uniteDrawing(&window);

	avScroll->openDrawing();
	for (auto btn : dataBase->varButtons)
	{
		drawBigButton(avScroll, btn, minCornerL, minCornerR, minBody, 0);
		avScroll->draw(btn, 0, 0, 2);
	}
	avScroll->uniteDrawing(&window);

	if (avTimeClr)
	{
		sf::RectangleShape white_rect = sf::RectangleShape(sf::Vector2f(MI_W - 164, 25));
		white_rect.setFillColor(sf::Color(244, 244, 244, std::min(255, avTimeClr * 20)));
		white_rect.setPosition(150, 318);
		window.draw(white_rect);
	}

	viScroll->openDrawing();
	for (int i = 0; i < dataBase->varBacks.size(); ++i)
	{
		auto back = dataBase->varBacks[i];
		drawBigButton(viScroll, back, midCornerL, midCornerR, midBody, 0);
		viScroll->draw(back, -1, 12, 2);
		viScroll->draw(dataBase->varValues[i], 0);
	}
	viScroll->uniteDrawing(&window);

}

void MainMap::drawBigButton(ScrollWindow* plot, PxButton* btn, sf::Sprite* cornerL, sf::Sprite* cornerR, sf::Sprite* body, bool aut)
{
	btn->prepareBack(cornerL, cornerR, body);
	plot->draw(cornerL);
	plot->draw(cornerR);
	plot->draw(body);
	if(aut) plot->draw(btn);
}

void MainMap::checkPressed(int X, int Y)
{
	if (selectedVar)
		offFocusVar();
    if (windowMode == 1)
    {
		miInput->offFocused();
		miLabelPn->offFocused();
		miAnswer->offFocused();
		if (precInput->offFocused())
			updatePrecision();
        if (miInput->checkPressed(miScroll->control, miScroll->mindPos, X, Y, 0)) lastInput = 1;
		miLabelPn->checkPressed(miScroll->control, miScroll->mindPos, X, Y, 1);
		miAnswer->checkPressed(miScroll->control, miScroll->mindPos, X, Y, 1);
		precInput->checkPressed(opScroll->control, miScroll->mindPos, X, Y, 1);

		miFormatBtn->checkPressed(miScroll->control, miScroll->mindPos, X, Y);
		miCopyPn->checkPressed(miScroll->control, miScroll->mindPos, X, Y);
		miCopyAns->checkPressed(miScroll->control, miScroll->mindPos, X, Y);
        opStart->checkPressed(opScroll->control, opScroll->mindPos, X, Y);

		avScroll->checkPressed(X, Y);
		for (auto btn : dataBase->varButtons)
			btn->checkPressed(avScroll->control, avScroll->mindPos, X, Y);

		if (viScroll->checkPressed(X, Y))
		{
			int ind = 0;
			for (auto txb : dataBase->varValues)
			{
				if (txb->checkPressed(viScroll->control, viScroll->mindPos, X, Y, 1))
				{
					selectedVar = txb;
					selectedVar_i = ind;
					selectedEmpty = dataBase->update_varBox(selectedVar);
					break;
				}
				++ind;
			}
		}
    }
}

void MainMap::checkMoved(int X, int Y, float dx, float dy)
{
    if (windowMode == 1)
    {
        miFormatBtn->checkOver(miScroll->control, miScroll->mindPos, X, Y);
		miCopyPn->checkOver(miScroll->control, miScroll->mindPos, X, Y);
		miCopyAns->checkOver(miScroll->control, miScroll->mindPos, X, Y);
		opStart->checkOver(opScroll->control, opScroll->mindPos, X, Y);

		avScroll->checkMove(dx, dy);
		viScroll->checkMove(dx, dy);
		for (auto btn : dataBase->varButtons)
			btn->checkOver(avScroll->control, avScroll->mindPos, X, Y);
    }
}

void MainMap::checkReleased(int X, int Y)
{
    if (windowMode == 1)
    {
		avScroll->isPressed = false;
		viScroll->isPressed = false;
        if (miFormatBtn->checkReleased(miScroll->control, miScroll->mindPos, X, Y))
        {
            miFormatBtn->setSprite(isFormatted ? formatOff : formatOn);
            isFormatted ^= 1;
            updateMainColorMap();
        }
		else if (miCopyPn->checkReleased(miScroll->control, miScroll->mindPos, X, Y))
		{
			sf::Clipboard::setString(miLabelPn->text);
		}
		else if (miCopyAns->checkReleased(miScroll->control, miScroll->mindPos, X, Y))
		{
			sf::Clipboard::setString(miAnswer->text);
		}
        else if (opStart->checkReleased(opScroll->control, opScroll->mindPos, X, Y))
		{
			miAnswer->setString(dataBase->compute_all());
			updateMainColorMap();
		}
		else for (int i = 0; i < dataBase->varButtons.size(); ++i)
			if (dataBase->varButtons[i]->checkReleased(avScroll->control, avScroll->mindPos, X, Y))
			{
				float y = dataBase->create_Variable(i, MI_W - MI_L - 36);
				dataBase->varValues.back()->colorDictionary = &frontColors;
				dataBase->varValues.back()->redrawTexture();
				viScroll->mindSize.y =	std::max(viScroll->mindSize.y, y);
				updateMainColorMap();
				avTime = 0;
			}
    }
}

void MainMap::checkKeyPressed(sf::Event::KeyEvent key)
{
    if (windowMode == 1)
    {
        if (miInput->checkKeyPressed(key, [](char c) {return true;}))
            updateMainColorMap();
		if (selectedVar)
		{
			if(selectedVar->checkKeyPressed(key, [](char c) {return true; }))
				selectedEmpty = dataBase->update_varBox(selectedVar);
			if (!selectedVar->isFocused)
				offFocusVar();
		}
		if (precInput->isFocused && key.code == sf::Keyboard::Enter)
			updatePrecision();
		precInput->checkKeyPressed(key, [](char c) {return true; });
		if (key.code == sf::Keyboard::Left || key.code == sf::Keyboard::Right)
		{
			miLabelPn->checkKeyPressed(key, [](char c) {return true; });
			miAnswer->checkKeyPressed(key, [](char c) {return true; });
		}
    }
}

void MainMap::checkTextEntered(sf::Event::TextEvent text)
{
    if (windowMode == 1)
    {
		if (miInput->checkInput(text.unicode, [](char c) {return c >= 32 && c <= 128; }))
			updateMainColorMap();
		if (selectedVar && selectedVar->checkInput(text.unicode, [](char c) {return c >= 32 && c <= 128;}))
			selectedEmpty = dataBase->update_varBox(selectedVar);
		if (precInput->isFocused && precInput->text.getSize() < 3)
		{
			precInput->checkInput(text.unicode, [](char c) {return c >= '0' && c <= '9'; });
		}
    }
}

void MainMap::resizeBoxes(uint16_t WINDOW_W, uint16_t WINDOW_H)
{
    delete miScroll;
    MI_W = std::max(WINDOW_W - MI_L - MI_R, 280);
    miScroll = new ScrollWindow(sf::FloatRect(MI_L, MI_CH + predHeight, MI_W + MI_A, MI_H),
        sf::FloatRect(0, 0, MI_W + MI_A, MI_H));
    miInputBorders = owt->createBackground(sf::FloatRect(10, 10, MI_W - 20, 48), "", 0);
    miInputBorders->setColor(sf::Color(40, 70, 70));
	miLabelBorders = owt->createBackground(sf::FloatRect(10, 10, MI_W - 20, 48), "", 0);
	miLabelBorders->setColor(sf::Color(120, 135, 135));
    miInputBack->setScale((MI_W - 26) / 16.f, 42 / 16.f);
	miFormatBtn->setPosition(MI_W, 22);
	miCopyPn->setPosition(MI_W, 86);
	miCopyAns->setPosition(MI_W, 216);

	miInput->bounds.width = MI_W - 55;
	miInput->setMovingView(false);
	miInput->setMovingView(true);
	miInput->redrawTexture();

	miLabelPn->bounds.width = MI_W - 55;
	miLabelPn->setMovingView(false);
	miLabelPn->setMovingView(true);
	miLabelPn->redrawTexture();

	miAnswer->bounds.width = MI_W - 55;
	miAnswer->setMovingView(false);
	miAnswer->setMovingView(true);
	miAnswer->redrawTexture();

    delete opScroll;
    opScroll = new ScrollWindow(sf::FloatRect(MI_L, MI_CH, MI_W + MI_A, OP_H),
        sf::FloatRect(0, 0, MI_W + MI_A, OP_H));

    opStart->setPosition(MI_L + (MI_W - 300) / 2, 163);

	miVarBack->setScale((MI_W - MI_L - 16) / 16.f, VI_H / 16.f);
	varLine->setScale((MI_W - MI_L - 36) / 16.f, 1 / 16.f);

	delete avScroll;
	avScroll = new ScrollWindow(sf::FloatRect(150, 318, MI_W - 164, 25),
		sf::FloatRect(0, 0, MI_W - MI_L - 56, 25));

	delete viScroll;
	viScroll = new ScrollWindow(sf::FloatRect(MI_L + 23, 360, MI_W - MI_L - 36, WINDOW_H - 360),
		sf::FloatRect(0, 0, MI_W - MI_L - 36, WINDOW_H - 360));
    updateMainColorMap();
}


void MainMap::updateMainColorMap()
{
	avTime = 50;
	std::set<std::string> add_vars;
	dataBase->fill_color(miInput->text.toAnsiString(), miInput->colorMap);
    if (!isFormatted) miInput->colorMap = "";
    if (miInput->text.isEmpty())
        miInputLb->text = "Write by keyboard or paste from clipboard";
    else if (miInput->text.getSize() < 5 || miInput->colorMap[0] == '-')
            miInputLb->text = "Example: 3x^2*(14y-3) | Cyan - constant | Gray - symbols not included";
        else miInputLb->text = "";
    miInputLb->updateText();
    miInput->redrawTexture();

	std::string req = "";
	dataBase->update_polis(dataBase->inputPartList, dataBase->inputPolis);
	for (auto d : dataBase->inputPolis)
	{
		req += d.second + ' ';
	}
	miLabelPn->setString(req);
}

void MainMap::offFocusVar()
{
	if (selectedVar->text == "")
		dataBase->delete_varBox(selectedVar_i);
	else
	{
		if (selectedEmpty)
			selectedVar->setString("0");
		selectedVar->offFocused();
		selectedVar->isCursorShowed = false;
		dataBase->update_variable(selectedVar_i);
	}
	selectedVar_i = -1;
	selectedVar = nullptr;
	updateMainColorMap();
}

void MainMap::updatePrecision()
{
	while (precInput->text.getSize() && precInput->text[0] == '0')
		precInput->text.erase(0, 1);
	if (precInput->text == "") precInput->text = "20";
	precInput->setString(precInput->text);
	int prec = atoi(precInput->text.toAnsiString().c_str());
	BigFixed::set_precision(prec);
	BigFixed::E = BigFixed(dataBase->variables["e"]);
	BigFixed::PI = BigFixed(dataBase->variables["pi"]);
}

bool MainMap::isFloat(std::string str)
{
    int l = 0, r = str.size();
    while (l < str.size() && str[l] == ' ') ++l;
    while (r >= 1 && str[r-1] == ' ') --r;
    if (r <= l) return false;
    str = str.substr(l, r - l);
    if (str.find_first_not_of("0123456789.+-") != -1)
        return false;
    int d = str.find_last_not_of("0123456789.");
    if (d > 0) return false;
    int p = str.find_first_of('.');
    if (str.size() - (d > -1) - (p > -1) > 9) return false;
    if (p == -1) return true;
    if (p <= d + 1 || p == str.size() - 1) return false;
    str[p] = '0';
    if (str.find_first_of('.') != -1) return false;
    return true;
}

MainMap::~MainMap()
{
    delete miScroll; delete miInputBack; delete miInputBorders;
	delete miInputLb; delete miLabelPn; delete miAnswer;
	delete miInput; miAnswer; delete miAddText;
    delete opScroll; delete opHeader; delete miFormatBtn; 
	delete formatOff; delete formatOn; delete opStart;
	delete copyPn; delete copyAn; delete bigCornerL; 
	delete bigCornerR; delete miVarBack; delete viAddvar;
	delete bigBody; delete miCopyPn; delete miCopyAns;
	delete minCornerL; delete minCornerR; delete minBody;
	delete midCornerL; delete midCornerR; delete midBody;
	delete viScroll; delete avScroll; delete precInput;
	delete precBack;
}