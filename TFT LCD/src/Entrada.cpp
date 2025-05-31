#include "Entrada.h"
#include "Game.h"
#define BotonRight 18
#define BotonLeft 19
#define BotonUp 20
#define BotonDown 21

Entrada::Entrada(Game*gameRef){
   juego=gameRef;
    pinMode(BotonRight, INPUT);
    pinMode(BotonLeft, INPUT);
    pinMode(BotonDown, INPUT);
    pinMode(BotonUp, INPUT);
  
}

void Entrada::Controlar(void){
     if(digitalRead(BotonRight) == HIGH){
        juego->moverJugador(RIGHT); //derecha
         Serial.println("Boton derecho presionado");
        } else  if(digitalRead(BotonLeft) == HIGH){
        juego->moverJugador(LEFT); 
        Serial.println("Boton izquierdo presionado");
        }else if(digitalRead(BotonDown) == HIGH){
        juego->moverJugador(DOWN);
        Serial.println("Boton abajo presionado");
        }
        else if(digitalRead(BotonUp) == HIGH){
        juego->moverJugador(UP); 
        Serial.println("Boton arriba presionado");
        }
}


