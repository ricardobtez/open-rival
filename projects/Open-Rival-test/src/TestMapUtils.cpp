#include "pch.h"
#include "catch2/catch.h"

#include "MapUtils.h"

using namespace Rival;

TEST_CASE("getDir should return the correct direction from an even-column node", "[map-utils]")
{

    // We are in the top part of the zigzag;
    // => Moving to the row above is diagonally north.
    // => Moving to the *same* row is diagonally south.
    const MapNode startNode = { 10, 10 };

    SECTION("destination is directly North")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x, startNode.y - 1 }) == Facing::North);
    }

    SECTION("destination is directly South")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x, startNode.y + 1 }) == Facing::South);
    }

    SECTION("destination is directly East")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x + 2, startNode.y }) == Facing::East);
    }

    SECTION("destination is directly West")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x - 2, startNode.y }) == Facing::West);
    }

    SECTION("destination is directly North-East")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x + 1, startNode.y - 1 }) == Facing::NorthEast);
    }

    SECTION("destination is directly North-West")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x - 1, startNode.y - 1 }) == Facing::NorthWest);
    }

    SECTION("destination is directly South-East")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x + 1, startNode.y }) == Facing::SouthEast);
    }

    SECTION("destination is directly South-West")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x - 1, startNode.y }) == Facing::SouthWest);
    }
}

TEST_CASE("getDir should return the correct direction from an odd-column node", "[map-utils]")
{

    // We are in the bottom part of the zigzag;
    // => Moving to the *same* row is diagonally north.
    // => Moving to the row below is diagonally south.
    const MapNode startNode = { 11, 10 };

    SECTION("destination is directly North")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x, startNode.y - 1 }) == Facing::North);
    }

    SECTION("destination is directly South")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x, startNode.y + 1 }) == Facing::South);
    }

    SECTION("destination is directly East")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x + 2, startNode.y }) == Facing::East);
    }

    SECTION("destination is directly West")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x - 2, startNode.y }) == Facing::West);
    }

    SECTION("destination is directly North-East")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x + 1, startNode.y }) == Facing::NorthEast);
    }

    SECTION("destination is directly North-West")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x - 1, startNode.y }) == Facing::NorthWest);
    }

    SECTION("destination is directly South-East")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x + 1, startNode.y + 1 }) == Facing::SouthEast);
    }

    SECTION("destination is directly South-West")
    {
        REQUIRE(MapUtils::getDir(startNode, { startNode.x - 1, startNode.y + 1 }) == Facing::SouthWest);
    }
}
