# Flappy Bird DCA — README

## Resumen

Implementación paso a paso (acumulativa) de un Flappy Bird con raylib, siguiendo los ejercicios 0–5:

- **Ej0**: render básico (texto).  
- **Ej1**: pájaro con gravedad y salto.  
- **Ej2**: tuberías en cola (deque) que aparecen periódicamente y se desplazan.  
- **Ej3**: colisiones AABB y estado de Game Over.  
- **Ej4**: puntuación al superar tuberías y render en pantalla.  
- **Ej5**: sprites para pájaro y tuberías.  

**Extras**: sonidos del juego y puntuación renderizada con sprites (dígitos).  

**Nota de estilo**: en los bucles hemos usado `size_t i` en lugar de `auto`, tal y como pide el profesor.

---

## Controles

- **Barra espaciadora**: salto.

---

## Compilación y ejecución

Asegúrate de tener raylib y los assets en su carpeta.

```bash
g++ -o game src/*.cpp -I src -I vendor/include -L vendor/lib \
    -lraylib -lGL -lm -lpthread -lrt -lX11
./game
```

**Importante**: en `main()` se llama primero a `InitWindow(...)` y, si usas audio, a `InitAudioDevice()` antes de que los estados carguen texturas/sonidos en `init();` de lo contrario se producirán errores/segfault.

---

## Estructura (alto nivel)

### MainGameState

- **Bird**: `{ x, y, vy, width, height }`  
- `std::deque<PipePair>` con `PipePair { Rectangle top, bot, bool scored }`  
- **Spawning** de tuberías con `spawnTimer/spawnEvery`  
- **Movimiento**: `x -= PIPE_SPEED * deltaTime`  
- **Colisiones**: `CheckCollisionRecs(...)` con bounding box del pájaro centrado en `(x, y)`  
- **Render**: `DrawTexture` (pájaro) y `DrawTextureEx` (tuberías)  
- **Puntuación**: `+1` cuando `(pipe.top.x + PIPE_W) < player.x`  

### GameOverState

- Muestra “Game Over” y la puntuación final.  
- Pulsando **Espacio** reinicia (reemplaza por un `MainGameState` nuevo).

---

## Assets

Coloca los sprites y audios así (mínimo):

```
assets/
    yellowbird-midflap.png
    pipe-green.png
    0.png
    1.png
    2.png
    3.png
    4.png
    5.png
    6.png
    7.png
    8.png
    9.png
    audio/
        wing.wav
        point.wav
        hit.wav
```

- **Carga de sprites**: `LoadTexture("assets/...")`  
- **Audio (extra)**: `InitAudioDevice()`, `LoadSound("assets/audio/...")`, `PlaySound(...)`

---

## Extras implementados

- **Sonidos**: aleteo (`wing.wav`), punto (`point.wav`) y choque (`hit.wav`).  
- **Puntuación con sprites**: en lugar de `DrawText`, se dibujan los dígitos `0.png–9.png` concatenando sus anchos.

---

## Notas de implementación

- **Delta Time** con `GetFrameTime()` para independencia de FPS.  
- **Colisiones AABB** usando `Rectangle` + `CheckCollisionRecs`.  
- **Tuberías** en `std::deque`, eliminando con `pop_front()` cuando salen de pantalla.  
- **Sprites**: el bounding box del pájaro depende de `width/height` del sprite (centrado en `x, y`).  
- **Transición a Game Over**: `state_machine->add_state(std::make_unique<GameOverState>(score), true);`

---

## Ajuste del salto (recomendación)

Aunque el enunciado sugiere `-300.0f` para el impulso, con gravedad ≈900 y sprites a escala de píxeles suele resultar corto. En práctica, funciona mejor probar entre `-1000.0f` y `-1400.0f` para una sensación de salto más natural.

---

## Créditos

- **raylib** — [https://www.raylib.com](https://www.raylib.com)  
- **Sprites**: repositorio de samuelcust (carpeta sprites).  
- **Antonio**: [[text](https://antoniorv6.github.io/dca-gii/practicas/p0/)]Práctica 0: Desarrollo de Flapy Bird con raylib