#pragma once

#include "raylib.h"
#include "tile.h"
#include "particle.h"
#include <vector>

class Lamp : public Tile{
    public:
        Lamp(Vector2, TileType, bool, int);
        ~Lamp();
        void draw(const Texture2D&);
        void drawEffects(Texture2D&);
        bool pointInsideLight(Vector2);
        void breakLamp(Vector2);
    private:
        const double kRayDuration = 0.3;
        const float kRayThickness = 5;
        int m_height;
        Rectangle m_center;
        Vector2 m_left1;
        Vector2 m_left2;
        Vector2 m_left3;
        Vector2 m_right1;
        Vector2 m_right2;
        Vector2 m_right3;
        bool m_isBroken;
        std::vector<Particle> m_destructionParticles;
        Vector2 m_magSlotCenter;
        double m_destroyedTime;
};