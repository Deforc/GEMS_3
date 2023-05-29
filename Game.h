#ifndef GEMS_3_GAME_H
#define GEMS_3_GAME_H
#include <iostream>
#include <iomanip>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "Engine.h"

class Game {
public:
    Game() : font(), gameOverText("GAME OVER!", font, 80)
    { gameOverText.setFillColor(sf::Color::Black); }
    ~Game() = default;

    bool init(unsigned int width, unsigned int height);
    void run();

private:
    void draw();

    sf::RenderWindow window;
    sf::View gemView, screenView;
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
    sf::Font font;
    sf::Text gameOverText;
    Engine engine;
};


#endif //GEMS_3_GAME_H