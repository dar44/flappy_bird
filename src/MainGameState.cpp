#include <MainGameState.hpp>
#include <iostream>
extern "C" {
    #include <raylib.h>
}
#include <StateMachine.hpp> 
#include <GameOverState.hpp>
#include <string> //para digitos

MainGameState::MainGameState()
{
    //1
    player = {200.0f, 200.0f };
    entered_key = 0;

    //EJ 2
    PIPE_W = 32;
    PIPE_H = 320;
    PIPE_SPEED = 110.0f; //EJ5 lo cambio a variable por enunciado
    spawnTimer = 0.0f;
    spawnEvery = 0.8f; //ARBITRARIO

    score = 0;

    //EJ5
    pipeGap = 0.0f;//en init() lo inicializo

}

void MainGameState::init()
{
    player.x = 200.0f;
    player.y = 200.0f;
    player.vy = 0.0f; 
    entered_key = 0;

    // Ej5 cargo sprites
    birdSprite = LoadTexture("assets/yellowbird-midflap.png");
    pipeSprite = LoadTexture("assets/pipe-green.png");

    player.width  = static_cast<float>(birdSprite.width);
    player.height = static_cast<float>(birdSprite.height);

    PIPE_W = pipeSprite.width;
    PIPE_H = pipeSprite.height;

    pipeGap = player.height * 4.5f; 

    // Extras: sonidos (ajusta rutas según tus ficheros en assets/)
    sndFlap  = LoadSound("assets/wing.wav");
    sndPoint = LoadSound("assets/point.wav");
    sndHit   = LoadSound("assets/hit.wav");

    // digitos como sprites
    for (int d = 0; d < 10; ++d) {
        std::string path = "assets/" + std::to_string(d) + ".png";
        digitTex[d] = LoadTexture(path.c_str());
    }
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
        PlaySound(sndFlap);
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

        int offsetTop = GetRandomValue(PIPE_H / 2, GetScreenHeight() / 2);
        float startX = static_cast<float>(GetScreenWidth());

        Rectangle topPipe =   {
            startX,
            -static_cast<float>(offsetTop),
            static_cast<float>(PIPE_W),
            static_cast<float>(PIPE_H)
        };

        Rectangle botPipe = {
            startX,
            topPipe.y + static_cast<float>(PIPE_H) + pipeGap, //cambio ej5
            static_cast<float>(PIPE_W),
            static_cast<float>(PIPE_H)
        };

        //nuevo obj en cola
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

    // Ej5 centro en x,y y bounding box segunn el sprite
    Rectangle playerBB = {
        player.x - player.width  / 2.0f,
        player.y - player.height / 2.0f,
        player.width,
        player.height
    };

    /* Ej.3: colisiones y límites
    const float BIRD_RADIUS = 17.0f;
    Rectangle playerBB = {
        player.x - BIRD_RADIUS,
        player.y - BIRD_RADIUS,
        BIRD_RADIUS,
        BIRD_RADIUS
    };
    */

    if(player.x - player.width /2.0f < 0.0f || player.x + player.width /2.0f > GetScreenWidth() ||
        player.y - player.height/2.0f < 0.0f ||player.y + player.height/2.0f > GetScreenHeight()) {
        //golpe
        PlaySound(sndHit);
        this->state_machine->add_state(std::make_unique<GameOverState>(score), true);
        return;
    }

    // Choque con cualquier tubería 
    for(size_t i = 0; i < pipes.size(); i++) {
        if(CheckCollisionRecs(playerBB, pipes[i].top) ||
            CheckCollisionRecs(playerBB, pipes[i].bot)) {
            //golpe
            PlaySound(sndHit);
            this->state_machine->add_state(std::make_unique<GameOverState>(score), true);
            return;
        }
    

        // ej4 puntuacion
        if(!pipes[i].scored && (pipes[i].top.x + PIPE_W) < player.x) {
            score++;
            pipes[i].scored = true;

            PlaySound(sndPoint);
        }

    }
}

void MainGameState::render()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    //ej0
    DrawText("Flappy Bird DCA", 20, 20, 20, BLACK);//EJ 0
    
    // Ej5: sprite del pájaro (centrado en x,y)
    DrawTexture(birdSprite, static_cast<int>(player.x - player.width  / 2.0f), static_cast<int>(player.y - player.height / 2.0f), WHITE);

    // EJ 3
    //DrawCircle((int)player.x, (int)player.y, 17.0f, RED);//EJ 1
    
    /* Ej2
    for (size_t i = 0; i < pipes.size(); i++) {
        DrawRectangleRec(pipes[i].top, GREEN);
        DrawRectangleRec(pipes[i].bot, GREEN);
    }
    */
    // Ej5: sprites de las tuberías
    for(size_t i = 0; i < pipes.size(); i++) {
        DrawTextureEx(this->pipeSprite, {pipes[i].top.x + PIPE_W, pipes[i].top.y + PIPE_H}, 180.f, 1.0f, WHITE);
        DrawTextureEx(this->pipeSprite, {pipes[i].bot.x , pipes[i].bot.y}, 0.f, 1.0f, WHITE);
    }

    /* Ej4
    DrawText(std::to_string(score).c_str(), 250, 20, 20, BLACK);
    */
    std::string s = std::to_string(score);
    int x = 250, y = 20;
    for (size_t i = 0; i < s.size(); i++) {
    int d = s[i] - '0';
    DrawTexture(digitTex[d], x, y, WHITE);
    x += digitTex[d].width;
}
    EndDrawing();
    

}