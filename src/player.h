#pragma once

#include "raylib.h"
#include "tileMap.h"
#include "orb.h"
#include <vector>
#include <unordered_map>

class Player{
    public:
        Player(int, int);
        void update(TileMap&);
        void draw(TileMap&);
        ~Player();
    private:
        const int kScreenHeight;
        const int kScreenWidth;
        const float kPlayerWidth = 24;
        const float kPlayerHeight = 32;
        const int kHorSpeedMax = 5; // 5
        const float kHorAccel = 0.2f; // 0.2f
        const float kHorFrict = 0.4f; // 0.4f
        const int kJumpSpeedMax = 10; // 10
        const int kJumpSpeedStart = 10; // 10
        const float kGravity = 0.5f; // 0.5f
        const double kBufferTime = 0.1; // 0.1
        const double kCoyoteTime = 0.05; // 0.05
        const int kIronMax = 7000; // 7000
        const int kIronCooldown = 1; // 1
        const int kIronRecover = 200; // 200
        const int kIronConsumption = 100; // 100
        Vector2 m_position;
        Vector2 m_speed;
        Vector2 m_acceleration;
        bool m_canJump;
        double m_lastFloorTime;
        double m_lastJumpTime;
        Vector2 m_cornerTL;
        Vector2 m_cornerTR;
        Vector2 m_cornerBL;
        Vector2 m_cornerBR;
        Orb m_orb;
        std::unordered_map<OrbMode,Texture2D> m_orbTextures;
        bool m_onLight;
        bool m_onWater;
        bool m_cesiumPush;
        int m_chargeIron;
        bool m_rechargeIron;
        double m_lastTimeIron;
};