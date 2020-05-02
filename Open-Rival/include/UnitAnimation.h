#ifndef UNIT_ANIMATION_H
#define UNIT_ANIMATION_H

#include "UnitAnimationLookup.h"

namespace Rival {

    class UnitAnimation {

    public:
        UnitAnimation(UnitType unitType);

        void rotateLeft();

        void rotateRight();

        int getCurrentSpriteIndex() const;

        void setAnimation(UnitAnimationType unitAnimationType);

        void setFacing(Facing newFacing);

        void tick();

    private:

        void updateSpriteSheetEntry();

        int animationStep;

        Facing facing;

        SpritesheetEntry spritesheetEntry;

        // Holds unitType and unitAnimationType
        UnitAnimationPair unitAnimationPair;

    };
}

#endif // UNIT_ANIMATION_H
