#pragma once
#include <GameState.hpp>
#include <deque>
extern "C" {
    #include <raylib.h> 
}

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

        // EJ2
        struct PipePair { Rectangle top, bot; bool scored = false; };
        std::deque<PipePair> pipes;

        const float PIPE_SPEED = 110.0f;

        float spawnTimer;
        float spawnEvery;

        int PIPE_W;
        int PIPE_H;


        
};