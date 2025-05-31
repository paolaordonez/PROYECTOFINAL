#include <Arduino.h>
#include "Game.h"
#include "Entrada.h"
void controlar(void);
Game game;
Entrada entrada(&game); 
void setup() {
    Serial.begin(9600);
    game.setup();
    attachInterrupt(digitalPinToInterrupt(BotonDown),controlar,RISING);
    attachInterrupt(digitalPinToInterrupt(BotonUp),controlar,RISING);
    attachInterrupt(digitalPinToInterrupt(BotonLeft),controlar,RISING);
    attachInterrupt(digitalPinToInterrupt(BotonRight),controlar,RISING);
}

void loop() {
    game.update();
    delay(10);  // Control básico de velocidad del juego
}
void controlar(void){
    entrada.Controlar();
}