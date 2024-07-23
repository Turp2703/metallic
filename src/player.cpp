#include "player.h"

#include "raylib.h"
#include "raymath.h"

// Debug
#include <string>

Player::Player(int p_screenHeight, int p_screenWidth)
    : kScreenHeight(p_screenHeight), kScreenWidth(p_screenWidth)
    , m_position({ p_screenWidth / 2.f, p_screenHeight / 2.f })
    , m_speed(Vector2Zero()), m_acceleration(Vector2Zero())
    , m_canJump(false), m_lastFloorTime(0.0), m_lastJumpTime(0.0)
    , m_cornerTL(m_position)
    , m_cornerTR({m_position.x+kPlayerSize,m_position.y})
    , m_cornerBL({m_position.x,m_position.y+kPlayerSize})
    , m_cornerBR(Vector2AddValue(m_position,kPlayerSize))
    , m_onLight(false)
{
    /* */
}

void Player::update(TileMap& p_tileMap){
    ///// MOVEMENT /////
    Vector2 newPos;
    Tile* newTile;
    
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
    newPos.x = m_position.x + m_speed.x;
    // Right Collision
    if(m_speed.x > 0){
        newTile = p_tileMap.getTileWorldPos( (newPos.x + kPlayerSize + 1), m_position.y );
        if(newTile != nullptr && newTile->isSolid()){
            newPos.x = newTile->getPositionWorld().x - kPlayerSize - 1.f;
            m_speed.x = 0.f;
        }
        else{
            newTile = p_tileMap.getTileWorldPos( (newPos.x + kPlayerSize + 1), (m_position.y + kPlayerSize - 1) );
            if(newTile != nullptr && newTile->isSolid()){
                newPos.x = newTile->getPositionWorld().x - kPlayerSize - 1.f;
                m_speed.x = 0.f;
            }
        }
    } // Left Collision
    else if (m_speed.x < 0){
        newTile = p_tileMap.getTileWorldPos( (newPos.x - 1), m_position.y );
        if(newTile != nullptr && newTile->isSolid()){
            newPos.x = newTile->getPositionWorld().x + kPlayerSize + 1.f;
            m_speed.x = 0.f;
        }
        else{
            newTile = p_tileMap.getTileWorldPos( (newPos.x - 1), (m_position.y + kPlayerSize - 1) );
            if(newTile != nullptr && newTile->isSolid()){
                newPos.x = newTile->getPositionWorld().x + kPlayerSize + 1.f;
                m_speed.x = 0.f;
            }
        }
    }
    
    //// Vertical Movement ////
    if(IsKeyPressed(KEY_SPACE))
        m_lastJumpTime = GetTime();
    else if (m_speed.y < 0 && IsKeyUp(KEY_SPACE))
        m_speed.y *= 0.6f;
    // Fall
    if(m_speed.y < kJumpSpeedMax)
        m_speed.y += kGravity;
    // Jump
    if(m_canJump && GetTime() - m_lastJumpTime < kBufferTime){
        m_speed.y = -kJumpSpeedStart;
        m_canJump = false;
    }
    newPos.y = m_position.y + m_speed.y;
    // float correction
    if(m_speed.y > -0.21f && m_speed.y < 0.21f) 
        m_speed.y = 0.0f;
    // Falling Collision
    newTile = p_tileMap.getTileWorldPos( newPos.x, (newPos.y + kPlayerSize));
    if(newTile != nullptr && newTile->isSolid()){
        newPos.y = newTile->getPositionWorld().y - kPlayerSize;
        m_speed.y = 0.f;
        m_canJump = true;
        m_lastFloorTime = GetTime();
    }
    else{
        newTile = p_tileMap.getTileWorldPos( (newPos.x + kPlayerSize), (newPos.y + kPlayerSize) );
        if(newTile != nullptr && newTile->isSolid()){
            newPos.y = newTile->getPositionWorld().y - kPlayerSize;
            m_speed.y = 0.f;
            m_canJump = true;
            m_lastFloorTime = GetTime();
        } 
        else if(GetTime() - m_lastFloorTime > kCoyoteTime) m_canJump = false;
    }
    // Jumping Collision
    newTile = p_tileMap.getTileWorldPos( newPos.x, newPos.y );
    if(newTile != nullptr && newTile->isSolid()){
        newPos.y = newTile->getPositionWorld().y + kPlayerSize;
        m_speed.y = 0.f;
    }
    else{
        newTile = p_tileMap.getTileWorldPos( (newPos.x + kPlayerSize), newPos.y );
        if(newTile != nullptr && newTile->isSolid()){
            newPos.y = newTile->getPositionWorld().y + kPlayerSize;
            m_speed.y = 0.f;
        }
    }
    
    // Update position
    m_position = newPos;
    m_cornerTL = m_position;
    m_cornerTR = {m_position.x + kPlayerSize, m_position.y};
    m_cornerBL = {m_position.x, m_position.y + kPlayerSize};
    m_cornerBR = Vector2AddValue(m_position, kPlayerSize);
    
    // Light Collision
    m_onLight = false;
    for(const auto& lamp : p_tileMap.getLamps()){
        if( lamp->pointInsideLight(m_cornerTL)
         || lamp->pointInsideLight(m_cornerTR)
         || lamp->pointInsideLight(m_cornerBL)
         || lamp->pointInsideLight(m_cornerBR)){
             m_onLight = true;
             break;
         }
    }
}

void Player::draw(){
    DrawRectangleV(m_position, {kPlayerSize,kPlayerSize}, WHITE);
    
    // Debug
    DrawText(std::to_string(m_speed.y).c_str(), 50, 30, 20, WHITE);
    DrawText(std::to_string(m_canJump).c_str(), 50, 50, 20, WHITE);
    DrawText(std::to_string(m_onLight).c_str(), 50, 70, 20, WHITE);
    DrawText(std::to_string(m_onWater).c_str(), 50, 90, 20, WHITE);
    DrawCircleV(m_cornerTL, 3.f, RED);
    DrawCircleV(m_cornerTR, 3.f, RED);
    DrawCircleV(m_cornerBL, 3.f, RED);
    DrawCircleV(m_cornerBR, 3.f, RED);
}

Player::~Player(){
    // Unload
}