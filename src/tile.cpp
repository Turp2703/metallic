#include "tile.h"

#include "raymath.h"

Tile::Tile(Vector2 p_position, float p_tileSize, TileType p_tileType, bool p_isSolid) 
    : kTileSize(p_tileSize), m_position(p_position), m_type(p_tileType), m_isSolid(p_isSolid)
    , m_hitBox({p_position.x * p_tileSize, p_position.y * p_tileSize, p_tileSize, p_tileSize})
{
    /* */
}

void Tile::draw(const Texture2D& p_texture) {
    DrawTextureV(p_texture, Vector2Scale(m_position, kTileSize), WHITE);
}

Vector2 Tile::getPosition() const {
    return m_position;
}

TileType Tile::getType() const {
    return m_type;
}

bool Tile::isSolid() const {
    return m_isSolid;
}

const Rectangle& Tile::getHitBox() const{
    return m_hitBox;
}