#pragma once

#include "raylib.h"
#include "tileMap.h"
#include <vector>

class Player{
    public:
        Player(int, int);
        void update(TileMap&);
        void draw();
        ~Player();
    private:
        const int kScreenHeight;
        const int kScreenWidth;
        const float kPlayerSize = 32;
        const int kHorSpeedMax = 5; // 5
        const float kHorAccel = 0.2f; // 0.2f
        const float kHorFrict = 0.4f; // 0.4f
        const int kJumpSpeedMax = 10; // 10
        const int kJumpSpeedStart = 10; // 8
        const float kJumpAccel = 1.1f; // 1.f
        const float kGravity = 0.5f; // 1.f
        const double kBufferTime = 0.1; // 0.1
        const double kCoyoteTime = 0.05; // 0.05
        Vector2 m_position;
        Vector2 m_speed;
        Vector2 m_acceleration;
        bool m_canJump;
        bool m_isJumping;
        double m_lastFloorTime;
        double m_lastJumpTime;
        Rectangle m_hitBox;
};