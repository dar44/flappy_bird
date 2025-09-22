// src/GameOverState.cpp
#include <GameOverState.hpp>
#include <MainGameState.hpp>
#include <memory>
#include <StateMachine.hpp> 
extern "C" {
    #include <raylib.h>
}
GameOverState::GameOverState(int finalScore) : finalScore(finalScore) {}

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

    std::string scoreText = "Puntuacion: " + std::to_string(finalScore);
    DrawText(scoreText.c_str(), 60, 260, 20, BLACK);


    EndDrawing();
}
