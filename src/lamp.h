#pragma once

#include "raylib.h"
#include "tile.h"

class Lamp : public Tile{
    public:
        Lamp(Vector2, TileType, bool, int);
        ~Lamp();
        void draw(const Texture2D&);
        bool pointInsideLight(Vector2);
    private:
        int m_height;
        Rectangle m_center;
        Vector2 m_left1;
        Vector2 m_left2;
        Vector2 m_left3;
        Vector2 m_right1;
        Vector2 m_right2;
        Vector2 m_right3;
};


// Big lamp? 3 tiles width (0.5-1-0.5)