#pragma once

#include "scene.h"
#include "player.h"
#include "tileMap.h"

class SceneGame : public Scene{
    public:
        SceneGame(int, int);
        void update(Game*);
        void draw();
        ~SceneGame();
    private:        
        // Objects
        Player m_player;
        TileMap m_tileMap;
        
        // Textures
        //Texture2D m_texBlock;

        // Sounds
        //Sound soundStart;
};