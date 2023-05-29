#include "Bomb.h"
void Bomb::uniqueAction() {

    if(column != 0)
        gems[0][row*Engine::getColumns() + column - 1]->setGemState(Gem::gemState::DELETED);
    if(column != Engine::getColumns()-1)
        gems[0][row*Engine::getColumns() + column + 1]->setGemState(Gem::gemState::DELETED);
    if(row != 0)
        gems[0][row*Engine::getColumns() + column - Engine::getRows()]->setGemState(Gem::gemState::DELETED);
    if(row != Engine::getRows()-1)
        gems[0][row*Engine::getColumns() + column + Engine::getRows()]->setGemState(Gem::gemState::DELETED);


    std::cout << "BOOOOOM!" << std::endl;

}