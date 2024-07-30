#include "lamp.h"

#include "raymath.h"

Lamp::Lamp(Vector2 p_position, TileType p_tileType, bool p_isSolid, int p_height)
    : Tile(p_position, p_tileType, p_isSolid), m_height(p_height)
    , m_center( { p_position.x * kTileSize, (p_position.y + 1) * kTileSize, kTileSize, p_height * kTileSize } )
    , m_left1( { p_position.x * kTileSize, (p_position.y + 1) * kTileSize } )
    , m_left2( { p_position.x * kTileSize, (p_position.y + 1 + p_height) * kTileSize } )
    , m_left3( { (p_position.x - 1) * kTileSize, (p_position.y + 1 + p_height) * kTileSize } )
    , m_right1( { (p_position.x + 1) * kTileSize, (p_position.y + 1) * kTileSize } )
    , m_right2( { (p_position.x + 1) * kTileSize, (p_position.y + 1 + p_height) * kTileSize } )
    , m_right3( { (p_position.x + 2) * kTileSize, (p_position.y + 1 + p_height) * kTileSize } )
    , m_isBroken(false), m_magSlotCenter(Vector2Zero()), m_destroyedTime(0.0)
{
    /* */
}

Lamp::~Lamp(){
    /* */
}

void Lamp::draw(const Texture2D& p_texture){
    Tile::draw(p_texture);
    
    // Light
    if(!m_isBroken){
        Color lightColor = {253,249,0,128};
        DrawTriangle(m_left3, m_left2, m_left1, lightColor); // Vertices go counter-clockwise
        DrawRectangleRec(m_center, lightColor);
        DrawTriangle(m_right1, m_right2, m_right3, lightColor);
    }
}

void Lamp::drawEffects(Texture2D& p_particleTexture){
    // Particles
    for (auto it = m_destructionParticles.begin(); it != m_destructionParticles.end();){
        if (!it->isAlive()){
            it = m_destructionParticles.erase(it);
        }
        else {
            it->update();
            it++;
        }
    }
    for(auto& particle : m_destructionParticles)
        particle.draw(p_particleTexture, SKYBLUE, true);
    
    // Ray
    if(m_isBroken){
        double timeDiff = GetTime() - m_destroyedTime;
        if(timeDiff < kRayDuration){
            DrawLineEx(Vector2AddValue(Vector2Scale(m_position, kTileSize), kTileSize/2), m_magSlotCenter, kRayThickness - timeDiff * 10.f, SKYBLUE);
        }
    }
}

bool Lamp::pointInsideLight(Vector2 p_point){
    if(m_isBroken) return false;
    Vector2 points[] = {m_left1, m_left3, m_left2, m_right2, m_right3, m_right1};
    return CheckCollisionPointPoly(p_point, points, 6);
}

void Lamp::breakLamp(Vector2 p_magSlotPos){
    m_isBroken = true;
    for(int i = 0; i < 24; i++)
        m_destructionParticles.push_back(Particle( Vector2AddValue(Vector2Scale(m_position, kTileSize),kTileSize/2), GetRandomValue(-30, 210), 0.3, 5, GetRandomValue(10, 20) / 10.f));
    
    m_magSlotCenter = Vector2AddValue(p_magSlotPos, kTileSize/2);
    m_destroyedTime = GetTime();
}