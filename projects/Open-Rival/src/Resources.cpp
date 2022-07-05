#include "pch.h"

#include "Resources.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include "ConfigUtils.h"
#include "FileUtils.h"
#include "MidsDecoder.h"
#include "PathUtils.h"
#include "RenderUtils.h"

namespace Rival {

const std::string Resources::dataDir = "res\\data\\";
const std::string Resources::mapsDir = "res\\maps\\";
const std::string Resources::soundDir = "res\\sound\\";
const std::string Resources::txDir = "res\\textures\\";

const std::vector<std::string> Resources::defaultFontDirs = PathUtils::getDefaultFontDirs();
const std::string Resources::defaultFontSmall = "serife.fon";
const std::string Resources::defaultFontRegular = "Procopius Regular.ttf";

Resources::Resources(json& cfg)
    : cfg(cfg)
    , freeTypeLib(initFreeType())
    , fontSmall(initFontSmall())
    , fontRegular(initFontRegular())
    , textures(loadTextures())
    , textureAtlases(loadTextureAtlases())
    , paletteTexture(initPaletteTexture())
    , unitSpritesheets(initUnitSpritesheets())
    , buildingSpritesheets(initBuildingSpritesheets())
    , tileSpritesheets(initTileSpritesheets())
    , objectSpritesheets(initObjectSpritesheets())
    , mapBorderSpritesheet(initMapBorderSpritesheet())
    , sounds(initSounds())
    , midis(initMidis())
    , unitDefs(initUnitDefs())
{
}

Resources::~Resources()
{
    // Clean up FreeType library
    FT_Done_FreeType(freeTypeLib);

    // Delete game Textures
    for (const Texture& texture : textures)
    {
        const GLuint texId = texture.getId();
        glDeleteTextures(1, &texId);
    }

    // Delete palette Texture
    {
        const GLuint texId = paletteTexture.getId();
        glDeleteTextures(1, &texId);
    }

    // Delete Font Textures
    {
        const GLuint texId = fontSmall.getTexture().getId();
        glDeleteTextures(1, &texId);
    }
    {
        const GLuint texId = fontRegular.getTexture().getId();
        glDeleteTextures(1, &texId);
    }
}

FT_Library Resources::initFreeType()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        throw std::runtime_error("Failed to initialize FreeType library");
    }
    return ft;
}

Font Resources::initFontSmall()
{
    std::vector<std::string> fontDirs = ConfigUtils::get(cfg, "fontDirs", defaultFontDirs);
    std::string fontName = ConfigUtils::get(cfg, "fontSmall", defaultFontSmall);
    int fontSize = ConfigUtils::get(cfg, "fontSmallSize", 32);
    return Font::loadFont(freeTypeLib, fontDirs, fontName, fontSize);
}

Font Resources::initFontRegular()
{
    std::vector<std::string> fontDirs = ConfigUtils::get(cfg, "fontDirs", defaultFontDirs);
    std::string fontName = ConfigUtils::get(cfg, "fontRegular", defaultFontRegular);
    int fontSize = ConfigUtils::get(cfg, "fontRegularSize", 16);
    return Font::loadFont(freeTypeLib, fontDirs, fontName, fontSize);
}

std::vector<Texture> Resources::loadTextures()
{
    std::vector<Texture> texList;
    texList.reserve(numTextures);

    std::vector<std::string> textureNames = { // Units - Human
                                              "unit_human_ballista.tga",
                                              "unit_human_battleship.tga",
                                              "unit_human_bowman.tga",
                                              "unit_human_chariot_of_war.tga",
                                              "unit_human_fire_master.tga",
                                              "unit_human_knight.tga",
                                              "unit_human_light_cavalry.tga",
                                              "unit_human_peasant.tga",
                                              "unit_human_pegas_rider.tga",
                                              "unit_human_priest.tga",
                                              "unit_human_sea_barge.tga",
                                              "unit_human_thief.tga",
                                              "unit_human_wizard.tga",
                                              "unit_human_zeppelin.tga",

                                              // Units - Greenskin
                                              "unit_greenskin_balloon.tga",
                                              "unit_greenskin_catapult.tga",
                                              "unit_greenskin_gnome_boomer.tga",
                                              "unit_greenskin_horde_rider.tga",
                                              "unit_greenskin_landing_craft.tga",
                                              "unit_greenskin_necromancer.tga",
                                              "unit_greenskin_priest_of_doom.tga",
                                              "unit_greenskin_rock_thrower.tga",
                                              "unit_greenskin_rogue.tga",
                                              "unit_greenskin_serf.tga",
                                              "unit_greenskin_storm_trooper.tga",
                                              "unit_greenskin_troll_galley.tga",
                                              "unit_greenskin_warbat.tga",
                                              "unit_greenskin_warlord.tga",

                                              // Units - Elf
                                              "unit_elf_archer.tga",
                                              "unit_elf_arquebusier.tga",
                                              "unit_elf_bark.tga",
                                              "unit_elf_bombard.tga",
                                              "unit_elf_centaur.tga",
                                              "unit_elf_druid.tga",
                                              "unit_elf_dwarf_miner.tga",
                                              "unit_elf_enchanter.tga",
                                              "unit_elf_mage.tga",
                                              "unit_elf_magic_chopper.tga",
                                              "unit_elf_scout.tga",
                                              "unit_elf_sky_rider.tga",
                                              "unit_elf_warship.tga",
                                              "unit_elf_yeoman.tga",

                                              // Units - Monsters
                                              "unit_monster_devil.tga",
                                              "unit_monster_dragon.tga",
                                              "unit_monster_golem.tga",
                                              "unit_monster_gryphon.tga",
                                              "unit_monster_hydra.tga",
                                              "unit_monster_sea_monster.tga",
                                              "unit_monster_skeleton.tga",
                                              "unit_monster_snake.tga",

                                              // Tiles
                                              "tiles_meadow.tga",
                                              "tiles_wilderness.tga",
                                              "tiles_fog.tga",
                                              "tiles_map_border.tga",

                                              // Objects
                                              "objects_common.tga",
                                              "objects_meadow.tga",
                                              "objects_wilderness.tga",

                                              // Cursors
                                              "cursor_select.tga",

                                              // Buildings
                                              "buildings_elf.tga",
                                              "buildings_greenskin.tga",
                                              "buildings_human.tga"
    };

    for (auto const& textureName : textureNames)
    {
        texList.push_back(Texture::loadTexture(txDir + textureName));
    }

    return texList;
}

std::vector<TextureAtlas> Resources::loadTextureAtlases()
{
    std::vector<TextureAtlas> texAtlasList;
    texAtlasList.reserve(numTextureAtlases);

    std::vector<std::string> resourceNames = { "game_interface" };

    for (auto const& resourceName : resourceNames)
    {
        texAtlasList.push_back(TextureAtlas::loadTextureAtlas(Resources::txDir + resourceName));
    }

    return texAtlasList;
}

Texture Resources::initPaletteTexture()
{
    return PaletteUtils::createPaletteTexture();
}

std::unordered_map<Building::Type, Spritesheet> Resources::initBuildingSpritesheets()
{
    std::unordered_map<Building::Type, Spritesheet> spritesheets;
    int nextIndex = txIndexBuildings;

    auto createSpritesheets = [&](int first, int last) {
        for (auto it = first; it <= last; ++it)
        {
            spritesheets.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(static_cast<Building::Type>(it)),
                    std::forward_as_tuple(
                            textures.at(nextIndex), RenderUtils::entityWidthPx, RenderUtils::entityHeightPx));
        }
    };

    createSpritesheets(Building::firstElfBuildingType, Building::lastElfBuildingType);
    ++nextIndex;
    createSpritesheets(Building::firstGreenskinBuildingType, Building::lastGreenskinBuildingType);
    ++nextIndex;
    createSpritesheets(Building::firstHumanBuildingType, Building::lastHumanBuildingType);

    return spritesheets;
}

std::unordered_map<Unit::Type, Spritesheet> Resources::initUnitSpritesheets()
{

    std::unordered_map<Unit::Type, Spritesheet> spritesheets;
    int nextIndex = txIndexUnits;

    for (auto it = Unit::firstUnitType; it <= Unit::lastUnitType; ++it)
    {
        spritesheets.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(static_cast<Unit::Type>(it)),
                std::forward_as_tuple(textures.at(nextIndex), RenderUtils::entityWidthPx, RenderUtils::entityHeightPx));
        nextIndex++;
    }

    return spritesheets;
}

std::vector<Spritesheet> Resources::initTileSpritesheets()
{
    std::vector<Spritesheet> spritesheets;

    // 0 = Meadow
    // 1 = Wilderness
    // 2 = Fog
    for (int i = txIndexTiles; i < txIndexTiles + 3; i++)
    {
        spritesheets.emplace_back(textures.at(i), RenderUtils::tileSpriteWidthPx, RenderUtils::tileSpriteHeightPx);
    }

    return spritesheets;
}

std::vector<Spritesheet> Resources::initObjectSpritesheets()
{
    std::vector<Spritesheet> spritesheets;

    // 0 = Common
    // 1 = Meadow
    // 2 = Wilderness
    for (int i = txIndexObjects; i < txIndexObjects + 3; i++)
    {
        spritesheets.emplace_back(textures.at(i), RenderUtils::entityWidthPx, RenderUtils::entityHeightPx);
    }

    return spritesheets;
}

Spritesheet Resources::initMapBorderSpritesheet()
{
    return Spritesheet(textures.at(txIndexTiles + 3), RenderUtils::tileSpriteWidthPx, RenderUtils::tileSpriteHeightPx);
}

std::vector<WaveFile> Resources::initSounds()
{
    std::vector<WaveFile> soundsRead;
    soundsRead.reserve(numSounds);

    for (int i = 0; i < numSounds; ++i)
    {
        std::string fileNum = std::to_string(i);
        std::string filename = std::string(3 - fileNum.length(), '0') + fileNum + ".wav";
        soundsRead.emplace_back(Resources::soundDir + filename);
    }

    return soundsRead;
}

std::vector<MidiFile> Resources::initMidis()
{
    std::vector<MidiFile> midisRead;

    for (int i = 0; i < numMidis; ++i)
    {
        midi_container result;

        // Read MIDI file
        std::vector<std::uint8_t> const& p_file =
                FileUtils::readBinaryFile(soundDir + std::to_string(midiStartIndex + i) + ".mid");

        // Parse MIDI file
        if (!MidsDecoder::process_mids(p_file, result))
        {
            throw std::runtime_error("Failed to parse MIDI file!");
        }

        midisRead.emplace_back(result);
    }

    return midisRead;
}

std::unordered_map<Unit::Type, UnitDef> Resources::initUnitDefs() const
{
    json rawData = FileUtils::readJsonFile(Resources::dataDir + "units.json");
    json unitList = rawData["units"];

    std::unordered_map<Unit::Type, UnitDef> allUnitDefs;
    int nextUnitType = 0;

    for (const auto& rawUnitDef : unitList)
    {
        if (nextUnitType < 0 || nextUnitType > Unit::lastUnitType)
        {
            throw std::runtime_error("Trying to parse invalid unit type: " + std::to_string(nextUnitType));
        }

        Unit::Type unitType = static_cast<Unit::Type>(nextUnitType);

        try
        {
            allUnitDefs.insert({ unitType, UnitDef::fromJson(rawUnitDef) });
        }
        catch (const json::exception&)
        {
            std::cout << "Error parsing unit definition: " << std::to_string(nextUnitType) << "\n";
            throw;
        }

        ++nextUnitType;
    }

    return allUnitDefs;
}

const Font& Resources::getFontSmall() const
{
    return fontSmall;
}

const Font& Resources::getFontRegular() const
{
    return fontRegular;
}

const Spritesheet& Resources::getTileSpritesheet(bool wilderness) const
{
    return wilderness ? tileSpritesheets.at(1) : tileSpritesheets.at(0);
}

const Spritesheet& Resources::getUnitSpritesheet(Unit::Type unitType) const
{
    return unitSpritesheets.at(unitType);
}

const Spritesheet& Resources::getBuildingSpritesheet(Building::Type buildingType) const
{
    return buildingSpritesheets.at(buildingType);
}

const Spritesheet& Resources::getCommonObjectSpritesheet() const
{
    return objectSpritesheets.at(0);
}

const Spritesheet& Resources::getObjectSpritesheet(bool wilderness) const
{
    return wilderness ? objectSpritesheets.at(2) : objectSpritesheets.at(1);
}

const Spritesheet& Resources::getMapBorderSpritesheet() const
{
    return mapBorderSpritesheet;
}

const Texture& Resources::getPalette() const
{
    return paletteTexture;
}

const TextureAtlas& Resources::getUiTextureAtlas() const
{
    return textureAtlases.at(0);
}

const WaveFile& Resources::getSound(int id) const
{
    return sounds.at(id);
}

const MidiFile& Resources::getMidi(int id) const
{
    return midis.at(id);
}

const UnitDef* Resources::getUnitDef(Unit::Type unitType) const
{
    auto iter = unitDefs.find(unitType);
    return iter == unitDefs.cend() ? nullptr : &iter->second;
}

}  // namespace Rival