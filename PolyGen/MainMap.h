#pragma once
#include "ScrollWindow.h"
#include "DataBase.h"
#include <queue>
#include <vector>
class MainMap
{
private:
    const int SHUFFLE_NUM = 3, APPL = 505, TAIL = 404;
    int size = 10;
    const float KK = 0.3;
public:
    const sf::String fontFamily = "fonts/ufonts.com_ydp-math-bold-italic.ttf";
    const sf::String boldFontFamily = "fonts/Nunito-bold.ttf";
    MainMap(uint16_t width, uint16_t height, const char* programName);
    ~MainMap();
    sf::RenderWindow* getWindow();
    sf::RenderWindow window;
    sf::Image frontMap;
    sf::Sprite mainGame;
// Left window
    DataBase * dataBase;
    ownUtilities * owt;
    int outCode = 0;
    void initBoxes(uint16_t WINDOW_W, uint16_t WINDOW_H, sf::Font &xF, sf::Font &bF, ownUtilities *ow);
    void drawBoxes();
    void drawBigButton(ScrollWindow * plot, PxButton * btn, sf::Sprite * cornerL, sf::Sprite * cornerR, sf::Sprite * body, bool aut = 1);
    void checkPressed(int X, int Y);
    void checkMoved(int X, int Y, float dx, float dy);
    void checkReleased(int X, int Y);
    void resizeBoxes(uint16_t WINDOW_W, uint16_t WINDOW_H);
    void checkKeyPressed(sf::Event::KeyEvent key);
    void checkTextEntered(sf::Event::TextEvent text);
    void updateMainColorMap();
	void offFocusVar();
	void updatePrecision();
private:
    uint16_t MI_W = 400, MI_H = 360, MI_CH = 10, MI_SH = 48, MI_L = 10, MI_R = 35, OP_CH = 150;
    uint16_t AP_W = 260, AP_H = 500, BT_H = 24, OP_H = 350, OP_L = 50, MI_A = 80, MI_OPL = 530;
    uint16_t BB_L = 540, BB_R = 490, VI_H = 400, MB_H = 25, XB_H = 35, PREC_W = 41;
    float frontArgs[8] = { 60, 23, 3, -10, 4, 10, 0, 0 };
	int selectedVar_i = -1;
    std::map<char, sf::Color> frontColors;

    int windowMode = 1, selecetdLibrary = -1, lastInput = 1, predHeight = 28;
	ClrBox* selectedVar = nullptr;
    bool isFormatted = true, selectedEmpty = false;
    uint16_t currentWidth, currentHeight, avTime = 0, avTimeClr = 0;
    sf::Font tf, xf, bf;
    sf::Sprite * miInputBack, * miInputBorders, * miLabelBorders, * formatOff, * formatOn;
	sf::Sprite* bigCornerL, * bigCornerR, * bigBody;
	sf::Sprite* minCornerL, * minCornerR, * minBody;
	sf::Sprite* midCornerL, * midCornerR, * midBody;
    sf::Sprite * copyPn, * copyAn, * miVarBack, * varLine;
    ScrollWindow * miScroll, *opScroll, * viScroll, * avScroll;
    ClrBox * miInput, * miLabelPn, * miAnswer, * miInputLb, * miAddText;
    ClrBox * opHeader, * viAddvar, * precInput;
    PxButton * miFormatBtn, * miCopyPn, * miCopyAns;
    PxButton * opStart, * precBack;
    bool isFloat(std::string str);
    void initInputBoxes(uint16_t WINDOW_W, uint16_t WINDOW_H);
    void initOperations(uint16_t WINDOW_W, uint16_t WINDOW_H);
    void initVarField(uint16_t WINDOW_W, uint16_t WINDOW_H);
};