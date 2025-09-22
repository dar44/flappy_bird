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

        void pause() override{};
        void resume() override{};

    
    private:
        struct Bird {
            float x;
            float y;
            float vy;
            //ej5
            float width;
            float height;
        };
        Bird player;
        char entered_key;

        // EJ2
        struct PipePair { Rectangle top, bot; bool scored = false; };
        std::deque<PipePair> pipes;

        float PIPE_SPEED; //EJ5 lo cambio a variable por enunciado

        float spawnTimer;
        float spawnEvery;
        int PIPE_W;
        int PIPE_H;
        float pipeGap; //EJ5
    

        //EJ 4
        int score;
        
        //EJ 5 SPRTES
        Texture2D birdSprite;
        Texture2D pipeSprite;

        //extras
        Sound     sndFlap;
        Sound     sndPoint;
        Sound     sndHit;
        Texture2D digitTex[10];
};