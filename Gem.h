#ifndef GEMS_3_GEM_H
#define GEMS_3_GEM_H
#include <iostream>
#include "SFML/Graphics.hpp"


class Gem {
public:
    enum class Type : int
    {
        RED = 0, ORANGE = 1, YELLOW = 2, GREEN = 3, BLUE = 4, PURPLE = 5, WHITE = 6
    };
    enum class gemState : int
    {
        NEW, NONE, SELECTED, MATCHED, MOVING, DELETED
    };


    Gem(int column, int row, Type type, sf::Texture& texture, gemState state);
    ~Gem() = default;

    void draw(sf::RenderWindow& window);
    void swapGems(Gem& neighbour);
    bool checkClick(const sf::Vector2f& curpos);
    gemState gemStateUpdate();

    int getColumn() const { return column; }
    int getRow() const { return row; }
    Type getType()  { return type; }
    gemState getGemState() { return gemstate;}

    void setType(Type type) { this->type = type; }
    void setGemState(gemState state) { if(this->gemstate != gemState::DELETED) this->gemstate = state; }
    void setMatchChance(bool matchChance) { this->matchChance = matchChance; }

    virtual void uniqueAction() { std::cout << "TYPE: " << static_cast<int>(getType()) <<
    " ROW: "<< getRow() << " COLUMN: "<< getColumn() << std::endl; }

    static const sf::Vector2f size;
    static constexpr int padding = 1; //Внутренний отступ, как в CSS
    static constexpr int gemSize = 64;


protected:
    int column, row;
    sf::Vector2f position;
    sf::Vector2f clicked;
    sf::Sprite sprite;
    Type type;
    gemState gemstate;
    bool matchChance = false;
    int alpha = 255;

};


#endif //GEMS_3_GEM_H
