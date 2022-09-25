#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

int main()
{
    const int windowWidth{384};
    const int windowHeight{384};
    InitWindow(windowWidth, windowHeight, "Classy Clash!");

    Texture2D map = LoadTexture("nature_tileset/OpenWorldMap24x24.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f};
    Character knight{windowWidth,windowHeight};

    Prop props[2]{
         Prop{Vector2{600.0f,300.0f},LoadTexture("nature_tileset/Rock.png")},
         Prop{Vector2{400.0f,500.0f},LoadTexture("nature_tileset/Log.png")}
    }; 

    Enemy goblin {
        Vector2{800.0f,300.0f},
        LoadTexture("characters/goblin_idle_spritesheet.png"),
        LoadTexture("characters/goblin_run_spritesheet.png")
    };  

    Enemy slime{
         Vector2{500.0f, 700.0f},
         LoadTexture("characters/slime_idle_spritesheet.png"),
         LoadTexture("characters/slime_run_spritesheet.png")
    }; 

    Enemy* enemies[]{
        &goblin,
        &slime
    }; 
    for (auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        mapPos = Vector2Scale(knight.getWorldPos(), -1.0f);
        // draw map
        DrawTextureEx(map, mapPos, 0, mapScale, WHITE);
        
        // draw the props
        for(auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        } 

        // character health
        if(!knight.getAlive()) // character is not alive
        {
            DrawText("Game Over!", 55.0f, 45.0f, 40,RED);
            EndDrawing();
            continue;
        } 
        else // character is alive
        {  
            std::string knightsHealth = "Health: ";
            knightsHealth.append(std::to_string(knight.getHealth()),0,5);
            DrawText(knightsHealth.c_str(),55.0f,45.0f,40,RED);
        }

        knight.tick(GetFrameTime());
        // check map bounds
        if(knight.getWorldPos().x < 0.0f  || 
           knight.getWorldPos().y < 0.0f  ||
           knight.getWorldPos().x + windowWidth > map.width * mapScale ||
           knight.getWorldPos().y + windowHeight > map.height * mapScale)
        {
            knight.undoMovement();
        } 
        // check for prop collsions
        for (auto prop: props)
        {
           if(CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()),knight.getCollisionRec()))
           {
             knight.undoMovement();
           }
        } 
        for(auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        } 
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {   
            for(auto enemy : enemies)
            {
                if(CheckCollisionRecs(enemy->getCollisionRec(),knight.getWeaponCollisonRec()))
                {
                    enemy->setAlive(false);
                }
            }     
        } 
        EndDrawing();
    }     
    CloseWindow();
}