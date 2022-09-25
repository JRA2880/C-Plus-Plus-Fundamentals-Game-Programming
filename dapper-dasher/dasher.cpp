#include "raylib.h"

// Struct data for Animation Data
struct AnimData
{ 
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
}; 
// Fuction to test if animation object is on the ground
bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
} 
// Update function to update the animation object
AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{ 
    // update running time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        //update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if(data.frame > maxFrame)
        {
            data.frame = 0;
        }
    } 
    return data;
}

int main()
{   
    // Window Dimensions Array
    int windowDimensions[2] = {512,380};
    // Initialize Window
    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher!");
    // acceleration due to gravity (pixels/s)/s
    const int gravity{1'000};

    // Nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    // AnimData Array
    const int sizeOfNebulae{3};
    AnimData nebulae[sizeOfNebulae]{};
    for (int i = 0; i < sizeOfNebulae; i++)
    { 
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;
        nebulae[i].pos.x = windowDimensions[0] + i * 300;
    } 
    //Finish Line Value
    float finishLine{nebulae[sizeOfNebulae - 1].pos.x};
    // Nnebula X velocity (pixels/second)
    int nebVel{-200}; 
    // Scarfy variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width / 6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0] / 2 - scarfyData.rec.width / 2;
    scarfyData.pos.y = windowDimensions[1] / 2 - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;
    // Is the rectangle in the air?
    bool isInAir;
    // Jump velocity (pixels/second)
    const int jumpVel{-600};
    int velocity{0};
    // background image
    Texture2D background = LoadTexture("textures/far-buildings.png");
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float bgX{};
    float mgX{};
    float fgX{};
    bool collision{false};

    //Set Target frames per second
    SetTargetFPS(60);
    //Game loop
    while (!WindowShouldClose())
    {   
        //delta time
        const float dT{GetFrameTime()}; 
        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);
        //Scroll the background
        bgX -= 20 * dT;
        if(bgX <= -background.width * 2)
        {
            bgX = 0.0;
        } 
        //Scroll the midground
         mgX -= 40 * dT;
        if(mgX <= -midground.width * 2)
        {
            mgX = 0.0;
        }
        //Scroll the foreground
        fgX -= 80 * dT;
         if(fgX <= -foreground.width * 2)
        {
            fgX = 0.0;
        } 
        //Draw the background
        Vector2 background1Pos{bgX,0.0};
        DrawTextureEx(background,background1Pos,0.0,2.0,WHITE);
        Vector2 background2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background,background2Pos,0.0,2.0,WHITE);
        //Draw the midground
        Vector2 midground1Pos{mgX,0.0};
        DrawTextureEx(midground,midground1Pos,0.0,2.0,WHITE);
        Vector2 midground2Pos{mgX + midground.width * 2, 0.0};
        DrawTextureEx(midground,midground2Pos,0.0,2.0,WHITE);
        //Draw the foreground
        Vector2 foreground1Pos{fgX,0.0};
        DrawTextureEx(foreground,foreground1Pos,0.0,2.0,WHITE);
        Vector2 foreground2Pos{fgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground,foreground2Pos,0.0,2.0,WHITE);

        // Peform Ground Check
        if (isOnGround(scarfyData,windowDimensions[1]))
        {
            // rectangle is on the ground
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // rectangle is in the air
            velocity += gravity * dT;
            isInAir = true;
        }
        // Jump Check
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }
        for(int i = 0; i < sizeOfNebulae; i++)
        {    
            // update the position of each nebula
             nebulae[i].pos.x += nebVel * dT;
        } 
        // Update Finish Line
        finishLine += nebVel * dT;
        // Update scarfy position
        scarfyData.pos.y += velocity * dT;
        // update scarfy's animation frame
        if(!isInAir)
        {   
            scarfyData = updateAnimData(scarfyData,dT,5); 
        } 
        for(int i = 0; i < sizeOfNebulae; i++)
        {  
            nebulae[i] = updateAnimData(nebulae[i],dT,7);
            
        }
        //Check for collision 
        
        for (AnimData nebula : nebulae)
        {   
            float pad{50};
            Rectangle nebRec {
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2* pad,
                nebula.rec.height - 2 * pad
            };
            Rectangle scarfyRec {
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            if(CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }

        } 
        if(collision)
        {
            // lose the game
            DrawText("Game Over!",windowDimensions[0]/4,windowDimensions[1]/2,40,RED);
        } 
        else if(scarfyData.pos.x >= finishLine)
        {
            //Win the game
             DrawText("You Win!",windowDimensions[0]/4,windowDimensions[1]/2,40,BLUE);
        }
        else
        {
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebula,nebulae[i].rec,nebulae[i].pos,WHITE);
            }
            //Draw Scarfy      
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }
        // stop drawing
        EndDrawing();
    }
    //Unload scafy texture
    UnloadTexture(scarfy);
    //Unload Nebula texture
    UnloadTexture(nebula);
    //Unload background
    UnloadTexture(background);
    //Unload midground
    UnloadTexture(midground);
    //Unload foreground
    UnloadTexture(foreground);
    //Close game window
    CloseWindow();
}