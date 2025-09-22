// include/GameOverState.hpp
#pragma once
#include <GameState.hpp>

class GameOverState : public GameState
{
public:
    GameOverState() = default;
    ~GameOverState() = default;
    GameOverState(int finalScore);

    void init() override {}

    void handleInput() override;
    void update(float deltaTime) override {}
    void render() override;

    void pause() override {}
    void resume() override {}

private:
    int finalScore;
};
