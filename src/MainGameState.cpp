#include <MainGameState.hpp>
#include <iostream>
extern "C" {
    #include <raylib.h>
}
#include <StateMachine.hpp> 
#include <GameOverState.hpp>

MainGameState::MainGameState()
{
    //1
    player = {200.0f, 200.0f, 0.0f};
    entered_key = 0;

    //EJ 2
    PIPE_W = 32;
    PIPE_H = 320;
    spawnTimer = 0.0f;
    spawnEvery = 0.8f; //ARBITRARIO

    score = 0;

}

void MainGameState::init()
{
    player.x = 200.0f;
    player.y = 200.0f;
    player.vy = 0.0f; 
    entered_key = 0;

    //EJ2
    pipes.clear();
    spawnTimer = 0.0f;

    score = 0;

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
    //fisica del pajaro ej 1
    const float gravedad = 900.0f;
    player.vy += gravedad * deltaTime;
    player.y  += player.vy * deltaTime;
    player.vy = 0.0f;

    //tuberias ej 2
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnEvery) {
        spawnTimer = 0.0f;

        int pipe_y_offset_top = GetRandomValue(PIPE_H / 2, GetScreenHeight() / 2);

        float startX = static_cast<float>(GetScreenWidth());

        Rectangle topPipe =   {
            startX,
            -static_cast<float>(pipe_y_offset_top),
            static_cast<float>(PIPE_W),
            static_cast<float>(PIPE_H)
        };

        int valorRandom = GetRandomValue(PIPE_H / 2, GetScreenHeight() / 2);
        Rectangle botPipe = {
            startX,
            static_cast<float>((PIPE_H - pipe_y_offset_top) + valorRandom),
            static_cast<float>(PIPE_W),
            static_cast<float>(PIPE_H)
        };

        //Inserta tu nuevo objeto en la cola.
        pipes.push_back(PipePair{topPipe, botPipe, false});
    }

    //mover tuberias en cola
    for(size_t i = 0; i < pipes.size(); i++) {
        pipes[i].top.x -= PIPE_SPEED * deltaTime;
        pipes[i].bot.x -= PIPE_SPEED * deltaTime;
    }

    //eliminar si está fuera de los limites
    while(!pipes.empty() && (pipes.front().top.x + PIPE_W) < 0.0f) 
        pipes.pop_front();

    // Ej.3: colisiones y límites
    const float BIRD_RADIUS = 17.0f;
    Rectangle playerBB = {
        player.x - BIRD_RADIUS,
        player.y - BIRD_RADIUS,
        BIRD_RADIUS,
        BIRD_RADIUS
    };

    // Salirse de la pantalla
    if(player.x - BIRD_RADIUS < 0.0f || player.x + BIRD_RADIUS > GetScreenWidth() ||
        player.y - BIRD_RADIUS < 0.0f || player.y + BIRD_RADIUS > GetScreenHeight()) {
        this->state_machine->add_state(std::make_unique<GameOverState>(score), true);
        return;
    }

    // Choque con cualquier tubería 
    for(size_t i = 0; i < pipes.size(); i++) {
        if (CheckCollisionRecs(playerBB, pipes[i].top) ||
            CheckCollisionRecs(playerBB, pipes[i].bot)) {
            this->state_machine->add_state(std::make_unique<GameOverState>(score), true);
            return;
        }
    

        // ej4 puntuacion
        if(!pipes[i].scored && (pipes[i].top.x + PIPE_W) < player.x) {
            score++;
            pipes[i].scored = true;
        }

    }
}

void MainGameState::render()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    //ej0
    DrawText("Flappy Bird DCA", 20, 20, 20, BLACK);//EJ 0
    
    
    DrawCircle((int)player.x, (int)player.y, 17.0f, RED);//EJ 1
    
    // Ej2
    for (size_t i = 0; i < pipes.size(); i++) {
        DrawRectangleRec(pipes[i].top, GREEN);
        DrawRectangleRec(pipes[i].bot, GREEN);
    }

    // Ej4
    DrawText(std::to_string(score).c_str(), 250, 20, 20, BLACK);

    EndDrawing();
    

}