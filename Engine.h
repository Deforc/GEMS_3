#ifndef GEMS_3_ENGINE_H
#define GEMS_3_ENGINE_H
#include <vector>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <algorithm>
#include "Gem.h"
#include "Bomb.h"



 class Engine {
public:
    bool initialisation();
    void draw(sf::RenderWindow& window);
    void reset();
    void click(const sf::Vector2f& curpos);
    void update();
    bool findMatchChance(bool chance);
    bool gameOver();

    static constexpr int rows = 8;
    static constexpr int columns = 8;

    static int getRows() {return rows;}
    static int getColumns() {return columns;}

    Gem* getGem(int column, int row) {return gems[row*columns + column];}

private:
    enum class engineState : int {WAITING, SELECTED, SWAPPING, MOVING, LOSING};

    void setEngineState (engineState newstate) { enginestate = newstate; }

    int match();
    int match3(Gem* gem_one, Gem* gem_two, Gem* gem_three);
    bool checkMatchEvent(Gem* gem_one, Gem* gem_two, Gem* gem_three, bool chance);

    bool transition();

    void replaceDeleted();
    void clearMatchChance() {
        for (auto &gem : gems)
            gem->setMatchChance(false);
    }


    std::vector<Gem*> gems;
    sf::Texture textureGem;
    sf::Texture textureBomb;
    sf::Texture textureBrush;
    engineState enginestate = engineState::WAITING;
    size_t sel1;
    size_t sel2;




};



#endif //GEMS_3_ENGINE_H
