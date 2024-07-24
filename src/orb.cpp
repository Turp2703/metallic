#include "orb.h"

#include "raymath.h"

Orb::Orb(Vector2 p_position, Vector2 p_target)
    : m_position(p_position), m_target(p_target)
    , m_orbit(0), m_onFront(true), m_mode(MODE_ORB)
{
    /* */
}

void Orb::update(){
    m_orbit += kOrbitSpeed;
    if(m_orbit > 359) 
        m_orbit = 0; 
    if(m_orbit == 0 || m_orbit == 180) 
        m_onFront = !m_onFront;
    m_target.x += 16.f * cos(m_orbit * DEG2RAD);
    m_target.y += 4.f * sin(2 * m_orbit * DEG2RAD);
    m_position = Vector2Lerp(m_position, m_target, 0.5f);
}
        
void Orb::draw(Texture2D& p_texture){
    DrawTextureV(p_texture, m_position, WHITE);
}

bool Orb::onFront() const{
    return m_onFront;
}

void Orb::setTarget(Vector2 p_newTarget){
    m_target = p_newTarget;
}

OrbMode Orb::getMode() const{
    return m_mode;
}

void Orb::setMode(OrbMode p_newMode){
    m_mode = p_newMode;
}