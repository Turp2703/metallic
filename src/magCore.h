#pragma once

#include "raylib.h"

class TileMap;

class MagCore{
    public:
        MagCore(Vector2);
        void update(TileMap&, Sound&);
        void draw(Texture2D&);
        void attach(Vector2);
        void detach();
        Rectangle getHitBox();
        bool onSlot() const;
    private:
        const int kLevitSpeed = 4;
        Vector2 m_position;
        int m_levitation;
        bool m_attached;
        Vector2 m_target;
        float m_followSpeed;
        bool m_onSlot;
};