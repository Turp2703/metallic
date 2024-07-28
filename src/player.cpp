#include "player.h"

#include "raylib.h"
#include "raymath.h"

// Debug
// #include <string>

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
{
    m_orbTextures[MODE_ORB] = LoadTexture("assets/orb.png");
    m_orbTextures[MODE_IRON] = LoadTexture("assets/iron.png");
    m_orbTextures[MODE_CESIUM] = LoadTexture("assets/cesium.png");
    m_orbTextures[MODE_NEOL] = LoadTexture("assets/neodymiumL.png");
    m_orbTextures[MODE_NEOR] = LoadTexture("assets/neodymiumR.png");
}

void Player::update(TileMap& p_tileMap){
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
    }
    // Cesium push
    if(m_onWater && m_orb.getMode() == MODE_CESIUM){
        m_speed.y = -kJumpSpeedStart * 100.f;
        m_canJump = false;
        m_cesiumPush = true;
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
    for(const auto& lamp : p_tileMap.getLamps()){
        if( lamp->pointInsideLight(m_cornerTL)
         || lamp->pointInsideLight(m_cornerTR)
         || lamp->pointInsideLight(m_cornerBL)
         || lamp->pointInsideLight(m_cornerBR)){
             m_onLight = true;
             m_isAlive = m_orb.getMode() == MODE_IRON;
             break;
         }
    }
    
    // Iron Charge
    m_rechargeIron = GetTime() - m_lastTimeIron >= kIronCooldown;
    if(m_rechargeIron && m_chargeIron < kIronMax){
        if(kIronMax - m_chargeIron >= kIronRecover)
            m_chargeIron += kIronRecover;
        else
            m_chargeIron += kIronMax - m_chargeIron;
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

void Player::draw(TileMap& p_tileMap){
    if(m_onLight){
        int i = 0;
        Tile* t = p_tileMap.getTileWorldPos(m_cornerBL.x, m_cornerBL.y);
        while(t == nullptr || !t->isSolid())
            t = p_tileMap.getTileWorldPos(m_cornerBL.x, m_cornerBL.y + ++i * kPlayerHeight);
        float lightCoverHeight = t->getPositionWorld().y - m_position.y;
        
        
        DrawRectangleV(m_position, {kPlayerWidth * 1.f, lightCoverHeight}, {20, 20, 20, 255});
    }
    
    if(m_orb.onFront()){
        DrawRectangleV(m_position, {kPlayerWidth,kPlayerHeight}, WHITE);
        m_orb.draw(m_orbTextures[m_orb.getMode()]);
    }
    else{
        m_orb.draw(m_orbTextures[m_orb.getMode()]);
        DrawRectangleV(m_position, {kPlayerWidth,kPlayerHeight}, WHITE);
    }
    
    // Iron Indicator
    if(m_isAlive && m_chargeIron < kIronMax){
        DrawRectangle(m_position.x - 30, m_position.y + 32 - m_chargeIron * 32 / kIronMax
                    , 20, m_chargeIron * 32 / kIronMax , WHITE);
        DrawRectangleLines(m_position.x - 30, m_position.y, 20, 32, GRAY);
    }
    
    // Debug
    // DrawText(std::to_string(m_speed.y).c_str(), 50, 30, 20, WHITE);
    // DrawText(std::to_string(m_canJump).c_str(), 50, 50, 20, WHITE);
    // DrawText(std::to_string(m_onLight).c_str(), 50, 70, 20, WHITE);
    // DrawText(std::to_string(m_onWater).c_str(), 50, 90, 20, WHITE);
    // DrawCircleV(m_cornerTL, 3.f, RED);
    // DrawCircleV(m_cornerTR, 3.f, RED);
    // DrawCircleV(m_cornerBL, 3.f, RED);
    // DrawCircleV(m_cornerBR, 3.f, RED);
}

bool Player::isAlive() const{
    return m_isAlive;
}

void Player::restart(){
    m_isAlive = true;
    m_position = { 36.f, kScreenHeight - 32.f*4 };
    m_chargeIron = kIronMax;
    m_orb.restart();
    m_speed = Vector2Zero();
    m_canJump = false;
    m_lastFloorTime = 0.0;
    m_lastJumpTime = 0.0;
}

float Player::getX() const{
    return m_position.x;
}

Player::~Player(){
    // Unload
    for(auto& t : m_orbTextures)
        UnloadTexture(t.second);
    m_orbTextures.clear();
}