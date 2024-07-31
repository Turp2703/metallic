#include "orb.h"

#include "raymath.h"

Orb::Orb(Vector2 p_position, Vector2 p_target)
    : m_position(p_position), m_target(p_target)
    , m_orbit(0), m_onFront(true)
    , m_followSpeed(0.5f), m_mode(MODE_ORB)
    , m_leftMag( { p_position.x - 32.f, p_position.y + 16.f } )
    , m_rightMag( { p_position.x + 56.f, p_position.y + 16.f } )
    , m_attachedMagCore(nullptr)
{
    /* */
}

void Orb::update(const std::vector<MagCore*>& m_magCores){
    for (auto it = m_neoParticles.begin(); it != m_neoParticles.end();){
        if (!it->isAlive()){
            it = m_neoParticles.erase(it);
        }
        else {
            it->update();
            it++;
        }
    }

    m_leftMag  = { m_position.x - 32.f, m_position.y + 16.f };
    m_rightMag = { m_position.x + 64.f, m_position.y + 16.f };
    
    if(m_mode == MODE_ORB){
        m_orbit += kOrbitSpeed;
        if(m_orbit > 359) 
            m_orbit = 0; 
        if(m_orbit == 0 || m_orbit == 180) 
            m_onFront = !m_onFront;
        m_target.x += 16.f * cos(m_orbit * DEG2RAD);
        m_target.y += 4.f * sin(2 * m_orbit * DEG2RAD);
    }
    else if(m_mode == MODE_IRON){
        m_target.x -= 4.f;
        m_target.y -= 16.f;
    }
    else if(m_mode == MODE_CESIUM){
        m_target.x -= 4.f;
        m_target.y += 8.f;
    }
    else if(m_mode == MODE_NEOL){
        m_target.x -= 8.f;
        if(m_attachedMagCore == nullptr){
            for(const auto& mc : m_magCores)
                if( m_attachedMagCore == nullptr && !mc->onSlot()
                    && (CheckCollisionPointRec(m_leftMag, mc->getHitBox())
                    || CheckCollisionPointRec({m_leftMag.x+16.f, m_leftMag.y}, mc->getHitBox())))
                    m_attachedMagCore = mc;
        }
        else if (m_attachedMagCore->onSlot())
            m_attachedMagCore = nullptr;
        else
            m_attachedMagCore->attach( { m_leftMag.x, m_leftMag.y - 16.f } );
        m_neoParticles.push_back(Particle( {m_leftMag.x, m_leftMag.y+GetRandomValue(-12,12)}, 180, 0.2));
    }
    else if(m_mode == MODE_NEOR){
        if(m_attachedMagCore == nullptr){
            for(const auto& mc : m_magCores)
                if( m_attachedMagCore == nullptr && !mc->onSlot()
                    && (CheckCollisionPointRec(m_rightMag, mc->getHitBox())
                    || CheckCollisionPointRec({m_rightMag.x-16.f, m_rightMag.y}, mc->getHitBox())))
                    m_attachedMagCore = mc;
        }
        else if (m_attachedMagCore->onSlot())
            m_attachedMagCore = nullptr;
        else
            m_attachedMagCore->attach( { m_rightMag.x - 32.f, m_rightMag.y - 16.f } );
        m_neoParticles.push_back(Particle( {m_rightMag.x, m_rightMag.y+GetRandomValue(-12,12)}, 0, 0.2));
    }
    m_position = Vector2Lerp(m_position, m_target, m_followSpeed);
}
        
void Orb::draw(Texture2D& p_texture){
    DrawTextureV(p_texture, m_position, WHITE);
    
    // if(m_mode == MODE_NEOL)
        // DrawCircleV(m_leftMag, 3, RED);
    // else if(m_mode == MODE_NEOR)
        // DrawCircleV(m_rightMag, 3, RED);
}
void Orb::drawEffects(Texture2D& p_particleTexture){
    for(auto& particle : m_neoParticles)
        particle.draw(p_particleTexture, {0,200,200,100}, false);
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
    if(p_newMode == MODE_ORB){
        m_followSpeed = 0.5f;
        m_onFront = m_orbit < 180;
    }
    else {
        m_followSpeed = 0.9f;
        m_onFront = true;
    }
    if(m_attachedMagCore != nullptr && p_newMode != MODE_NEOL && p_newMode != MODE_NEOR){
        m_attachedMagCore->detach();
        m_attachedMagCore = nullptr;
    }
}

void Orb::restart(){
    m_attachedMagCore = nullptr;
    m_position = m_target;
}