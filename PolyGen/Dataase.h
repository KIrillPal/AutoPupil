#pragma once
#include "PxButton.h"
#include <list>
#include <fstream>
#include <iostream>
#include <string>
struct Polynomial {
    uint16_t serial = 0;
    std::list<int> chars;
};
std::list < std::pair<char *, std::list<Polynomial> > > dataBase;
std::list <PxButton> buttonBase;

void loadFromFile(char * fileName);
void saveToFile(char * fileName);