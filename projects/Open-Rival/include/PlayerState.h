#pragma once

#include "Race.h"

namespace Rival {

class PlayerState
{
public:
    PlayerState(Race race, int gold, int wood, int food);

    Race getRace() const
    {
        return race;
    }

private:
    Race race;
    int gold;
    int wood;
    int food;
};

/**
 * Interface providing access to player data.
 */
class PlayerStore
{
public:
    virtual PlayerState& getLocalPlayerState() const = 0;
    virtual PlayerState* getPlayerState(int playerId) const = 0;
    virtual bool isLocalPlayer(int playerId) const = 0;
};

}  // namespace Rival
