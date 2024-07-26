#pragma once

#include "raylib.h"

enum TileType{
    TILE_BASE,
    TILE_TEST1,
    TILE_TEST2,
    TILE_TEST3,
    TILE_LAMP,
    TILE_LAMPBROKEN,
    TILE_WATER,
    TILE_MAGSLOT,
    TILE_MAGSLOTACTIVE
};

class Tile{
    public:
        Tile(Vector2, TileType, bool);
        virtual ~Tile();
        virtual void draw(const Texture2D&);
        Vector2 getPosition() const;
        Vector2 getPositionWorld() const;
        TileType getType() const;
        void setType(TileType);
        bool isSolid() const;
        void setSolid(bool p_newSolid);
    protected:
        const float kTileSize = 32;
        Vector2 m_position;
        TileType m_type;
        bool m_isSolid;
};