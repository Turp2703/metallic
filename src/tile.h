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
    TILE_MAGSLOTACTIVE,
    TILE_BLOCK,
    TILE_BLOCKB,
    TILE_BLOCKBL,
    TILE_BLOCKBLH,
    TILE_BLOCKBP,
    TILE_BLOCKBR,
    TILE_BLOCKBRH,
    TILE_BLOCKF,
    TILE_BLOCKH,
    TILE_BLOCKL,
    TILE_BLOCKLP,
    TILE_BLOCKR,
    TILE_BLOCKRP,
    TILE_BLOCKT,
    TILE_BLOCKTL,
    TILE_BLOCKTLH,
    TILE_BLOCKTP,
    TILE_BLOCKTR,
    TILE_BLOCKTRH,
    TILE_BLOCKV,
    TILE_ARROW
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