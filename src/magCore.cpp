#include "magCore.h"

#include "raymath.h"

MagCore::MagCore(Vector2 p_position)
    : m_position(p_position), m_levitation(0), m_attached(false)
    , m_target(p_position), m_followSpeed(0.5f)
{
    /* */
}

void MagCore::update(){
    if(!m_attached){
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
    
    // MOVE OUTSIDE WALLS
}

Rectangle MagCore::getHitBox(){
    Rectangle ret = {m_position.x, m_position.y, 32.f, 32.f};
    return ret;
}

