#pragma once

class Scene;

class Game{
    public:
        Game();
        void init(const char*);
        void update();
        void draw();
        void changeScene(Scene*);
    private:
        const int k_screenWidth = 1024;
        const int k_screenHeight = 576;
        void shutdown();
        Scene* m_currentScene;
};