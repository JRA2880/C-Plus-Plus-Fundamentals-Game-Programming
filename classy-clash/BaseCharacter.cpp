#include "BaseCharacter.h"
#include "raymath.h"

BaseCharacter::BaseCharacter()
{
     
} 

void BaseCharacter::undoMovement()
{
    worldPos = worldPosLastFrame;
} 

Rectangle BaseCharacter::getCollisionRec()
{ 
    return Rectangle{
        getScreenPos().x,
        getScreenPos().y,
        width * scale,
        height * scale
    };

} 

void BaseCharacter::tick(float deltaTime)
{   
    worldPosLastFrame = worldPos;
    // update animation frame
    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
        frame++;
        runningTime = 0.0f;
        if (frame > maxFrames)
            frame = 0;
    }
    if (Vector2Length(velocity) != 0.0)
    {
        // Set worldPos = worldPos + direction
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), speed));
        velocity.x < 0.0f ? rightLeft = -1.0f : rightLeft = 1.0f;
        texture = run;
    }
    else
    {
        texture = idle;
    } 
    velocity = {};
    // draw character
    Rectangle source{frame * width, 0.f, rightLeft * width, height};
    Rectangle dest{getScreenPos().x, getScreenPos().y, scale * width, scale * height};
    Vector2 origin{};
    DrawTexturePro(texture, source, dest, origin, 0.f, WHITE);
}