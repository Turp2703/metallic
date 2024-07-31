#include "player.h"

#include "raylib.h"
#include "raymath.h"

// Debug
// #include <iostream>

Player::Player(int p_screenHeight, int p_screenWidth)
    : kScreenHeight(p_screenHeight), kScreenWidth(p_screenWidth)
    , m_position({ 36.f, kScreenHeight - 32.f*4 })
    , m_speed(Vector2Zero()), m_acceleration(Vector2Zero())
    , m_canJump(false), m_lastFloorTime(0.0), m_lastJumpTime(0.0)
    , m_cornerTL(m_position)
    , m_cornerTR({m_position.x+kPlayerWidth,m_position.y})
    , m_cornerBL({m_position.x,m_position.y+kPlayerHeight})
    , m_cornerBR({m_position.x+kPlayerWidth,m_position.y+kPlayerHeight})
    , m_orb( {p_screenWidth / 2.f, p_screenHeight / 2.f}, m_position)
    , m_onLight(false), m_onWater(false), m_cesiumPush(false)
    , m_chargeIron(kIronMax), m_rechargeIron(false)
    , m_lastTimeIron(GetTime()), m_isAlive(true)
    , m_drawSourceWidth(24.f)
{
    m_orbTextures[MODE_ORB] = LoadTexture("assets/orb.png");
    m_orbTextures[MODE_IRON] = LoadTexture("assets/iron.png");
    m_orbTextures[MODE_CESIUM] = LoadTexture("assets/cesium.png");
    m_orbTextures[MODE_NEOL] = LoadTexture("assets/neodymiumL.png");
    m_orbTextures[MODE_NEOR] = LoadTexture("assets/neodymiumR.png");
    
    m_soundDeath = LoadSound("assets/death.wav");
    m_soundJump = LoadSound("assets/jump.wav");
    m_soundCesiumPush = LoadSound("assets/cesiumPush.wav");
    m_soundIronCharged = LoadSound("assets/ironCharged.wav");
}

void Player::update(TileMap& p_tileMap){
    // FIX WINDOW MOVING
    if(m_position.y < 5 || m_position.y > GetScreenHeight() + 5)
        restart();
    
    ///// PARTICLES /////
    for (auto it = m_jumpParticles.begin(); it != m_jumpParticles.end();){
        if (!it->isAlive()){
            it = m_jumpParticles.erase(it);
        }
        else {
            it->update();
            it++;
        }
    }
    for (auto it = m_cesiumParticles.begin(); it != m_cesiumParticles.end();){
        if (!it->isAlive()){
            it = m_cesiumParticles.erase(it);
        }
        else {
            it->update();
            it++;
        }
    }
    for (auto it = m_deathParticles.begin(); it != m_deathParticles.end();){
        if (!it->isAlive()){
            it = m_deathParticles.erase(it);
        }
        else {
            it->update();
            it++;
        }
    }
    
    ///// MOVEMENT /////
    Vector2 newPos;
    Tile* newTile;
    float delta = GetFrameTime();
    
    //// Horizontal Movement ////
    bool aDown = IsKeyDown(KEY_A);
    bool dDown = IsKeyDown(KEY_D);
    // Horizontal Acceleration
    if     ( dDown && !aDown)
        m_acceleration.x = kHorAccel;
    else if(!dDown &&  aDown) 
        m_acceleration.x = -kHorAccel;
    else
        m_acceleration.x = 0.0f;
    // Horizontal Speed
    if(m_acceleration.x != 0){
        if( m_speed.x != 0 && std::signbit(m_speed.x) != std::signbit(m_acceleration.x)) 
            m_acceleration.x *= 4; // Turning
        float newSpeed = m_speed.x + m_acceleration.x;
        if(newSpeed < kHorSpeedMax && newSpeed > -kHorSpeedMax)
            m_speed.x = newSpeed;
    }
    else{
        // Friction
        if     (m_speed.x > 0)
            m_speed.x -= m_canJump ? kHorFrict : kHorFrict / 3.f;
        else if(m_speed.x < 0)
            m_speed.x += m_canJump ? kHorFrict : kHorFrict / 3.f;
        // float correction
        if(m_speed.x > -0.21f && m_speed.x < 0.21f) 
            m_speed.x = 0.0f;
    }
    // Water slow
    m_onWater = false;
    Tile* bottomTile = p_tileMap.getTileWorldPos(m_cornerBL.x, m_cornerBL.y - 2.f);
    if( bottomTile != nullptr && bottomTile->getType() == TILE_WATER ) {
        m_speed = Vector2Scale(m_speed, 0.9f);
        m_onWater = true;
    }
    else{
        bottomTile = p_tileMap.getTileWorldPos(m_cornerBR.x, m_cornerBR.y - 2.f);
        if ( bottomTile != nullptr && bottomTile->getType() == TILE_WATER ){
            m_speed = Vector2Scale(m_speed, 0.9f);
            m_onWater = true;
        }
    }
    // New position
    newPos.x = m_position.x + m_speed.x * 60.f * delta;
    // Right Collision
    if(m_speed.x > 0){
        newTile = p_tileMap.getTileWorldPos( (newPos.x + kPlayerWidth + 1), m_position.y );
        if(newTile != nullptr && newTile->isSolid()){
            newPos.x = newTile->getPositionWorld().x - kPlayerWidth - 1.f;
            m_speed.x = 0.f;
        }
        else{
            newTile = p_tileMap.getTileWorldPos( (newPos.x + kPlayerWidth + 1), (m_position.y + kPlayerHeight - 1) );
            if(newTile != nullptr && newTile->isSolid()){
                newPos.x = newTile->getPositionWorld().x - kPlayerWidth - 1.f;
                m_speed.x = 0.f;
            }
        }
    } // Left Collision
    else if (m_speed.x < 0){
        newTile = p_tileMap.getTileWorldPos( (newPos.x - 1), m_position.y );
        if(newTile != nullptr && newTile->isSolid()){
            newPos.x = newTile->getPositionWorld().x + p_tileMap.getTileSize() + 1.f;
            m_speed.x = 0.f;
        }
        else{
            newTile = p_tileMap.getTileWorldPos( (newPos.x - 1), (m_position.y + kPlayerHeight - 1) );
            if(newTile != nullptr && newTile->isSolid()){
                newPos.x = newTile->getPositionWorld().x + p_tileMap.getTileSize() + 1.f;
                m_speed.x = 0.f;
            }
        }
    }
    if(!m_isAlive){
        m_speed.x = 0;
    }
    else{
        if(m_speed.x < 0) m_drawSourceWidth = abs(m_drawSourceWidth) * -1;
        else if(m_speed.x > 0) m_drawSourceWidth = abs(m_drawSourceWidth);
    }
    
    //// Vertical Movement ////
    if(IsKeyPressed(KEY_SPACE))
        m_lastJumpTime = GetTime();
    else if (m_speed.y < 0 && !m_cesiumPush && IsKeyUp(KEY_SPACE))
        m_speed.y *= 0.6f;
    // Fall
    if(m_speed.y < kJumpSpeedMax * 60.f)
        m_speed.y += kGravity * 3600.f * delta;
    // Jump
    if(m_canJump && GetTime() - m_lastJumpTime < kBufferTime){
        m_speed.y = -kJumpSpeedStart * 60.f;
        m_canJump = false;
        
        for(int i = 0; i < 12; i++)
            m_jumpParticles.push_back(Particle( {m_cornerBL.x+2*i, m_cornerBL.y}, GetRandomValue(180, 359), 0.2, 2));
        if(!IsSoundPlaying(m_soundJump))
            PlaySound(m_soundJump);
    }
    // Cesium push
    if(m_onWater && m_orb.getMode() == MODE_CESIUM){
        m_speed.y = -kJumpSpeedStart * 100.f;
        m_canJump = false;
        m_cesiumPush = true;
        for(int i = -4; i < 16; i++)
            m_cesiumParticles.push_back(Particle( {m_cornerBL.x+2*i, m_cornerBL.y}, GetRandomValue(180, 359), 0.5));
        if(!IsSoundPlaying(m_soundCesiumPush))
            PlaySound(m_soundCesiumPush);
    }
    if(m_speed.y > -10)
        m_cesiumPush = false;
    // New position
    newPos.y = m_position.y + m_speed.y * delta;
    // float correction
    if(m_speed.y > -0.21f && m_speed.y < 0.21f) 
        m_speed.y = 0.0f;
    // Falling collision
    newTile = p_tileMap.getTileWorldPos( newPos.x, (newPos.y + kPlayerHeight));
    if(newTile != nullptr && newTile->isSolid()){
        newPos.y = newTile->getPositionWorld().y - kPlayerHeight;
        m_speed.y = 0.f;
        m_canJump = true;
        m_lastFloorTime = GetTime();
    }
    else{
        newTile = p_tileMap.getTileWorldPos( (newPos.x + kPlayerWidth), (newPos.y + kPlayerHeight) );
        if(newTile != nullptr && newTile->isSolid()){
            newPos.y = newTile->getPositionWorld().y - kPlayerHeight;
            m_speed.y = 0.f;
            m_canJump = true;
            m_lastFloorTime = GetTime();
        } 
        else if(GetTime() - m_lastFloorTime > kCoyoteTime) m_canJump = false;
    }
    // Jumping collision
    newTile = p_tileMap.getTileWorldPos( newPos.x, newPos.y );
    if(newTile != nullptr && newTile->isSolid()){
        newPos.y = newTile->getPositionWorld().y + kPlayerHeight;
        m_speed.y = 0.f;
    }
    else{
        newTile = p_tileMap.getTileWorldPos( (newPos.x + kPlayerWidth), newPos.y );
        if(newTile != nullptr && newTile->isSolid()){
            newPos.y = newTile->getPositionWorld().y + kPlayerHeight;
            m_speed.y = 0.f;
        }
    }
    
    // Update position
    if(m_isAlive){
        m_position = newPos;
        m_cornerTL = m_position;
        m_cornerTR = {m_position.x + kPlayerWidth, m_position.y};
        m_cornerBL = {m_position.x, m_position.y + kPlayerHeight};
        m_cornerBR = {m_position.x + kPlayerWidth, m_position.y + kPlayerHeight};
    }
    
    // Light Collision
    m_onLight = false;
    if(m_isAlive){
        for(const auto& lamp : p_tileMap.getLamps()){
            if( lamp->pointInsideLight(m_cornerTL)
             || lamp->pointInsideLight(m_cornerTR)
             || lamp->pointInsideLight(m_cornerBL)
             || lamp->pointInsideLight(m_cornerBR)){
                 m_onLight = true;
                 if(m_orb.getMode() == MODE_IRON){
                     m_isAlive = true;
                 }
                 else{
                     m_isAlive = false;
                     for(int i = 0; i < 128; i++){
                        m_deathParticles.push_back(Particle( {m_cornerTL.x+kPlayerWidth/2, m_cornerTL.y+kPlayerHeight/2}, GetRandomValue(0, 359), 0.5, 0.f, GetRandomValue(80, 120) / 10.f));
                     }
                     PlaySound(m_soundDeath);
                 }
                 break;
             }
        }
    }
    
    // Iron Charge
    m_rechargeIron = GetTime() - m_lastTimeIron >= kIronCooldown;
    if(m_rechargeIron && m_chargeIron < kIronMax){
        if(kIronMax - m_chargeIron >= kIronRecover)
            m_chargeIron += kIronRecover;
        else
            m_chargeIron += kIronMax - m_chargeIron;
        if(m_chargeIron == kIronMax)
            PlaySound(m_soundIronCharged);
    }
    
    // Orb
    OrbMode newMode;
    if(IsKeyDown(KEY_W)){
        m_lastTimeIron = GetTime();
        if(m_chargeIron > 0){
            newMode = MODE_IRON;
            m_chargeIron -= kIronConsumption;
        }
        else
            newMode = MODE_ORB;
    }
    else if(IsKeyDown(KEY_K))
        newMode = MODE_CESIUM;
    else if(IsKeyDown(KEY_L))
        newMode = MODE_NEOR;
    else if(IsKeyDown(KEY_J))
        newMode = MODE_NEOL;
    else
        newMode = MODE_ORB;
    m_orb.setTarget(m_position);
    if(m_isAlive)
        m_orb.setMode(newMode);
    m_orb.update(p_tileMap.getMagCores());
}

void Player::draw(TileMap& p_tileMap, Texture2D& p_texture){
    // light background
    if(m_onLight){
        int i = 0;
        Tile* tLeft = p_tileMap.getTileWorldPos(m_cornerBL.x, m_cornerBL.y);
        Tile* tRight = p_tileMap.getTileWorldPos(m_cornerBR.x, m_cornerBR.y);
        float lightCoverHeight = 0.f;
        bool stop = false;
        while(!stop){
            tLeft = p_tileMap.getTileWorldPos(m_cornerBL.x, m_cornerBL.y + i * kPlayerHeight);
            tRight = p_tileMap.getTileWorldPos(m_cornerBR.x, m_cornerBR.y + i * kPlayerHeight);
            if(tLeft != nullptr && tLeft->isSolid()){
                stop = true;
                lightCoverHeight = tLeft->getPositionWorld().y - m_position.y;
            }
            else if(tRight != nullptr && tRight->isSolid()){
                stop = true;
                lightCoverHeight = tRight->getPositionWorld().y - m_position.y;
            }
            else{
                i++;
            }
        }
        DrawRectangleV(m_position, {kPlayerWidth * 1.f, lightCoverHeight}, {20, 20, 20, 255});
    }
    
    // player
    if(m_orb.onFront()){
        // DrawRectangleV(m_position, {kPlayerWidth,kPlayerHeight}, WHITE);
        DrawTexturePro( p_texture
                      , { 0.f, 0.f, m_drawSourceWidth, (float)p_texture.height }
                      , { m_position.x+kPlayerWidth/2, m_position.y+kPlayerHeight/2, kPlayerWidth, kPlayerHeight }
                      , { p_texture.width / 2.f, p_texture.height / 2.f }
                      , m_speed.x * 2
                      , WHITE);
        
        m_orb.draw(m_orbTextures[m_orb.getMode()]);
    }
    else{
        m_orb.draw(m_orbTextures[m_orb.getMode()]);
        DrawTexturePro( p_texture
                      , { 0.f, 0.f, m_drawSourceWidth, (float)p_texture.height }
                      , { m_position.x+kPlayerWidth/2, m_position.y+kPlayerHeight/2, kPlayerWidth, kPlayerHeight }
                      , { p_texture.width / 2.f, p_texture.height / 2.f }
                      , m_speed.x * 2
                      , WHITE);
        // DrawRectangleV(m_position, {kPlayerWidth,kPlayerHeight}, WHITE);
    }
    
    // Iron Indicator
    if(m_isAlive && m_chargeIron < kIronMax){
        DrawRectangle(m_position.x - 30, m_position.y + 32 - m_chargeIron * 32 / kIronMax
                    , 20, m_chargeIron * 32 / kIronMax , WHITE);
        DrawRectangleLines(m_position.x - 30, m_position.y, 20, 32, GRAY);
    }
    
    // Debug
    // DrawText(std::to_string(m_speed.x).c_str(), 50, 30, 20, WHITE);
    // DrawText(std::to_string(m_canJump).c_str(), 50, 50, 20, WHITE);
    // DrawText(std::to_string(m_onLight).c_str(), 50, 70, 20, WHITE);
    // DrawText(std::to_string(m_onWater).c_str(), 50, 90, 20, WHITE);
    // DrawCircleV(m_cornerTL, 3.f, RED);
    // DrawCircleV(m_cornerTR, 3.f, RED);
    // DrawCircleV(m_cornerBL, 3.f, RED);
    // DrawCircleV(m_cornerBR, 3.f, RED);
}

void Player::drawEffects(Texture2D& p_particleTexture){
    for(auto& particle : m_jumpParticles)
        particle.draw(p_particleTexture, GRAY, false);
    for(auto& particle : m_cesiumParticles)
        particle.draw(p_particleTexture, GetRandomValue(0,6) ? BLUE : WHITE, false);
    for(auto& particle : m_deathParticles)
        particle.draw(p_particleTexture, YELLOW, false);
    m_orb.drawEffects(p_particleTexture);
}

bool Player::isAlive() const{
    return m_isAlive;
}

void Player::restart(){
    m_isAlive = true;
    m_position = { 36.f, kScreenHeight - 32.f*4 };
    m_chargeIron = kIronMax;
    m_orb.setTarget(m_position);
    m_orb.restart();
    m_speed = Vector2Zero();
    m_canJump = false;
    m_lastFloorTime = 0.0;
    m_lastJumpTime = 0.0;
    m_jumpParticles.clear();
    m_cesiumParticles.clear();
    m_deathParticles.clear();
}

float Player::getX() const{
    return m_position.x;
}

Player::~Player(){
    // Unload
    for(auto& t : m_orbTextures)
        UnloadTexture(t.second);
    m_orbTextures.clear();
    
    UnloadSound(m_soundDeath);
    UnloadSound(m_soundJump);
    UnloadSound(m_soundCesiumPush);
    UnloadSound(m_soundIronCharged);
}