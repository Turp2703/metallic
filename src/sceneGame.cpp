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
    std::cout << "test \n";
    
    // Object Calls
    //portalManager.spawnPortal();
    for(int i = 1; i < m_tileMap.getWidth() - 1; i++)
        m_tileMap.setTile(new Tile({i*1.f,m_tileMap.getHeight()-2.f}, m_tileMap.getTileSize(), TILE_BASE, true));
    for(int i = 3; i < 10; i++)
        m_tileMap.setTile(new Tile({i*1.f,m_tileMap.getHeight()-6.f}, m_tileMap.getTileSize(), TILE_BASE, true));
    for(int i = 22; i < 29; i++)
        m_tileMap.setTile(new Tile({i*1.f,m_tileMap.getHeight()-4.f}, m_tileMap.getTileSize(), TILE_BASE, true));
    for(int i = 22; i < 29; i++)
        m_tileMap.setTile(new Tile({i*1.f,m_tileMap.getHeight()-5.f}, m_tileMap.getTileSize(), TILE_BASE, true));
    for(int i = 25; i < 28; i++)
        m_tileMap.setTile(new Tile({i*1.f,m_tileMap.getHeight()-6.f}, m_tileMap.getTileSize(), TILE_BASE, true));
    
    // Textures
    //m_texBlock = LoadTexture("assets/block.png");
    
    // Sounds
    //soundStart = LoadSound("assets/start.wav");
}

void SceneGame::update(Game* p_game){
    // Objects
    m_player.update(m_tileMap);
    
    // GAME OVER
    
    
    // DEBUG
    if(IsKeyPressed(KEY_ONE))
        p_game->changeScene(new SceneMenu(m_screenWidth, m_screenHeight));
}

void SceneGame::draw(){
    // Background
    
    // Effects
    
    // Objects
    m_tileMap.draw();
    m_player.draw();
    
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