// src/GameOverState.cpp
#include <GameOverState.hpp>
#include <MainGameState.hpp>
#include <memory>
#include <StateMachine.hpp> 
extern "C" {
    #include <raylib.h>
}

void GameOverState::handleInput()
{
    if (IsKeyPressed(KEY_SPACE)) {
        this->state_machine->add_state(std::make_unique<MainGameState>(), true);
    }
}

void GameOverState::render()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Game Over", 80, 200, 32, BLACK);
    EndDrawing();
}
