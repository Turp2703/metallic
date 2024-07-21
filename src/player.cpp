#include "player.h"

#include "raylib.h"
#include "raymath.h"

// Debug
#include <string>

Player::Player(int p_screenHeight, int p_screenWidth)
    : kScreenHeight(p_screenHeight), kScreenWidth(p_screenWidth)
    , m_canJump(false), m_isJumping(false)
    , m_lastFloorTime(0.0), m_lastJumpTime(0.0)
{
    m_position = { p_screenWidth / 2.f, p_screenHeight / 2.f };
    m_speed = Vector2Zero();
    m_acceleration = Vector2Zero();
    m_hitBox = {m_position.x, m_position.y, kPlayerSize, kPlayerSize};
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
    newPos.x = m_position.x + m_speed.x;
    // Right Collision
    if(m_speed.x > 0){
        newTile = p_tileMap.getTile( (newPos.x + kPlayerSize + 1) / p_tileMap.getTileSize()
                                    , m_position.y / p_tileMap.getTileSize());
        if(newTile != nullptr && newTile->isSolid()){
            newPos.x = newTile->getPosition().x * p_tileMap.getTileSize() - kPlayerSize - 1.f;
            m_speed.x = 0.f;
        }
        else{
            newTile = p_tileMap.getTile( (newPos.x + kPlayerSize + 1) / p_tileMap.getTileSize()
                                       , (m_position.y + kPlayerSize - 1) / p_tileMap.getTileSize());
            if(newTile != nullptr && newTile->isSolid()){
                newPos.x = newTile->getPosition().x * p_tileMap.getTileSize() - kPlayerSize - 1.f;
                m_speed.x = 0.f;
            }
        }
    } // Left Collision
    else if (m_speed.x < 0){
        newTile = p_tileMap.getTile( (newPos.x - 1) / p_tileMap.getTileSize()
                                   , m_position.y / p_tileMap.getTileSize());
        if(newTile != nullptr && newTile->isSolid()){
            newPos.x = newTile->getPosition().x * p_tileMap.getTileSize() + kPlayerSize + 1.f;
            m_speed.x = 0.f;
        }
        else{
            newTile = p_tileMap.getTile( (newPos.x - 1) / p_tileMap.getTileSize()
                                       , (m_position.y + kPlayerSize - 1) / p_tileMap.getTileSize());
            if(newTile != nullptr && newTile->isSolid()){
                newPos.x = newTile->getPosition().x * p_tileMap.getTileSize() + kPlayerSize + 1.f;
                m_speed.x = 0.f;
            }
        }
    }
    
    //// Vertical Movement ////
    if(IsKeyPressed(KEY_SPACE))
        m_lastJumpTime = GetTime();
    else if (m_speed.y < 0 && IsKeyUp(KEY_SPACE)){
        m_speed.y *= 0.6f;
        m_isJumping = false;
    }
    // Fall
    if(!m_isJumping && m_speed.y < kJumpSpeedMax)
        m_speed.y += kGravity;
    // Jump
    if(m_canJump && GetTime() - m_lastJumpTime < kBufferTime){
        m_speed.y = -kJumpSpeedStart;
        m_isJumping = true;
        m_canJump = false;
    }
    else
        m_isJumping = false;
    newPos.y = m_position.y + m_speed.y;
    // float correction
    if(m_speed.y > -0.21f && m_speed.y < 0.21f) 
        m_speed.y = 0.0f;
    // Falling Collision
    newTile = p_tileMap.getTile( newPos.x / p_tileMap.getTileSize()
                               , (newPos.y + kPlayerSize) / p_tileMap.getTileSize());
    if(newTile != nullptr && newTile->isSolid()){
        newPos.y = newTile->getPosition().y * p_tileMap.getTileSize() - kPlayerSize;
        m_speed.y = 0.f;
        m_canJump = true;
        m_lastFloorTime = GetTime();
    }
    else{
        newTile = p_tileMap.getTile( (newPos.x + kPlayerSize) / p_tileMap.getTileSize()
                                   , (newPos.y + kPlayerSize) / p_tileMap.getTileSize());
        if(newTile != nullptr && newTile->isSolid()){
            newPos.y = newTile->getPosition().y * p_tileMap.getTileSize() - kPlayerSize;
            m_speed.y = 0.f;
            m_canJump = true;
            m_lastFloorTime = GetTime();
        } 
        else if(GetTime() - m_lastFloorTime > kCoyoteTime) m_canJump = false;
    }
    // Jumping Collision
    newTile = p_tileMap.getTile( newPos.x / p_tileMap.getTileSize()
                               , newPos.y / p_tileMap.getTileSize());
    if(newTile != nullptr && newTile->isSolid()){
        newPos.y = newTile->getPosition().y * p_tileMap.getTileSize() + kPlayerSize;
        m_speed.y = 0.f;
        m_isJumping = false;
    }
    else{
        newTile = p_tileMap.getTile( (newPos.x + kPlayerSize) / p_tileMap.getTileSize()
                                   , newPos.y / p_tileMap.getTileSize());
        if(newTile != nullptr && newTile->isSolid()){
            newPos.y = newTile->getPosition().y * p_tileMap.getTileSize() + kPlayerSize;
            m_speed.y = 0.f;
            m_isJumping = false;
        }
    }
    
    // Update position
    m_position = newPos;
    m_hitBox.x = m_position.x; m_hitBox.y = m_position.y;
}

void Player::draw(){
    DrawRectangleV(m_position, {kPlayerSize,kPlayerSize}, WHITE);
    
    // Debug
    DrawText(std::to_string(m_speed.y).c_str(), 50, 30, 20, WHITE);
    DrawText(std::to_string(m_canJump).c_str(), 50, 50, 20, WHITE);
    DrawText(std::to_string(m_isJumping).c_str(), 50, 70, 20, WHITE);
    DrawRectangleLinesEx(m_hitBox, 2.f, RED);
}

Player::~Player(){
    // Unload
}