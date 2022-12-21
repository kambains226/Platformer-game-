#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos; 
    int frame; 
    float updatetime;
    float runningTime;
};
bool isOnGround(AnimData data, int WindowHeight ){
    return data.pos.y >= WindowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime,int maxFrame)
{
    //update running time 
    data.runningTime+= deltaTime;
    if (data.runningTime >= data.updatetime){
        
        data.runningTime =0.0;
        //update animation frame
        data.rec.x = data.frame*data.rec.width;
        data.frame++;
        if(data.frame > maxFrame){
            data.frame=0;
        }

    }
    return data;
}
int main(){
    //array with window dimnesios 
    int WindowDimensions[2];
    WindowDimensions[0] = 512;
    WindowDimensions[1] = 380;
    
    InitWindow(WindowDimensions[0], WindowDimensions[1] ,"Dasher game ");
    //acceleration due to gravity (pixels per frame)
    const int gravity{1'000};
    const int jumpVel{-600};
    bool isInAir{};

    //nebula
    Texture2D nebula =LoadTexture("textures/12_nebula_spritesheet.png");

    
    const int sizeofnebulae{10};
    AnimData nebulae[sizeofnebulae]{};

    for (int i =0; i < sizeofnebulae; i++ )
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width= nebula.width/8;
        nebulae[i].rec.height= nebula.height/8;
        nebulae[i].pos.y= WindowDimensions[1] -nebula.height/8;
        nebulae[i].frame =0;
        nebulae[i].runningTime = 0.0; 
        nebulae[i].updatetime = 0.0;
        nebulae[i].pos.x =  WindowDimensions[0] +i * 300;
    }
    float finishline { nebulae[sizeofnebulae - 1 ].pos.x };


    
    
    
    //nebula x velocity (p/s)
    int nebVel{-200};


    
    
    //scarfy varianles


    Texture2D scarfy = LoadTexture("textures/scarfy.png"); 
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6; 
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = WindowDimensions[0]/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = WindowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame =0;
    scarfyData.updatetime =1.0/12.0;
    scarfyData.runningTime =0.0;

   
    
    
    
   


    

    int velocity{0};


    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    
    Texture2D midground=LoadTexture("textures/back-buildings.png");
    float mgX{};
    Texture2D foreground=LoadTexture("textures/foreground.png");
    float fgX{};
    

    bool collision{}; 


    SetTargetFPS(60);   
    //while loop 
    while(!WindowShouldClose()){
        
        
        //delta time (time since last frame)
        const float dt{GetFrameTime()};

        //start drewaing 
        BeginDrawing();
        ClearBackground(WHITE);
        
        
        //scroll the background 
        bgX -= 20 * dt;
        if(bgX <= -background.width*2)
        {
            bgX =0.0;
        }
        //scroll the midground 
        mgX -= 40 * dt;
        if(mgX<= -midground.width*2)
        {
            mgX =0.0;
        }
        //scroll the foreground 
        fgX -=80*dt;
        if (fgX <=-foreground.width*2)
        {
            fgX =0.0;
        }

        //draw the background
        Vector2 bg1POS{bgX,0.0};
        DrawTextureEx(background , bg1POS, 0.0, 2.0, WHITE);
        Vector2 bg2POS{bgX+ background.width * 2, 0.0};
        DrawTextureEx(background, bg2POS, 0.0, 2.0, WHITE);
        //draw the midground 
        Vector2 mg1POS{mgX,0.0};
        DrawTextureEx(midground , mg1POS ,0.0, 2.0,WHITE);
        Vector2 mg2POS{mgX+ midground.width*2.0, 0.0};
        DrawTextureEx(midground,mg2POS,0.0, 2.0, WHITE);
        //Draw the foreground
        Vector2 fg1POS{fgX,0.0};
        DrawTextureEx(foreground , fg1POS ,0.0, 2.0,WHITE);
        Vector2 fg2POS{fgX + foreground.width*2.0, 0.0};
        DrawTextureEx(foreground,fg2POS,0.0, 2.0, WHITE);


        
        //ground check
        if (isOnGround(scarfyData,WindowDimensions[1]))
        {
            //recntanle is on the ground
            velocity =0;
            isInAir = false;
    
        }
        
        else{
            //rectnangle is in the air
            velocity += gravity*dt; 
            isInAir =true; 
        }   
        //check for jumping 
        if (IsKeyPressed(KEY_SPACE) && !isInAir  )
        {
            velocity+= jumpVel;
            
            
        }
        
        for (int i = 0; i <sizeofnebulae; i++)
        {
            //update the postion of each nebula
            nebulae[i].pos.x+= nebVel *dt;
       
        }
        // update finish line 
        finishline += nebVel *dt;

       
        
        //update SCARFypostion 
        scarfyData.pos.y += velocity*dt;

        //updates scarfy positon 
       if (!isInAir){
        scarfyData =updateAnimData(scarfyData, dt,5);
       }

       for (int i = 0; i <3; i++)
       {
        nebulae[i]=updateAnimData(nebulae[i],dt,7);
       }

       bool collision {false};
       for (AnimData nebula : nebulae)
       {
        float pad(20);
        Rectangle nebRec{
            nebula.pos.x +pad,
            nebula.pos.y +pad ,
            nebula.rec.width-2*pad,
            nebula.rec.height -2*pad
        };
        Rectangle scarfyRec{
            scarfyData.pos.x,
            scarfyData.pos.y,
            scarfyData.rec.width,
            scarfyData.rec.height
        
            };
            if (CheckCollisionRecs(nebRec,scarfyRec))
            {
                collision = true;
            }
       }

       if (collision )
       {
        //LOSE THE GAME 
        DrawText("Game Over!",WindowDimensions[0]/4, WindowDimensions[1]/2,40,RED);

       }
       else if(scarfyData.pos.x>= finishline)
    
       {
        //win the game 
         DrawText("YOU WIN! ",WindowDimensions[0]/4, WindowDimensions[1]/2,40,GREEN);
       }
        else 
        {
                
            for ( int i =0; i < sizeofnebulae; i++)
            {
                //draw nebula 
                DrawTextureRec(nebula,nebulae[i].rec, nebulae[i].pos, WHITE);
            }
                
                
                
                
                //draw scarfy
            DrawTextureRec(scarfy,scarfyData.rec, scarfyData.pos, WHITE);
        }
       
       
        //stop drawing
        
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}