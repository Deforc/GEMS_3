#include "Brush.h"

void Brush::changeType(int paintedGem) {
    switch (paintedGem) {
        case 0 :
            if(column != 0 || row != 0)
                gems[0][(row-1)*Engine::getColumns() + column - 1]->setType(getType());
            break;
        case 1:
            if(column != Engine::getColumns() - 1 || row != 0)
                gems[0][(row-1)*Engine::getColumns() + column + 1]->setType(getType());
            break;
        case 2:
            if(column != 0 - 1 || row != Engine::getRows()-1)
                gems[0][(row+1)*Engine::getColumns() + column - 1]->setType(getType());
            break;
        case 3:
            if(column != Engine::getColumns() - 1 || row != Engine::getRows()-1)
                gems[0][(row+1)*Engine::getColumns() + column + 1]->setType(getType());
            break;
    }
}

void Brush::uniqueAction() {
    int firstPaintedGem = rand()%4;
    int secondPaintedGem = rand()%4;
    while (firstPaintedGem == secondPaintedGem)
        secondPaintedGem = rand()%4;
    changeType(firstPaintedGem);
    changeType(secondPaintedGem);

    std::cout << "PAINTED" << std::endl;

}