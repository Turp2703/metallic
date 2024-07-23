#include "tile.h"

#include "raymath.h"

Tile::Tile(Vector2 p_position, TileType p_tileType, bool p_isSolid) 
    : m_position(p_position), m_type(p_tileType), m_isSolid(p_isSolid)
{
    /* */
}

Tile::~Tile(){
    /* */
}

void Tile::draw(const Texture2D& p_texture) {
    if(m_type == TILE_WATER)
        DrawTextureV(p_texture, Vector2Scale(m_position, kTileSize), {255,255,255,128});
    else
        DrawTextureV(p_texture, Vector2Scale(m_position, kTileSize), WHITE);
}

Vector2 Tile::getPosition() const {
    return m_position;
}

Vector2 Tile::getPositionWorld() const {
    return Vector2Scale(m_position, kTileSize);
}

TileType Tile::getType() const {
    return m_type;
}

void Tile::setType(TileType p_newType) {
    m_type = p_newType;
}

bool Tile::isSolid() const {
    return m_isSolid;
}

void Tile::setSolid(bool p_newSolid) {
    m_isSolid = p_newSolid;
}