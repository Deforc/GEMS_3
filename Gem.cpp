#include "Gem.h"

const sf::Vector2f Gem::size {gemSize, gemSize};

Gem::Gem(int column, int row, Gem::Type type, sf::Texture &texture, Gem::gemState state)
    : column(column), row(row), position(float((Gem::size.x+padding)*column), float((Gem::size.y+padding)*row)), type(type), gemstate(state), sprite(texture) {

    clicked = position;
    sprite.setTextureRect(sf::IntRect(static_cast<int>(type)*gemSize, 0, gemSize, gemSize));
    sprite.setColor(sf::Color(255, 255, 255, alpha));
    sprite.setPosition(position);

}

void Gem::draw(sf::RenderWindow &window) {
    if (gemstate != gemState::NEW) {
        sprite.setPosition(position);
        if (matchChance)
            sprite.setTextureRect(sf::IntRect(static_cast<int>(type)*gemSize, 2*gemSize, gemSize, gemSize));
        else
            sprite.setTextureRect(sf::IntRect(static_cast<int>(type)*gemSize, (gemstate == gemState::SELECTED) ? gemSize : 0, gemSize, gemSize));
        sprite.setColor(sf::Color(255, 255, 255, alpha));
        window.draw(sprite);
    }
}

bool Gem::checkClick(const sf::Vector2f &curpos) {
   if(gemstate == gemState::DELETED)
       return false;
   return sf::Rect<float>(position, size).contains(curpos);
}

void Gem::swapGems(Gem &neighbour) {
    std::swap(column,neighbour.column);
    std::swap(row, neighbour.row);

    clicked = sf::Vector2f (float((Gem::size.x+padding)*column), float((Gem::size.y+padding)*row));
    neighbour.clicked = sf::Vector2f (float((Gem::size.x+padding)*neighbour.column), float((Gem::size.y+padding)*neighbour.row));

    setGemState(gemState::MOVING);
    neighbour.setGemState(gemState::MOVING);
}

Gem::gemState Gem::gemStateUpdate() {
    switch (gemstate) {
        case gemState::MATCHED:
        {
            //Вызвать uniqueAction
            uniqueAction();
            setGemState(gemState::DELETED);
            break;
        }
        case gemState::MOVING:
        case gemState::DELETED:
        {
            auto move = [=](float& p, float t) -> bool
            {
                if (p == t) return false;
                if (p < t) p += std::min(5.0f, t-p);
                else p -= std::min(5.0f, p-t);
                return true;
            };
            if (!move(position.x, clicked.x) && !move(position.y, clicked.y) && gemstate == gemState::MOVING)
                setGemState(gemState::NONE);
            break;
        }
        default:
            break;
    }
    return gemstate;
}




