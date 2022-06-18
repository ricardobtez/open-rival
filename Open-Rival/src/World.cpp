#include "pch.h"

#include "World.h"

namespace Rival {

// Creates an empty World
World::World(int width, int height, bool wilderness)
    : width(width)
    , height(height)
    , wilderness(wilderness)
    ,

    // Default to Grass everywhere
    tiles(std::vector<Tile>(width * height, Tile(TileType::Grass, 0, 0)))
    , tilePassability(std::vector<TilePassability>(width * height, TilePassability::Clear))
    , nextId(0)
{
}

// Creates a World from data
World::World(int width, int height, bool wilderness, std::vector<Tile> tiles)
    : width(width)
    , height(height)
    , wilderness(wilderness)
    , tiles(tiles)
    , tilePassability(std::vector<TilePassability>(width * height, TilePassability::Clear))
    , nextId(0)
{
}

Tile World::getTile(int x, int y) const
{
    return tiles[y * width + x];
}

void World::addEntity(std::shared_ptr<Entity> entity, int x, int y)
{
    // Add the Entity to the world
    entities[nextId] = entity;
    entities[nextId]->onSpawn(this, nextId, { x, y });

    // Increase the ID for the next one
    ++nextId;
}

void World::requestAddEntity(std::shared_ptr<Entity> entity, int x, int y)
{
    pendingEntities.push_back({ entity, x, y });
}

void World::addPendingEntities()
{
    for (auto const& pendingEntity : pendingEntities)
    {
        addEntity(pendingEntity.entity, pendingEntity.x, pendingEntity.y);
    }
    pendingEntities.clear();
}

void World::removeEntity(std::shared_ptr<Entity> entity)
{
    entities.erase(entity->getId());
}

const std::vector<std::shared_ptr<Entity>> World::getEntities() const
{
    std::vector<std::shared_ptr<Entity>> entityList;
    entityList.reserve(entities.size());

    for (auto const& entry : entities)
    {
        entityList.push_back(entry.second);
    }

    return entityList;
}

const std::shared_ptr<Entity> World::getEntity(int id) const
{
    auto const iter = entities.find(id);
    return iter == entities.end() ? std::shared_ptr<Entity>() : iter->second;
}

void World::setPassability(const MapNode& pos, TilePassability passability)
{
    tilePassability[pos.y * width + pos.x] = passability;
}

TilePassability World::getPassability(const MapNode& pos) const
{
    return tilePassability[pos.y * width + pos.x];
}

}  // namespace Rival
