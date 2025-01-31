#pragma once

#include <memory>
#include <string>

#include "Building.h"
#include "EntityComponent.h"
#include "SpriteComponent.h"

namespace Rival {

// We have 16 wall variants.
// The underlying int value corresponds to the wall's texture index.
enum class WallVariant : std::uint8_t
{
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P
};

/**
 * Component containing data required by wall segments.
 */
class WallComponent : public EntityComponent
{

public:
    WallComponent(WallVariant variant);

    void onEntitySpawned(World* scenario) override;

    WallVariant getVariant() const;

private:
    int getBaseTxIndex(Building::Type buildingType) const;

public:
    static const std::string key;

private:
    static constexpr int baseTxIndexElf = 48;
    static constexpr int baseTxIndexGreenskin = 33;
    static constexpr int baseTxIndexHuman = 42;
    static constexpr int baseTxIndexPalisade = 32;
    static constexpr int baseTxIndexGrate = 48;

    WallVariant variant;
};

}  // namespace Rival
