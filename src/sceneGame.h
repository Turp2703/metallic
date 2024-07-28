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
        const int kScreenWidth;
        const int kScreenHeight;
        const int kLastLevel = 4;
    
        // Objects
        Player m_player;
        TileMap m_tileMap;
        int m_currentLevel;
        
        // Textures
        //Texture2D m_texBlock;

        // Sounds
        //Sound soundStart;
};