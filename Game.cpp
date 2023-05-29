#include "Game.h"

void Game::draw() {
    window.setView(screenView);
    window.draw(bgSprite);

    std::ostringstream sstr;
    gameOverText.setPosition(window.getSize().x / 2, window.getSize().y / 2);

    if (engine.gameOver()) {
        window.draw(gameOverText);
    }

    window.setView(gemView);
    engine.draw(window);

    window.display();
}

bool Game::init(unsigned int width, unsigned int height) {
    window.create(sf::VideoMode(width, height), "MATCH-3-GEMS");
    window.setFramerateLimit(60);

    screenView.reset(sf::FloatRect(0, 0, width, height));

    if (!bgTexture.loadFromFile("..\\resources\\background.jpg")) {
        std::cerr << "ERROR LOADING BACKGROUND IMAGE\n";
        return false;
    }
    if (!font.loadFromFile("..\\resources\\EightBits.ttf")) {
        std::cerr << "ERROR LOADING FONT\n";
        return false;
    }
    bgSprite.setTexture(bgTexture);
    bgSprite.setScale((float)width/bgTexture.getSize().x, (float)height/bgTexture.getSize().y);

    double edge = 64.0f*8 + 7;

    gemView.reset(sf::FloatRect(0, 0, edge, edge));
    gemView.setViewport(sf::FloatRect(0.05f, 0.05f, 0.9f, 0.9f));
    window.setView(gemView);
    engine.initialisation();
}

void Game::run() {
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                {
                    screenView.reset(sf::FloatRect(0.0f, 0.0f, event.size.width, event.size.height));
                    bgSprite.setScale((float)event.size.width / bgTexture.getSize().x, (float)event.size.height / bgTexture.getSize().y);
                    float aspect = (float)event.size.height / (float)event.size.width;
                    gemView.setViewport(sf::FloatRect(0.05f * aspect, 0.05f, 0.9f * aspect, 0.9f));
                    break;
                }
                case sf::Event::KeyReleased:
                    if (event.key.code == sf::Keyboard::Escape) window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        engine.click(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                    }
                    break;
                default:
                    break;
            }
        }

        engine.update();
        draw();


    }
}

