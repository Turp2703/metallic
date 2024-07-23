#include "lamp.h"

Lamp::Lamp(Vector2 p_position, TileType p_tileType, bool p_isSolid, int p_height)
    : Tile(p_position, p_tileType, p_isSolid), m_height(p_height)
    , m_center( { p_position.x * kTileSize, (p_position.y + 1) * kTileSize, kTileSize, p_height * kTileSize } )
    , m_left1( { p_position.x * kTileSize, (p_position.y + 1) * kTileSize } )
    , m_left2( { p_position.x * kTileSize, (p_position.y + 1 + p_height) * kTileSize } )
    , m_left3( { (p_position.x - 1) * kTileSize, (p_position.y + 1 + p_height) * kTileSize } )
    , m_right1( { (p_position.x + 1) * kTileSize, (p_position.y + 1) * kTileSize } )
    , m_right2( { (p_position.x + 1) * kTileSize, (p_position.y + 1 + p_height) * kTileSize } )
    , m_right3( { (p_position.x + 2) * kTileSize, (p_position.y + 1 + p_height) * kTileSize } )
{
    /* */
}

Lamp::~Lamp(){
    /* */
}

void Lamp::draw(const Texture2D& p_texture){
    Tile::draw(p_texture);
    
    // Draw responsive light ?
    
    Color lightColor = {253,249,0,128};
    DrawTriangle(m_left3, m_left2, m_left1, lightColor); // Vertices go counter-clockwise
    DrawRectangleRec(m_center, lightColor);
    DrawTriangle(m_right1, m_right2, m_right3, lightColor);
}

bool Lamp::pointInsideLight(Vector2 p_point){
    Vector2 points[] = {m_left1, m_left3, m_left2, m_right2, m_right3, m_right1};
    return CheckCollisionPointPoly(p_point, points, 6);
}