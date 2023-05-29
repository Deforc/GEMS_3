#ifndef GEMS_3_BOMB_H
#define GEMS_3_BOMB_H
#include "Gem.h"

class Bomb : public Gem{
public:
    Bomb(int column, int row, Type type, sf::Texture& texture, gemState state) : Gem(column, row, type, texture, state) {}
};


#endif //GEMS_3_BOMB_H
