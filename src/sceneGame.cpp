#include "sceneGame.h"

#include "game.h"
#include "sceneMenu.h"
#include "raylib.h"

#include <iostream>
//#include <string>

SceneGame::SceneGame(int p_screenWidth, int p_screenHeight)
    : Scene(p_screenWidth, p_screenHeight)
    , kScreenWidth(p_screenWidth), kScreenHeight(p_screenHeight)
    , m_player(p_screenHeight, p_screenWidth)
    , m_currentLevel(1)
{
    m_tileMap.loadMap(m_currentLevel);
    
    // Textures
    m_texPlayer = LoadTexture("assets/player.png");
    m_texParticle = LoadTexture("assets/particle.png");
    
    // Sounds
    m_soundStart = LoadSound("assets/start.wav");
    m_soundLevelLoad = LoadSound("assets/levelLoad.wav");
    m_soundLampDestruction = LoadSound("assets/lampDestruction.wav");
}

void SceneGame::update(Game* p_game){
    // Objects
    m_player.update(m_tileMap);
    
    for(const auto& mc : m_tileMap.getMagCores())
        mc->update(m_tileMap, m_soundLampDestruction);
    
    // Beat Level
    if(m_player.getX() > kScreenWidth && m_currentLevel < kLastLevel){
        m_player.restart();
        m_tileMap.loadMap(++m_currentLevel);
        PlaySound(m_soundLevelLoad);
    }
    
    // GAME OVER
    if(!m_player.isAlive()){
        if(IsKeyPressed(KEY_R)){
            PlaySound(m_soundLevelLoad);
            m_player.restart();
            m_tileMap.loadMap(m_currentLevel);
        }
        else if(IsKeyPressed(KEY_T)){
            p_game->changeScene(new SceneMenu(m_screenWidth, m_screenHeight));
        }
    }
    
    // LAST LEVEL
    if(m_currentLevel == kLastLevel && IsKeyPressed(KEY_T))
        p_game->changeScene(new SceneMenu(m_screenWidth, m_screenHeight));
    
    // DEBUG
    // if(IsKeyPressed(KEY_ONE))
        // p_game->changeScene(new SceneMenu(m_screenWidth, m_screenHeight));
    // else if(IsKeyPressed(KEY_TWO)){
        // m_currentLevel = 1;
        // m_tileMap.loadMap(m_currentLevel);
        // m_player.restart();
    // }
    // else if(IsKeyPressed(KEY_THREE)){
        // if(m_currentLevel > 1) m_currentLevel--;
        // m_tileMap.loadMap(m_currentLevel);
        // m_player.restart();
    // }
    // else if(IsKeyPressed(KEY_FOUR)){
        // if(m_currentLevel < kLastLevel) m_currentLevel++;
        // m_tileMap.loadMap(m_currentLevel);
        // m_player.restart();
    // }
}

void SceneGame::draw(){
    // Background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {20, 20, 20, 255});
    
    // Draw Level Guides
    if(m_currentLevel == 1){
        DrawRectangleLines(64, 64, 192, 54, PURPLE);
        DrawText("[A] & [D] to walk", 68, 68, 22, PURPLE);
        DrawText("[Space] to jump", 68, 94, 22, PURPLE);
    }
    else if(m_currentLevel == 2){
        DrawRectangleLines(64, 64, 210, 54, PURPLE);
        DrawText("Touching the light", 68, 68, 22, PURPLE);
        DrawText("will make you lose", 72, 94, 22, PURPLE);
    }
    else if(m_currentLevel == 3){
        DrawRectangleLines(64, 64, 340, 54, PURPLE);
        DrawText("[W] to transmute the orb into" , 68, 68, 22, PURPLE);
        DrawText("iron and cover from the light", 70, 94, 22, PURPLE);
    }
    else if(m_currentLevel == 4){
        DrawRectangleLines(64, 64, 512, 54, PURPLE);
        DrawText("[K] to transmute the orb into cesium, a " , 68, 68, 22, PURPLE);
        DrawText("metal that explodes upon contact with water", 70, 94, 22, PURPLE);
    }
    else if(m_currentLevel == 5){
        DrawRectangleLines(356, 64, 184, 72, PURPLE);
        DrawText("[J]&[L] to transmute" , 364, 68, 18, PURPLE);
        DrawText("the orb into a" , 389, 90, 18, PURPLE);
        DrawText("neodymium magnet" , 375, 112, 18, PURPLE);
        DrawRectangleLines(356, 140, 184, 72, PURPLE);
        DrawText("You can overload" , 370, 144, 18, PURPLE);
        DrawText("certain lamps by" , 375, 166, 18, PURPLE);
        DrawText("connecting modules" , 368, 188, 18, PURPLE);
    }
    else if(m_currentLevel == kLastLevel){
        DrawRectangleLines(445, 224, 245, 78, PURPLE);
        DrawText("Congratulations!", 477, 228, 22, PURPLE);
        DrawText("That's it for now" , 472, 252, 22, PURPLE);
        DrawText("Press [T] to go back", 452, 276, 22, PURPLE);
    }
    
    // Effects
    m_player.drawEffects(m_texParticle);
    
    // Objects
    m_tileMap.draw(m_texParticle);
    m_player.draw(m_tileMap, m_texPlayer);
    m_tileMap.drawWater();
    
    // Game Over
    if(!m_player.isAlive()){
        const char *gameOverText = "GAME OVER";
        const char *restartText = "PRESS [R] TO RESTART";
        const char *menuText = "PRESS [T] TO RETURN";
        DrawRectangle(kScreenWidth / 2 - MeasureText(restartText, 20) / 2 - 15, kScreenHeight / 2 - 25, MeasureText(restartText, 20) + 30, 130, GREEN);
        DrawRectangle(kScreenWidth / 2 - MeasureText(restartText, 20) / 2 - 10, kScreenHeight / 2 - 20, MeasureText(restartText, 20) + 20, 120, BLACK);
        DrawText(gameOverText, kScreenWidth / 2 - MeasureText(gameOverText, 20) / 2, kScreenHeight / 2, 20, GREEN);
        DrawText(restartText, kScreenWidth / 2 - MeasureText(restartText, 20) / 2, kScreenHeight / 2 + 30, 20, GREEN);
        DrawText(menuText, kScreenWidth / 2 - MeasureText(menuText, 20) / 2, kScreenHeight / 2 + 60, 20, GREEN);
    }
    
    // DEBUG
    // DrawFPS(10, 10);
    // for(unsigned int i = 0; i < asteroids.size(); i++)
        // DrawText(std::to_string(0).c_str(), 10 * i, 30, 20, WHITE);
}

SceneGame::~SceneGame(){
    // Textures
    UnloadTexture(m_texPlayer);
    UnloadTexture(m_texParticle);
    
    // Sounds
    UnloadSound(m_soundStart);
    UnloadSound(m_soundLevelLoad);
    UnloadSound(m_soundLampDestruction);
}