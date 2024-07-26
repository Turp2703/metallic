#include "sceneGame.h"

#include "game.h"
#include "sceneMenu.h"
#include "raylib.h"

#include <iostream>
//#include <string>

SceneGame::SceneGame(int p_screenWidth, int p_screenHeight)
    : Scene(p_screenWidth, p_screenHeight)
    , m_player(p_screenHeight, p_screenWidth)
{
    // Init objects on initializer list
    
    // Object Calls
    m_tileMap.loadMap(1);
    
    // Textures
    //m_texBlock = LoadTexture("assets/block.png");
    
    // Sounds
    //soundStart = LoadSound("assets/start.wav");
}

void SceneGame::update(Game* p_game){
    // Objects
    m_player.update(m_tileMap);
    
    for(const auto& mc : m_tileMap.getMagCores())
        mc->update(m_tileMap);
    
    // GAME OVER
    
    
    // DEBUG
    if(IsKeyPressed(KEY_ONE))
        p_game->changeScene(new SceneMenu(m_screenWidth, m_screenHeight));
    else if(IsKeyPressed(KEY_TWO))
        m_tileMap.loadMap(1);
    else if(IsKeyPressed(KEY_THREE))
        m_tileMap.loadMap(2);
    else if(IsKeyPressed(KEY_FOUR))
        m_tileMap.loadMap(3);
    else if(IsKeyPressed(KEY_FIVE))
        m_tileMap.loadMap(4);
}

void SceneGame::draw(){
    // Background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKGRAY);
    
    // Effects
    
    // Objects
    m_tileMap.draw();
    m_player.draw(m_tileMap);
    m_tileMap.drawWater();
    
    // UI
    
    // DEBUG
    DrawFPS(10, 10);
    // for(unsigned int i = 0; i < asteroids.size(); i++)
        // DrawText(std::to_string(0).c_str(), 10 * i, 30, 20, WHITE);
}

SceneGame::~SceneGame(){
    // Textures
    //UnloadTexture(m_texBlock);
    
    // Sounds
    //UnloadSound(soundStart);
}