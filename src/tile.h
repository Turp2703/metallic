#pragma once

#include "raylib.h"

enum TileType{
    TILE_BASE,
    TILE_TEST1,
    TILE_TEST2,
    TILE_TEST3
};

class Tile{
    public:
        Tile(Vector2, float, TileType, bool);
        void draw(const Texture2D&);
        Vector2 getPosition() const;
        TileType getType() const;
        bool isSolid() const;
        const Rectangle& getHitBox() const;
    private:
        const float kTileSize;
        Vector2 m_position;
        TileType m_type;
        bool m_isSolid;
        Rectangle m_hitBox;
};