#include "ownUtilities.h"



ownUtilities::ownUtilities()
{}

sf::Sprite* ownUtilities::createBackground(sf::FloatRect box, sf::String texturePath = "", bool scaled)
{
    sf::Texture * back = new sf::Texture();
    if (texturePath == "")
    {
        const int BORDER = 3;
        sf::Image backImage;
        // Fill area //
        backImage.create(box.width, box.height);
        for (int i = 0; i < box.height; ++i)
            for (int j = 0; j < box.width; ++j)
                backImage.setPixel(j, i, sf::Color(204, 204, 204, 255));
        // Round corners //
        for (int corner = 0; corner < 4; ++corner)
        {
            int x0 = (corner & 1) * (box.width - 1);
            int y0 = ((corner & 2) >> 1) * (box.height - 1);
            for (int x = x0 - 1; x <= x0 + 1; ++x)
                for (int y = y0 - 1; y <= y0 + 1; ++y)
                    if ((x - x0)*(y - y0) == 0 && x >= 0 && y >= 0 && x < box.width && y < box.height)
                        backImage.setPixel(x, y, sf::Color(0, 0, 0, 0));
        }
        // Fill inner area //
        for (int i = BORDER; i < box.height - BORDER; ++i)
            for (int j = BORDER; j < box.width - BORDER; ++j)
                backImage.setPixel(j, i, sf::Color(255, 255, 255, 255));
        // Draw border lines //
        for (int i = BORDER; i < box.height - BORDER; ++i)
        {
            backImage.setPixel(BORDER - 1, i, sf::Color(228, 228, 228, 255));
            backImage.setPixel(box.width - BORDER, i, sf::Color(228, 228, 228, 255));
        }
        for (int j = BORDER; j < box.width - BORDER; ++j)
        {
            backImage.setPixel(j, BORDER - 1, sf::Color(228, 228, 228, 255));
            backImage.setPixel(j, box.height - BORDER, sf::Color(228, 228, 228, 255));
        }
        back->loadFromImage(backImage);
    }
    else back->loadFromFile(texturePath);
    back->setSmooth(false);
    sf::Sprite *res = new sf::Sprite(*back);
    res->setPosition(box.left, box.top);
    if(scaled) res->setScale(USS, USS);
    return res;
}

ownUtilities::~ownUtilities()
{
}


