#include "Engine.h"

bool Engine::initialisation() {
    if (!textureGem.loadFromFile("..\\resources\\gems.png")
     || !textureBomb.loadFromFile("..\\resources\\bombs.png")
     || !textureBrush.loadFromFile("..\\resources\\brush.png")) {
        std::cerr << "ERROR LOADING IMAGE\n";
        return false;
    }
    srand(time(0));
    reset();
    return true;
}

void Engine::draw(sf::RenderWindow &window) {
    for (auto& gem : gems)
        gem->draw(window);
}

void Engine::reset() {
    gems.clear();
    for (int v = 0; v < rows; ++v)
        for(int u = 0; u < columns; ++u)
        {
            if(rand()%25 == 1)//Bomb
            {
                gems.emplace_back(new Bomb( u, v, static_cast<Gem::Type>(rand() % 7), textureBomb, Gem::gemState::NONE,&gems));
            }
            else if(rand()%25 == 2)//Brush
            {
                gems.emplace_back(new Brush(u, v, static_cast<Gem::Type>(rand()%7), textureBrush, Gem::gemState::NONE, &gems));
            }
            else//Gem
                gems.emplace_back(new Gem(u, v, static_cast<Gem::Type>(rand()%7), textureGem, Gem::gemState::NONE));
        }

    setEngineState(engineState::WAITING);
}

void Engine::click(const sf::Vector2f& curpos) {
    if(enginestate != engineState::WAITING && enginestate != engineState::SELECTED) return;
    for (auto& gem : gems) {
        if(gem->checkClick(curpos)) {
            if(enginestate == engineState::SELECTED)
            {
                clearMatchChance();
                sel2 = gem->getRow()*columns + gem->getColumn();
                auto& neighbour = gems[sel1];
                if((abs(gem->getColumn()-neighbour->getColumn()) + abs(gem->getRow()-neighbour->getRow())) != 1) break;

                std::iter_swap(&gem, &neighbour);
                neighbour->swapGems(*gem);
                setEngineState(engineState::SWAPPING);
            } else {
                sel1 = gem->getRow()*columns + gem->getColumn();
                gem->setGemState(Gem::gemState::SELECTED);
                setEngineState(engineState::SELECTED);
            }
            break;
        }
    }
}
int Engine::checkMatch(Gem *gem_one, Gem *gem_two, Gem *gem_three) {
    if(gem_one->getGemState() != Gem::gemState::NONE && gem_one->getGemState() != Gem::gemState::MATCHED
        && gem_two->getGemState() != Gem::gemState::NONE && gem_two->getGemState() != Gem::gemState::MATCHED
        && gem_three->getGemState() != Gem::gemState::NONE && gem_three->getGemState() != Gem::gemState::MATCHED)
        return 0;
    int match = 0;
    if(gem_one->getType() == gem_two->getType() && gem_one->getType() == gem_three->getType())
    {
        if(gem_one->getGemState() != Gem::gemState::MATCHED)
        {
            ++match;
            gem_one->setGemState(Gem::gemState::MATCHED);
        }
        if(gem_two->getGemState() != Gem::gemState::MATCHED)
        {
            ++match;
            gem_two->setGemState(Gem::gemState::MATCHED);
        }
        if(gem_three->getGemState() != Gem::gemState::MATCHED)
        {
            ++match;
            gem_three->setGemState(Gem::gemState::MATCHED);
        }
    }
    return match;
}

int Engine::checkMatchField() {
    int match = 0;
    auto gem = gems.begin();
    for(int v = 0; v < rows; ++v)
    {
        for (int u = 0; u < columns; ++u, ++gem)
        {
            if(u > 0 && u < (columns - 1))
                match += checkMatch(&**gem, &**(gem + 1), &**(gem - 1));
            if(v > 0 && v < (rows - 1))
                match += checkMatch(&**gem, &**(gem + columns), &**(gem - columns));
        }
    }
    return match;

}

bool Engine::checkMatchEvent(Gem *gem_one, Gem *gem_two, Gem *gem_three, bool chance) {
    if (gem_one->getType() == gem_three->getType()) {
        if (chance)
        {
            gem_one->setMatchChance(true);
            gem_two->setMatchChance(true);
            gem_three->setMatchChance(true);
        }
        return true;
    }
    return false;
}

void Engine::replaceDeleted()
{

    for(auto gem = gems.begin(); gem != gems.end(); ++gem) {
        if (gem.operator*()->getGemState() == Gem::gemState::DELETED) {
            if(rand()%25 == 1) //Bomb
                *gem = new Bomb(gem.operator*()->getColumn(), gem.operator*()->getRow(), static_cast<Gem::Type>(rand() % 7), textureBomb, Gem::gemState::NEW, &gems);
            else if(rand()%25 == 2) //Brush
                *gem = new Brush(gem.operator*()->getColumn(), gem.operator*()->getRow(), static_cast<Gem::Type>(rand() % 7), textureBrush, Gem::gemState::NEW, &gems);
            else //Gem
                *gem.operator*() = Gem(gem.operator*()->getColumn(), gem.operator*()->getRow(), static_cast<Gem::Type>(rand() % 7), textureGem, Gem::gemState::NEW);

        }
    }
}

bool Engine::transition()
{
    bool transiting = false;
    for(auto gem = gems.rbegin(); gem != gems.rend(); gem++) {
        if (gem.operator*()->getGemState() != Gem::gemState::DELETED) continue;
        if (gem.operator*()->getRow() == 0) break;
        transiting = true;
        for (int row = 1; row <= gem.operator*()->getRow(); row++) {
            if ((gem+(columns*row)).operator*()->getGemState() != Gem::gemState::DELETED) {
                std::iter_swap(gem, (gem+(columns*row)));
                gem.operator*()->swapGems(*(gem+(columns*row)).operator*());
                break;
            }
        }
    }
    replaceDeleted();
    return transiting;
}

bool Engine::findMatchChance(bool chance) {
    if (enginestate != engineState::WAITING) return false;
    clearMatchChance();
    for (int v = rows-1; v >= 0; v--) {
        for (int u = 0; u < columns; u++) {
            Gem* gem = gems[v*columns + u];
            Gem::Type type = gem->getType();

            //   3
            // 1 g 2
            //   3
            if (u < columns-2 && (gem+2)->getType() == type) {
                if (v > 0 && checkMatchEvent(gem, gem + 2, gem + 1 - columns, chance)) return true;
                if (v < rows-1 && checkMatchEvent(gem, gem + 2, gem + 1 + columns, chance)) return true;
            }

            //   1
            // 3 g 3
            //   2
            if (v < rows-2 && (gem+2*columns)->getType() == type) {
                if (u > 0 && checkMatchEvent(gem, gem + 2 * columns, gem - 1 + columns, chance)) return true;
                if (u < columns-1 && checkMatchEvent(gem, gem + 2 * columns, gem + 1 + columns, chance)) return true;
            }

            //     3
            // 1 2 g 3
            //     3
            if (u < columns-2 && (gem+1)->getType() == type) {
                if (v > 0 && checkMatchEvent(gem, gem + 1, gem + 2 - columns, chance)) return true;
                if (v < rows-1 && checkMatchEvent(gem, gem + 1, gem + 2 + columns, chance)) return true;
                if (u < columns-3 && checkMatchEvent(gem, gem + 1, gem + 3, chance)) return true;
            }

            //   3
            // 3 g 2 1
            //   3
            if (u > 1 && (gem-1)->getType() == type) {
                if (v > 0 && checkMatchEvent(gem, gem - 1, gem - 2 - columns, chance)) return true;
                if (v < rows-1 && checkMatchEvent(gem, gem - 1, gem - 2 + columns, chance)) return true;
                if (u > 2 && checkMatchEvent(gem, gem - 1, gem - 3, chance)) return true;
            }

            //   1
            //   2
            // 3 g 3
            //   3
            if (v < rows-2 && (gem+columns)->getType() == type) {
                if (u > 0 && checkMatchEvent(gem, gem + columns, gem - 1 + 2 * columns, chance)) return true;
                if (u < columns-1 && checkMatchEvent(gem, gem + columns, gem + 1 + 2 * columns, chance)) return true;
                if (v < rows-3 && checkMatchEvent(gem, gem + columns, gem + 3 * columns, chance)) return true;
            }

            //   3
            // 3 g 3
            //   2
            //   1
            if (v > 1 && (gem-columns)->getType() == type) {
                if (u > 0 && checkMatchEvent(gem, gem - columns, gem - 1 - 2 * columns, chance)) return true;
                if (u < columns-1 && checkMatchEvent(gem, gem - columns, gem + 1 - 2 * columns, chance)) return true;
                if (v > 2 && checkMatchEvent(gem, gem - columns, gem - 3 * columns, chance)) return true;
            }
        }
    }
    return false;
}

void Engine::update() {
    if (enginestate == engineState::WAITING) {
        for (auto &gem: gems)
            if (gem->getGemState() == Gem::gemState::NEW)
                gem->setGemState(Gem::gemState::NONE);

        int matchGem = checkMatchField();
        if(matchGem > 0)
            setEngineState(engineState::MOVING);
        else if (transition())
            setEngineState(engineState::MOVING);
        else if (!findMatchChance(false) && enginestate != engineState::MOVING && enginestate != engineState::SWAPPING)
            setEngineState(engineState::LOSING);
    }

    if (enginestate == engineState::MOVING || enginestate == engineState::SWAPPING) {
        bool moving = false;
        for (auto& gem : gems) {
            Gem::gemState newGemState = gem->gemStateUpdate();
            if (newGemState == Gem::gemState::MOVING) moving = true;
        }
        if (!moving) {
            if (enginestate == engineState::SWAPPING) {
                int matchGem = checkMatchField();
                if (matchGem == 0) {
                    auto& gem = gems[sel1];
                    auto& neighbour = gems[sel2];
                    std::iter_swap(&gem, &neighbour);
                    neighbour->swapGems(*gem);
                }
                setEngineState(engineState::MOVING);
            } else {
                setEngineState(engineState::WAITING);
            }
        }
    }


}



