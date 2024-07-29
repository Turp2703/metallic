#pragma once

#include "tile.h"
#include "lamp.h"
#include "magCore.h"
#include <unordered_map>
#include <vector>

class TileMap {
    public:
        TileMap();
        ~TileMap();
        Tile* getTile(int, int);
        Tile* getTileWorldPos(int, int);
        bool isWithinBounds(int, int) const;
        int getWidth() const;
        int getHeight() const;
        int getTileSize() const;
        const std::vector<Lamp*>& getLamps() const;
        const std::vector<MagCore*>& getMagCores() const;
        void setTile(Tile*);
        void setLamp(Lamp*);
        void setWater(Tile*);
        void setMagCore(MagCore*);
        void setMagSlot(Tile*, Lamp*);
        void activateMagSlot(Tile*);
        void draw();
        void drawWater();
        void loadMap(int);
    private:
        void addBlockPlatform(float p_posX, float p_posY, int p_length, bool p_vertical);
        void addBlockRectangle(float p_posX, float p_posY, int p_width, int p_height, bool p_fill);
        void addWaterPool(float p_posX, float p_posY, int p_length, bool p_onFloor);
        void addExit(int p_posY);
        void addFillRectOnFloor(float p_posX, float p_posY, int p_width, int p_height);
        void clearMap();
        static const int kMapWidth = 32;
        static const int kMapHeight = 18;
        static const int kTileSize = 32;
        Tile* m_map[kMapWidth][kMapHeight];
        std::unordered_map<TileType,Texture2D> m_textures;
        Texture2D m_textureMagCore;
        std::vector<Lamp*> m_lamps;
        std::vector<std::pair<int,int>> m_waterTiles;
        std::vector<MagCore*> m_magCores;
        std::unordered_map<Tile*,Lamp*> m_magSlots;
};