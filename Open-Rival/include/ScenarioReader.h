#ifndef SCENARIO_READER_H
#define SCENARIO_READER_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "Scenario.h"
#include "Unit.h"

namespace Rival {

    const int numPlayers = 8;
    const int numTroops = 42;
    const int numTroopsPerRace = 14;
    const int numMonsters = 30;
    const int numBuildings = 36;
    const int numBuildingsPerRace = 12;
    const int numBuildingsPlusCropland = numBuildings + 1;
    const int numProductionCosts = numBuildingsPlusCropland + numTroops;
    const int numWeapons = 96;
    const int numUpgrades = 112;
    const int numAiStrategies = 7;

    struct ScenarioHeader {
        // 8 bytes: unknown
        bool wilderness;
        std::string mapName;
        std::uint32_t mapHeight;
        std::uint32_t mapWidth;
        // 9 bytes: unknown
    };

    struct PlayerProperties {
        bool hasStartLocation;
        std::uint32_t startLocX;
        std::uint32_t startLocY;
        std::uint32_t startingFood;
        std::uint32_t startingWood;
        std::uint32_t startingGold;
        std::uint8_t race;
        bool ai;
        std::uint8_t aiType;
        std::uint8_t aiPerformance;
        std::uint8_t aiStrategy;
    };

    struct TroopDefaults {  // also used for Monster Defaults
        std::uint16_t hitpoints;
        std::uint16_t magic;
        std::uint8_t armour;
        // 2 bytes: unknown
        std::uint8_t sight;
        std::uint8_t range;
        // 39 bytes: unknown
    };

    struct UpgradeProperties {
        std::uint32_t amount;
        std::uint32_t goldCost;
        std::uint32_t woodCost;
        std::uint32_t unknown;
    };

    struct ProductionCost {
        std::uint16_t goldCost;
        std::uint16_t woodCost;
        std::uint32_t constructionTime;
        std::uint32_t requiredExpOrIncreasePercent;
    };

    struct WeaponDefaults {
        std::uint16_t moveSpaces;
        std::uint16_t moveTime;
        std::uint16_t damage;
        std::uint16_t penetrate;
        std::uint16_t accuracy;
        std::uint8_t effectRange;
        std::uint8_t attackRange;
        std::uint16_t manaCost;
        std::uint32_t reloadTime;
        std::uint16_t unknown;
        // 1 byte: empty
    };

    struct AvailableBuildings {
        bool cropLand;
        bool goldAmplifier;
        bool rangedTroopBuilding;
        bool siegeTroopBuilding;
        bool meleeTroopBuilding;
        bool flyingTroopBuilding;
        bool engineerTroopBuilding;
        bool healerTroopBuilding;
        bool spellcasterTroopBuilding;
        bool shipyard;
        bool watchTower;
        bool wall;
    };

    struct HireTroopsRestrictions {
        bool worker;
        bool rangedTroop;
        bool lightMeleeOrSpellcasterTroop;
        bool heavyMeleeTroop;
        bool engineer;
        bool stealthTroop;
        bool siegeTroop;
        bool raceBonusTroop;
        bool spellcaster;
        bool healer;
        bool transportShip;
        bool combatShip;
        bool flyingTroop;
        bool flyingTransport;
        bool mustHire;
    };

    struct AiSetting {
        std::uint8_t amount;
        std::uint8_t flag;
    };

    struct AiStrategy {
        std::array<AiSetting, numBuildingsPerRace> aiBuildingSettings;
        std::array<AiSetting, numTroopsPerRace> aiTroopSettings;
    };

    struct TilePlacement {
        TileType type;
    };

    struct BuildingPlacement {
        std::uint8_t type;
        std::uint8_t player;
        // 1 byte: unknown (0x00 - 0x0f for Palisade)
        // 1 byte: empty
        // 1 byte: unknown (0x01 for Green Door, 0x02 for Blue Door, 0x03 for Yellow Door, 0x04 for Buildings)
        std::uint16_t x;
        std::uint16_t y;
        // 1 byte: unknown (0x08 for Palisade / Grate / Door)
        std::uint16_t hitpoints;
        std::uint16_t armour;
        // 1 byte: empty
        std::uint8_t sight;
        std::uint8_t range;
        bool upgrade1Enabled;
        bool upgrade2Enabled;
        std::uint8_t specialColour;
        bool prisoner;
        std::string name;
        // 1 byte: empty
    };

    struct UnitPlacement {
        std::uint8_t type;
        // 2 bytes: empty
        std::uint8_t facing;
        // 1 byte: empty
        std::uint16_t x;
        std::uint16_t y;
        std::uint8_t player;
        std::uint16_t hitpoints;
        std::uint8_t magic;
        std::uint16_t armour;
        // 1 bytes: empty
        std::uint8_t type2;
        std::uint8_t sight;
        std::uint8_t range;
        // 2 bytes: empty
        std::uint8_t specialColour;
        bool prisoner;
        std::uint16_t goldCost;
        std::uint16_t woodCost;
        std::string name;
        // 13 bytes: empty
        bool upgrade1Enabled;
        bool upgrade2Enabled;
        bool upgrade3Enabled;
        bool upgrade4Enabled;
        std::uint8_t fightingArea;
    };

    struct ChestPlacement {
        std::uint32_t type;
        std::uint32_t variant;
        std::uint8_t x;
        std::uint8_t y;
    };

    struct InfoPointPlacement {
        std::uint8_t x;
        std::uint8_t y;
        std::string message;
    };

    struct TrapPlacement {
        std::uint8_t x;
        std::uint8_t y;
        std::uint8_t player;
    };

    struct ObjectPlacement {
        std::uint8_t type;
        // 1 byte: empty
        std::uint8_t variant;
        std::uint32_t x;
        std::uint32_t y;
    };

    struct GoalLocation {
        std::uint8_t type;
        std::uint8_t x;
        // 3 bytes: empty
        std::uint8_t y;
        // 3 bytes: empty
    };

    struct Goal {
        std::uint8_t type;
        std::uint8_t x;
        std::uint8_t y;
        std::uint8_t count;
        std::uint8_t player;
        std::uint8_t radius;
        std::uint8_t troopId;
        std::uint8_t troopOrBuildingType;
        std::uint8_t itemType;
    };

    struct CampaignText {
        std::string title;
        std::string objectives;
        std::string narration;
    };

    struct ScenarioFile {
        ScenarioHeader hdr;
        std::array<PlayerProperties, numPlayers> playerProperties;
        std::array<TroopDefaults, numTroops> troopDefaults;
        std::array<TroopDefaults, numMonsters> monsterDefaults;
        std::array<UpgradeProperties, numUpgrades> upgradeProperties;
        std::array<ProductionCost, numProductionCosts> productionCosts;
        std::array<WeaponDefaults, numWeapons> weaponDefaults;
        AvailableBuildings availableBuildings;
        HireTroopsRestrictions hireTroopsRestrictions;
        std::array<AiStrategy, numAiStrategies> aiStrategies;
        std::vector<TilePlacement> tiles;
        std::vector<ObjectPlacement> objects;
        std::vector<BuildingPlacement> buildings;
        std::vector<UnitPlacement> units;
        std::vector<ChestPlacement> chests;
        std::vector<InfoPointPlacement> infoPoints;
        std::vector<TrapPlacement> traps;
        std::vector<GoalLocation> goalLocations;
        std::vector<Goal> goals;
        CampaignText campaignText;
        std::uint8_t checksum;

        ScenarioFile() {
            // Initialise AI strategies
            for (int i = 0; i < numAiStrategies; i++) {
                aiStrategies[i] = AiStrategy();
            }

        }
    };

    class ScenarioReader {

    public:

        ScenarioReader(const std::string filename);

    private:

        static const std::map<std::uint8_t, char> alphabet;

        static const int numBytesShort = 2;
        static const int numBytesInt = 4;

        static const int bytesPerTile = 6;

        size_t pos = 0;

        void readFile(const std::string filename);

        ///////////////////////////////////////////////////////////////////////
        // Parsing
        ///////////////////////////////////////////////////////////////////////

        ScenarioHeader parseHeader(std::vector<unsigned char>& data);

        PlayerProperties parsePlayerProperties(
                std::vector<unsigned char>& data);

        TroopDefaults parseTroopDefaults(std::vector<unsigned char>& data);

        bool ScenarioReader::doesUpgradeHaveAmount(int i) const;

        UpgradeProperties parseUpgradeProperties(
                std::vector<unsigned char>& data, bool readAmount);

        ProductionCost parseProductionCost(
                std::vector<unsigned char>& data);

        WeaponDefaults parseWeaponDefaults(std::vector<unsigned char>& data);

        AvailableBuildings parseAvailableBuildings(
                std::vector<unsigned char>& data);

        HireTroopsRestrictions parseHireTroopsRestrictions(
                std::vector<unsigned char>& data);

        AiSetting parseAiSetting(std::vector<unsigned char>& data);

        std::vector<TilePlacement> ScenarioReader::parseTerrain(
                std::vector<unsigned char>& data,
                int width,
                int height);

        TilePlacement ScenarioReader::parseTile(
                std::vector<unsigned char>& data);

        std::vector<ObjectPlacement> parseObjects(
                std::vector<unsigned char>& data);

        ObjectPlacement parseObject(std::vector<unsigned char>& data);

        std::vector<BuildingPlacement> parseBuildings(
                std::vector<unsigned char>& data);

        BuildingPlacement parseBuilding(std::vector<unsigned char>& data);

        std::vector<UnitPlacement> parseUnits(
                std::vector<unsigned char>& data);

        UnitPlacement parseUnit(std::vector<unsigned char>& data);

        std::vector<ChestPlacement> parseChests(
                std::vector<unsigned char>& data);

        ChestPlacement parseChest(
                std::vector<unsigned char>& data);

        std::vector<InfoPointPlacement> parseInfoPoints(
                std::vector<unsigned char>& data);

        InfoPointPlacement parseInfoPoint(
                std::vector<unsigned char>& data);

        std::vector<TrapPlacement> parseTraps(
                std::vector<unsigned char>& data);

        TrapPlacement parseTrap(std::vector<unsigned char>& data);

        std::vector<GoalLocation> parseGoalLocations(
                std::vector<unsigned char>& data);

        GoalLocation parseGoalLocation(std::vector<unsigned char>& data);

        std::vector<Goal> parseGoals(std::vector<unsigned char>& data);

        Goal parseGoal(std::vector<unsigned char>& data);

        CampaignText parseCampaignText(std::vector<unsigned char>& data);

        ///////////////////////////////////////////////////////////////////////
        // Token Extraction
        ///////////////////////////////////////////////////////////////////////

        std::uint8_t readByte(std::vector<unsigned char>& data);

        std::uint8_t readByte(
                std::vector<unsigned char>& data, size_t offset) const;

        std::uint8_t readRivalByte(std::vector<unsigned char>& data);

        std::uint8_t readRivalByte(
            std::vector<unsigned char>& data, size_t offset) const;

        bool readBool(std::vector<unsigned char>& data);

        bool readBool(
                std::vector<unsigned char>& data, size_t offset) const;

        std::uint16_t readShort(std::vector<unsigned char>& data);

        std::uint16_t readShort(
            std::vector<unsigned char>& data, size_t offset) const;

        std::uint32_t readInt(std::vector<unsigned char>& data);

        std::uint32_t readInt(
                std::vector<unsigned char>& data, size_t offset) const;

        std::string readString(
                std::vector<unsigned char>& data, size_t length);

        std::string readString(
                std::vector<unsigned char>& data,
                size_t offset,
                size_t length) const;

        std::string readRivalString(
                std::vector<unsigned char>& data, size_t length);

        std::string readRivalString(
                std::vector<unsigned char>& data,
                size_t offset,
                size_t length) const;

        char getRivalChar(std::uint8_t c) const;

        void skip(std::vector<unsigned char>& data, const size_t n, bool print);

        ///////////////////////////////////////////////////////////////////////
        // Printing
        ///////////////////////////////////////////////////////////////////////

        void ScenarioReader::printOffset() const;

        void printSection(std::string title) const;

        void printNext(std::vector<unsigned char>& data, const size_t n) const;

        void print(ScenarioHeader& hdr) const;

        void print(PlayerProperties& props) const;

        void print(TroopDefaults& troop) const;

        void print(UpgradeProperties& upgrade) const;

        void print(ProductionCost& unitCost) const;

        void print(WeaponDefaults& wpn) const;

        void print(AvailableBuildings& bldg) const;

        void print(HireTroopsRestrictions& restrictions) const;

        void print(AiSetting& setting) const;

        void print(ObjectPlacement& obj) const;

        void print(BuildingPlacement& bldg) const;

        void print(UnitPlacement& unit) const;

        void print(TrapPlacement& trap) const;

        void print(GoalLocation& trap) const;

        void print(CampaignText& text) const;

    };

}

#endif // SCENARIO_READER_H