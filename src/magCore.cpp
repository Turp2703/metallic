#include "magCore.h"

#include "raymath.h"
#include "tileMap.h"

// DEBUG
// #include <string>
// #include <iostream>

MagCore::MagCore(Vector2 p_position)
    : m_position(p_position), m_levitation(0), m_attached(false)
    , m_target(p_position), m_followSpeed(0.5f), m_onSlot(false)
{
    /* */
}

void MagCore::update(TileMap& p_tileMap){
    if(!m_onSlot){
        Vector2 center = Vector2AddValue(m_position, 16.f);
        Tile* currentTile = p_tileMap.getTileWorldPos(center.x, center.y);
        if(currentTile != nullptr && currentTile->getType() == TILE_MAGSLOT){
            m_onSlot = true;
            m_attached = false;
            m_target = currentTile->getPositionWorld();
            m_followSpeed = 0.1f;
            p_tileMap.activateMagSlot(currentTile);
        }
    }
    
    if(!m_onSlot && !m_attached){ 
        m_levitation += kLevitSpeed;
        if(m_levitation > 359) 
            m_levitation = 0; 
        m_target.y += cos(m_levitation * DEG2RAD);
    }
    m_position = Vector2Lerp(m_position, m_target, m_followSpeed);
}

void MagCore::draw(Texture2D& p_texture){
    DrawTextureV(p_texture, m_position, WHITE);
    // DrawRectangleV(m_position, {32.f, 32.f}, RED);
    
    // DrawText(std::to_string(m_onSlot).c_str(), 50, 110, 20, WHITE);
}

void MagCore::attach(Vector2 p_target){
    m_target = p_target;
    m_attached = true;
    m_followSpeed = 0.85f;
}

void MagCore::detach(){
    m_target = m_position;
    m_attached = false;
    m_followSpeed = 0.5f;
    
    // MOVE OUTSIDE WALLS ?
}

Rectangle MagCore::getHitBox(){
    Rectangle ret = {m_position.x, m_position.y, 32.f, 32.f};
    return ret;
}

bool MagCore::onSlot() const{
    return m_onSlot;
}