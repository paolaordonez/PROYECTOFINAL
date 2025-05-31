#ifndef GAME_H
#define GAME_H
#include <Adafruit_ILI9341.h>
#include "Maze.h"
#include "Player.h"
#include "SoundManager.h"

enum EstadoJuego { JUGANDO, GAME_OVER, GANAR, NIVEL_CUMPLIDO };
class Entrada;
class Game {
public:
    Game();
    void setup();
    void update();
    void draw();
    void moverJugador(uint8_t direccion);

private:
    Entrada*entrada;
    void dibujarHUD();
    void cargarNivel(int nivel);
    void verificarColisiones();
    void actualizarTiempo();
    void mostrarPantallaInicio();
    void mostrarPantallaGameOver();
    void mostrarPantallaNivelCumplido();

    Adafruit_ILI9341 screen;
    Maze maze;
    Player player;
    SoundManager sonido;

    EstadoJuego estado;
    int nivel;
    int vidas;
    int puntaje;
    int totalItems;
    int itemsRecogidos;
    int tiempoLimite;
    unsigned long tiempoInicio;
    bool sonidoDerrotaReproducido= false;
    bool sonidoVictoriaReproducido= false;
};

#endif
