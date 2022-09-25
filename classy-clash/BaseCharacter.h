#include "raylib.h"
#ifndef BASE_CHARACTER_H
#define BASE_CHARACTER_H

class BaseCharacter
{
    public:
        BaseCharacter();
        Vector2 getWorldPos() { return worldPos; }
        virtual void tick(float deltaTime);
        void undoMovement();
        Rectangle getCollisionRec();
        virtual Vector2 getScreenPos() = 0;
        bool getAlive() {return alive;}
        void setAlive(bool isAlive){ alive = isAlive;}

    protected:
        Texture2D texture{LoadTexture("characters/knight_idle_spritesheet.png")};
        Texture2D idle{LoadTexture("characters/knight_idle_spritesheet.png")};
        Texture2D run{LoadTexture("characters/knight_run_spritesheet.png")};
        Vector2 worldPos{};
        Vector2 worldPosLastFrame{};
        // 1 :facing right, -1 : facing left
        float rightLeft{1.0f};
        // animation variables
        float runningTime{};
        int frame{};
        int maxFrames{6};
        float updateTime{1.0f / 12.0f};
        float speed{4.0f};
        float width{};
        float height{};
        float scale{4.0f};
        Vector2 velocity{};
    private:
        bool alive{true};
}; 
#endif