#pragma once

#include "tile.h"
#include <unordered_map>

class TileMap {
    public:
        TileMap();
        ~TileMap();

        Tile* getTile(int, int);
        bool isWithinBounds(int, int) const;
        int getWidth() const;
        int getHeight() const;
        int getTileSize() const;
        void setTile(Tile*);
        void draw();
        
        // LOAD MAP ?

    private:
        static const int kMapWidth = 32;
        static const int kMapHeight = 18;
        static const int kTileSize = 32;
        Tile* m_map[kMapWidth][kMapHeight];
        std::unordered_map<TileType,Texture2D> m_textures;
};