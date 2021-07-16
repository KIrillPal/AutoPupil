#pragma once
#include "DataBase.h"

int convert_from(char * inp)
{
    int result = 0;
    for (int i = 0; i < 4; ++i)
    {
        result <<= 8;
        result += *inp, ++inp;
    }
    return result;
}

void loadFromFile(char * fileName)
{
    std::ifstream fin(fileName);
    std::string input;
    while (fin >> input)
    {
        Polynomial p;
        if (input.size() < 4 || (input.size() - 4) % 8 != 0)
        {
            std::cout << "Error at input in " << fileName << "! Invalid format.\n";
            return;
        }
        p.serial = convert_from(&(input[0]));
        int n = (input.size() - 4) >> 3;
        for (int i = 0; i < n; ++i)
        {
        }
    }
    fin.close();
}

void saveToFile(char * fileName)
{
    std::ofstream fout(fileName);
    fout << char(0) << char(0) << char(1) << char(1);
    fout.close();
}
