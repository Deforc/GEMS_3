#ifndef GEMS_3_BRUSH_H
#define GEMS_3_BRUSH_H
#include "Gem.h"
#include "Engine.h"

class Brush : public Gem{
public:
    Brush(int column, int row, Type type, sf::Texture& texture, gemState state, std::vector<Gem*>* gems)
    : Gem(column, row, type, texture, state) {
        this->gems = gems;
    }
    ~Brush() = default;

    void uniqueAction() override;

private:
    std::vector<Gem*>* gems;
    void changeType(int paintedGem);
};


#endif //GEMS_3_BRUSH_H
