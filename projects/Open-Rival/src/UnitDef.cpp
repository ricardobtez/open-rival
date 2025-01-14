#include "pch.h"

#include "UnitDef.h"

#include <tuple>
#include <utility>

namespace Rival {

UnitDef UnitDef::fromJson(const json& j)
{
    // Basic properties
    std::string name = j.at("name");
    int portraitId = j.at("portrait");
    std::string movementModeStr = j.value("movement", "ground");
    MovementMode movementMode = getMovementMode(movementModeStr);

    // Animations
    const auto& rawAnims = j.at("animations");
    std::unordered_map<UnitAnimationType, const Animation> animations;
    tryReadAnimation(rawAnims, "standing", UnitAnimationType::Standing, animations);
    tryReadAnimation(rawAnims, "standingWithBag", UnitAnimationType::StandingWithBag, animations);
    tryReadAnimation(rawAnims, "moving", UnitAnimationType::Moving, animations);
    tryReadAnimation(rawAnims, "movingWithBag", UnitAnimationType::MovingWithBag, animations);
    tryReadAnimation(rawAnims, "attacking", UnitAnimationType::Attacking, animations);
    tryReadAnimation(rawAnims, "harvesting", UnitAnimationType::Harvesting, animations);
    tryReadAnimation(rawAnims, "dying", UnitAnimationType::Dying, animations);

    // Sounds
    const auto& rawSounds = j.at("sounds");
    std::unordered_map<UnitSoundType, const SoundBank> soundBanks;
    tryReadSoundBank(rawSounds, "select", UnitSoundType::Select, soundBanks);
    tryReadSoundBank(rawSounds, "train", UnitSoundType::Train, soundBanks);
    tryReadSoundBank(rawSounds, "move", UnitSoundType::Move, soundBanks);

    return { name, portraitId, movementMode, animations, soundBanks };
}

void UnitDef::tryReadAnimation(
        const json& rawAnims,
        const std::string& key,
        UnitAnimationType animType,
        std::unordered_map<UnitAnimationType, const Animation>& animations)
{
    auto iter = rawAnims.find(key);
    if (iter == rawAnims.end())
    {
        // Animation not found
        return;
    }

    int startIndex = iter->at("startIndex");
    int endIndex = iter->at("endIndex");
    int msPerFrame = JsonUtils::getOrDefault(iter, "msPerFrame", Animation::defaultMsPerFrame);
    int facingStride = JsonUtils::getOrDefault(iter, "facingStride", 0);

    animations.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(animType),
            std::forward_as_tuple(startIndex, endIndex, msPerFrame, facingStride));
}

void UnitDef::tryReadSoundBank(
        const json& rawSounds,
        const std::string& key,
        UnitSoundType soundType,
        std::unordered_map<UnitSoundType, const SoundBank>& soundBanks)
{
    auto iter = rawSounds.find(key);
    if (iter == rawSounds.end())
    {
        // SoundBank not found
        return;
    }

    soundBanks.emplace(soundType, *iter);
}

MovementMode UnitDef::getMovementMode(const std::string& s)
{
    if (s == "flying")
    {
        return MovementMode::Flying;
    }
    else if (s == "seafaring")
    {
        return MovementMode::Seafaring;
    }
    else
    {
        return MovementMode::Walking;
    }
}

UnitDef::UnitDef(
        std::string name,
        int portraitId,
        MovementMode movementMode,
        std::unordered_map<UnitAnimationType, const Animation> animations,
        std::unordered_map<UnitSoundType, const SoundBank> soundBanks)
    : name(name)
    , portraitId(portraitId)
    , movementMode(movementMode)
    , animations(animations)
    , soundBanks(soundBanks)
{
}

const Animation* UnitDef::getAnimation(UnitAnimationType animType) const
{
    auto iter = animations.find(animType);
    return iter == animations.cend() ? nullptr : &iter->second;
}

const SoundBank* UnitDef::getSoundBank(UnitSoundType soundType) const
{
    auto iter = soundBanks.find(soundType);
    return iter == soundBanks.cend() ? nullptr : &iter->second;
}

}  // namespace Rival
