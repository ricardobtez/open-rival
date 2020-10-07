#ifndef SCENARIO_H
#define SCENARIO_H

#include <map>
#include <memory>
#include <vector>

#include "Entity.h"
#include "Tile.h"

namespace Rival {

    class Scenario {

    public:
        // Creates a blank Scenario
        Scenario(int width, int height, bool wilderness);

        int getWidth() const;

        int getHeight() const;

        const std::vector<Tile>& getTiles() const;

        // Sets all tiles at once
        void tilesLoaded(std::vector<Tile> tiles);

        Tile getTile(int x, int y) const;

        bool isWilderness() const;

        void addEntity(
                std::unique_ptr<Entity> unit,
                int x,
                int y);

        const std::map<int, std::unique_ptr<Entity>>& getEntities() const;

        void setPassability(int x, int y, TilePassability passability);

    private:
        const int width;
        const int height;
        bool wilderness;
        std::vector<Tile> tiles;
        std::vector<TilePassability> tilePassability;

        int nextId;
        std::map<int, std::unique_ptr<Entity>> entities;
    };

}  // namespace Rival

#endif  // SCENARIO_H
