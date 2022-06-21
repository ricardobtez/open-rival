#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <unordered_map>
#include <vector>

#include "Animations.h"
#include "Sounds.h"

using json = nlohmann::json;

namespace Rival {

class UnitDef
{
public:
    static UnitDef fromJson(const json& j);

    UnitDef(std::string name,
            int portraitId,
            std::unordered_map<UnitAnimationType, const Animation> animations,
            std::unordered_map<UnitSoundType, const SoundBank> soundBanks);

    const Animation* getAnimation(UnitAnimationType animType) const;

    const SoundBank* getSoundBank(UnitSoundType soundType) const;

public:
    /**
     * The default name for this unit type.
     */
    std::string name;

    /**
     * ID of this unit's portrait image.
     */
    int portraitId = 0;

private:
    /**
     * Animations by type.
     *
     * These specify the animation frames when the unit is facing south, and other facings are derived from them.
     */
    std::unordered_map<UnitAnimationType, const Animation> animations;

    /**
     * Sound banks by type.
     */
    std::unordered_map<UnitSoundType, const SoundBank> soundBanks;
};

}  // namespace Rival