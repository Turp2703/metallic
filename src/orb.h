#pragma once

#include "raylib.h"
#include "magCore.h"
#include <vector>

enum OrbMode{
    MODE_ORB,
    MODE_IRON,
    MODE_CESIUM,
    MODE_NEOL,
    MODE_NEOR
};

class Orb{
    public:
        Orb(Vector2, Vector2);
        void update(const std::vector<MagCore*>&);
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
        float m_followSpeed;
        OrbMode m_mode;
        Vector2 m_leftMag;
        Vector2 m_rightMag;
        MagCore* m_attachedMagCore;
};