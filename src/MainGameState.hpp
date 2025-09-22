#pragma once
#include <GameState.hpp>

class MainGameState : public GameState
{
    public:
        MainGameState();
        ~MainGameState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause(){};
        void resume(){};

    
    private:
        struct Bird {
            float x;
            float y;
            float vy;
        };
        Bird player;
        char entered_key;
        
};