#ifndef GEMS_3_BOMB_H
#define GEMS_3_BOMB_H
#include "Gem.h"
#include "Engine.h"

class Bomb : public Gem{
public:
    Bomb(int column, int row, Type type, sf::Texture& texture, gemState state, std::vector<Gem*>* gems) : Gem(column, row, type, texture, state) {
        this->gems = gems;
    }
    void uniqueAction() override;
private:
    std::vector<Gem*>* gems;
};


#endif //GEMS_3_BOMB_H
