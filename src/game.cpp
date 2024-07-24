#include "game.h"

#include "sceneGame.h"
#include "sceneMenu.h"

#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

Game::Game(){
    /* */
}

void UpdateDrawFrame(void* arg){
    Game* instance = static_cast<Game*>(arg);
    instance->update();
    BeginDrawing();
        instance->draw();
    EndDrawing();
}
void Game::init(const char* p_title){
    InitWindow(k_screenWidth, k_screenHeight, p_title);
    InitAudioDevice();

    m_currentScene = new SceneMenu(k_screenWidth, k_screenHeight);
    // m_currentScene = new SceneGame(k_screenWidth, k_screenHeight);
    
    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop_arg(&UpdateDrawFrame, this, 0, 1);
    #else
        SetTargetFPS(60);
        while (!WindowShouldClose()) { UpdateDrawFrame(this); }
    #endif
}

void Game::update(){
    m_currentScene->update(this);
}

void Game::draw(){
    ClearBackground(BLACK);
    m_currentScene->draw();
}

void Game::changeScene(Scene* newScene){
    delete m_currentScene;
    m_currentScene = newScene;
}

void Game::shutdown(){
    delete m_currentScene;
    CloseWindow();
}