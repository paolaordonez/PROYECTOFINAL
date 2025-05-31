#ifndef ENTRADA_H
#define ENTRADA_H
#include "Game.h"
#include <Arduino.h>
#define BotonRight 18
#define BotonLeft 19
#define BotonUp 20
#define BotonDown 21
class Game;
class Entrada{
private:
Game*juego; //Puntero a la clase Game
const uint8_t UP = 0;
const uint8_t DOWN = 1;
const uint8_t RIGHT = 2;
const uint8_t LEFT = 3;
public:
Entrada(Game*gameRef); //Constructor
void Controlar(void);
};
#endif // ENTRADA_H