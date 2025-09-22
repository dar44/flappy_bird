#include <MainGameState.hpp>
#include <iostream>
extern "C" {
    #include <raylib.h>
}

MainGameState::MainGameState()
{
}

void MainGameState::init()
{
    player.x = 200.0f;
    player.y = 200.0f;
    player.vy = 0.0f; 
    entered_key = 0;

}

void MainGameState::handleInput()
{
    if(IsKeyPressed(KEY_SPACE)) {
        player.vy += -300.f;
        entered_key = ' ';

    }

}

void MainGameState::update(float deltaTime)
{
    const float gravedad = 900.0f;
    player.vy += gravedad * deltaTime;
    player.y  += player.vy * deltaTime;
    player.vy = 0.0f;


}

void MainGameState::render()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Bienvenido a Flappy Bird DCA", 20, 20, 20, BLACK);//EJ 0
    
    DrawCircle((int)player.x, (int)player.y, 17.0f, RED);//EJ 1
    EndDrawing();
    

}