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
        const int kLastLevel = 9;
    
        // Objects
        Player m_player;
        TileMap m_tileMap;
        int m_currentLevel;
        
        // Textures
        Texture2D m_texPlayer;
        Texture2D m_texParticle;

        // Sounds
        Sound m_soundStart;
        Sound m_soundLevelLoad;
        Sound m_soundLampDestruction;
};