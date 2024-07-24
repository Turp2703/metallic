#pragma once

#include "raylib.h"

enum OrbMode{
    MODE_ORB,
    MODE_IRON,
    MODE_CESIUM
};

class Orb{
    public:
        Orb(Vector2, Vector2);
        void update();
        void draw(Texture2D&);
        bool onFront() const;
        void setTarget(Vector2);
        OrbMode getMode() const;
        void setMode(OrbMode);
    private:
        const int kOrbitSpeed = 2;
        Vector2 m_position;
        Vector2 m_target;
        int m_orbit;
        bool m_onFront;
        OrbMode m_mode;
};