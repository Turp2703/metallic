#include "sceneMenu.h"

#include "game.h"
#include "sceneGame.h"

#include <iostream>

SceneMenu::SceneMenu(int p_screenWidth, int p_screenHeight)
    : Scene(p_screenWidth, p_screenHeight)
{
    texLogo = LoadTexture("assets/logo.png");
    
    mouseOverButton = false;
    transitioning = false;
    alpha = 0;
    
    soundStart = LoadSound("assets/start.wav"); 
}

void SceneMenu::update(Game* p_game){
    mouseOverButton = CheckCollisionPointRec(GetMousePosition(), button);
    if(transitioning){
        if(alpha < 252)
            alpha += 3;
        else
            p_game->changeScene(new SceneGame(m_screenWidth, m_screenHeight));
    }
    else if((mouseOverButton && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_ENTER)){
        transitioning = true;
        PlaySound(soundStart);
    }
    
    // DEBUG
    if(IsKeyPressed(KEY_TWO))
        p_game->changeScene(new SceneGame(m_screenWidth, m_screenHeight));
}

void SceneMenu::draw(){
    // Title
    DrawTexturePro( texLogo
                  , { 0.f, 0.f, (float)texLogo.width, (float)texLogo.height }
                  , { 50.f, 50.f, 924.f, 125.f }
                  , { 0.f, 0.f }
                  , 0.f
                  , WHITE);
    
    // Play button
    if(mouseOverButton){
        DrawRectangleRec(button, GRAY);
        DrawText("PLAY", 512 - MeasureText("PLAY", 50) / 2, 250, 50, BLACK);
    }
    else{
        DrawText("PLAY", 512 - MeasureText("PLAY", 50) / 2, 250, 50, RAYWHITE);
    }
    DrawRectangleLinesEx(button, 2.f, RAYWHITE);
    
    // Guide
    DrawRectangleLines(15, 350, 984, 191, RAYWHITE);
    DrawLine(697, 350, 697, 539, RAYWHITE);
    DrawText("QUICK GUIDE -> Light kills you, avoid it.", 25, 360, 24, RAYWHITE);
    DrawText("Some lamps can be overloaded by energized slots.", 25, 390, 24, RAYWHITE);
    DrawText("You can transmute your orb into:", 25, 420, 24, RAYWHITE);
    DrawText(" - IRON that covers you from the light", 25, 450, 24, RAYWHITE);
    DrawText(" - NEODYMIUM that magnetically attracts other metals", 25, 480, 24, RAYWHITE);
    DrawText(" - CESIUM that explodes on contact with water", 25, 510, 24, RAYWHITE);
    DrawText("[A] to move left", 710, 360, 24, RAYWHITE);
    DrawText("[D] to move right", 710, 390, 24, RAYWHITE);
    DrawText("[Space] to jump", 710, 420, 24, RAYWHITE);
    DrawText("[W] to use iron", 710, 450, 24, RAYWHITE);
    DrawText("[J]&[L] to use neodymium", 710, 480, 24, RAYWHITE);
    DrawText("[K] to use cesium", 710, 510, 24, RAYWHITE);
    
    if(transitioning)
        DrawRectangle(0, 0, m_screenWidth, m_screenHeight, { 0, 0, 0, alpha } );
}

SceneMenu::~SceneMenu(){
    UnloadTexture(texLogo);
    UnloadSound(soundStart);
}