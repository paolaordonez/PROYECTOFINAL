/*
    Tamaño de la pantalla 240x320 píxeles
*/
// Librerías necesarias
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "SpriteLava.h" 
#include "SpritePared.h"
#include "SpriteRecolectable.h"
#include "SpriteCorazon.h"
#include "SpriteP.h"
#include "pitches.h"
#include "SpriteStart.h"

// Pines del TFT LCD ILI9341
#define TFT_DC 7
#define TFT_CS 6
#define TFT_MOSI 11
#define TFT_CLK 13
#define TFT_RST 10
#define TFT_MISO 12
// Dimensiones del laberinto
#define MAZE_WIDTH 8
#define MAZE_HEIGHT 11
// Botones
#define botonRight 18
#define botonLeft 19
#define botonUp 20
#define botonDown 21
#define Buzzer 8
#define NumNiveles 3
// Definición de colores
#define WALL_COLOR ILI9341_PINK
#define PATH_COLOR ILI9341_BLACK
#define PLAYER_COLOR ILI9341_PURPLE
#define GOAL_COLOR ILI9341_NAVY
#define ITEM_COLOR ILI9341_MAGENTA
#define TRAP_COLOR ILI9341_RED
#define TIMER_COLOR ILI9341_WHITE

// Dimensiones de la pantalla
const int XMAX = 240; // Ancho del display
const int YMAX = 320; // Alto del display
const int intervaloantirebote = 10; // Tiempo en milisegundos para evitar rebotes

// Posición inicial del jugador
int playerX = 0; // Coordenada X en la matriz del laberinto
int playerY = 1; // Coordenada Y en la matriz del laberinto
int puntaje = 0; // Puntaje del jugador
int tiempolimite ;
int vidas=0;
int nivel =0; 
int totalitems = 0;
int itemsRecogidos = 0;
int prevPlayerX = 0;
int prevPlayerY = 1;
int tiempoRestante=0;

unsigned long TiempoInicio;
unsigned long UltimoMovimiento =0;
bool sonidoVictoriaReproducido = false;
bool sonidoDerrotaReproducido = false;

// Dirección del movimiento
const uint8_t UP = 0;
const uint8_t DOWN = 1;
const uint8_t RIGHT = 2;
const uint8_t LEFT = 3;

enum EstadoJuego {JUGANDO, GAME_OVER, GANAR, NIVEL_CUMPLIDO}; // Estados del juego
EstadoJuego estado_juego = JUGANDO;
// Laberinto
int maze1[MAZE_HEIGHT][MAZE_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {2, 2, 2, 5, 2, 2, 5, 1}, // Entrada en (1, 1)
    {1, 2, 2, 4, 1, 2, 1, 1},
    {1, 1, 1, 2, 5, 2, 1, 1},
    {1, 2, 1, 4, 4, 4, 2, 1},
    {1, 2, 4, 1, 2, 1, 4, 1},
    {1, 4, 1, 1, 1, 5, 2, 1},
    {1, 4, 4, 4, 4, 2, 2, 1},
    {1, 1, 1, 1, 4, 1, 4, 1},
    {1, 1, 1, 1, 1, 1, 3, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}    
};
int maze2[MAZE_HEIGHT][MAZE_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {2, 2, 2, 5, 4, 4, 5, 1}, // Entrada en (1, 1)
    {1, 5, 2, 4, 1, 4, 1, 1},
    {1, 2, 2, 2, 4, 2, 4, 1},
    {1, 1, 1, 4, 4, 5, 2, 1},
    {1, 4, 4, 1, 2, 1, 3, 1},
    {1, 5, 4, 2, 2, 5, 5, 1},
    {1, 4, 1, 1, 4, 2, 2, 1},
    {1, 2, 2, 2, 4, 2, 4, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}    
};
int maze3[MAZE_HEIGHT][MAZE_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {2, 2, 4, 2, 4, 5, 3, 1}, // Entrada en (1, 1)
    {1, 5, 4, 4, 2, 5, 2, 1},
    {1, 2, 5, 4, 2, 2, 2, 1},
    {1, 1, 4, 4, 1, 1, 5, 1},
    {1, 4, 2, 5, 2, 2, 4, 1},
    {1, 2, 4, 4, 2, 5, 2, 1},
    {1, 4, 5, 4, 4, 1, 2, 1},
    {1, 2, 2, 1, 4, 1, 4, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}    
};
int* mazes[NumNiveles] = {(int *)maze1, (int*)maze2, (int*)maze3};  // Array de laberintos
int maze[MAZE_HEIGHT][MAZE_WIDTH]; // Laberinto actual

// Objeto para manejar la pantalla TFT
Adafruit_ILI9341 screen = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
// Declaración de funciones
void dibujarLaberinto();
void dibujarJugador();
void moverPlayer(uint8_t direccion);
void moverPlayerDerecha();
void moverPlayerIzquierda();
void moverPlayerArriba();
void moverPlayerAbajo();
void ActualizarTiempo();
bool esMovimientoValido(int x, int y);
void CargarNivel(int nivel);
void DibujarVidas();
void DibujarPuntuacion();
void MostrarGameOver();
void mostrarVictoria();
void sonidoDerrota();
void SonidoVictoria();
void MostrarPresentacion();
void limpiarCelda(int x, int y);
// Configuración inicial
void setup() {
    Serial.begin(9600);
    pinMode(Buzzer, OUTPUT);
     pinMode(botonRight, INPUT);
    pinMode(botonLeft, INPUT);
    pinMode(botonUp, INPUT);
    pinMode(botonDown, INPUT);
    Serial.println("Inicializando...");
       // Se agregan interrupciones externas
    attachInterrupt(digitalPinToInterrupt(botonRight), moverPlayerDerecha, HIGH);
    attachInterrupt(digitalPinToInterrupt(botonLeft), moverPlayerIzquierda, HIGH);
    attachInterrupt(digitalPinToInterrupt(botonUp), moverPlayerArriba, HIGH);
    attachInterrupt(digitalPinToInterrupt(botonDown), moverPlayerAbajo, HIGH);
    // Inicialización de la pantalla
    screen.begin(); 
    //SonidoVictoria();
    MostrarPresentacion();
    CargarNivel(0); // Cargar el primer nivel
    estado_juego = JUGANDO; // Estado inicial del juego
}
// Bucle principal
void loop() {
    ActualizarTiempo();
    if (estado_juego==JUGANDO){
       // ActualizarTiempo();
        //dibujarJugador();
    } else if(estado_juego== NIVEL_CUMPLIDO){
        delay(10);
        CargarNivel(nivel);
        estado_juego=JUGANDO;
    }else if(estado_juego==GANAR){
        mostrarVictoria();
        delay(10);
    }
    else if(estado_juego==GAME_OVER){
        MostrarGameOver();
        delay(10);
    }
    delay(100);
}
    // Convierte RGB de 8 bits a RGB565 (16 bits)
uint16_t convertirRGB(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
void MostrarPresentacion(){
    uint16_t violetaFondo = convertirRGB(154, 136, 231);
    screen.fillScreen(violetaFondo);  // Pinta toda la pantalla con ese color
    screen.setTextColor(ILI9341_CYAN);
    screen.setTextSize(3);
    screen.setCursor(25,50);
    screen.println("Presiona el");
    screen.setCursor(45,100);
    screen.println("boton LEFT");
    screen.drawRGBBitmap(90,150, Start[0],50, 50);
    screen.drawRGBBitmap(130,220, Nube[0],32, 32);
    screen.drawRGBBitmap(80,260, Nube[0],32, 32);
    screen.drawRGBBitmap(200,130, Nube[0],32, 32);
    screen.drawRGBBitmap(15,180, Nube[0],32, 32);
    screen.drawRGBBitmap(200,280, Nube[0],32, 32);
    screen.drawRGBBitmap(2,7, Nube[0],32, 32);
    while (
           digitalRead(botonLeft)==LOW  ){
            delay(10);
           }
           delay(100);
}

void CargarNivel(int nivel) {
    // Cargar el laberinto correspondiente al nivel
    memcpy(maze, mazes[nivel],sizeof(maze));
    // Actualizar la posición inicial del jugador
    playerX = 0;
    playerY = 1;
    prevPlayerX = playerX;
    prevPlayerY = playerY;
    puntaje=0;
    vidas =3;
    tiempolimite=40 - nivel*5; // Tiempo límite para cada nivel
    if (tiempolimite < 10 ) tiempolimite=10; // Limitar tiempo a 10 segundos
    totalitems = 0;
    itemsRecogidos = 0;
    for (int i=0;i< MAZE_HEIGHT; i++){
        for (int j = 0; j< MAZE_WIDTH; j++){
            if (maze[i][j] == 4) totalitems++;         
        }
    }
    screen.fillScreen(ILI9341_PINK); 
    dibujarLaberinto();
    dibujarJugador();
    ActualizarTiempo(); 
    DibujarVidas();
    DibujarPuntuacion();
    TiempoInicio=millis();
    
}
// Dibuja el laberinto en la pantalla
void dibujarLaberinto() {
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
    
        if (maze[i][j] == 3) {
            screen.drawRGBBitmap(j*32,i*32, Meta[0], 32, 32);
        }  
        if (maze[i][j] == 2) {
             screen.fillRect(j * 32, i * 32, 32, 32, PATH_COLOR);
        } 
         if (maze[i][j] == 4) {
             screen.drawRGBBitmap(j*32,i*32, Cereza[0], 32, 32);
        }  
         if (maze[i][j] == 5) {
           screen.drawRGBBitmap(j*32,i*32, Fuego[0], 32, 32);
        }  
        if (maze[i][j] == 1) {
           screen.drawRGBBitmap(j*32,i*32, Pared[0], 32, 32);
        }  
    }}}
void limpiarCelda(int x, int y) {
    switch(maze[y][x]){
        case 1: //pared
        screen.drawRGBBitmap(x*32,y*32, Pared[0], 32,32);
        break;
        case 2: //camino
        screen.fillRect(x*32,y*32, 32, 32, PATH_COLOR);
        break;
        case 3: //meta
        screen.drawRGBBitmap(x*32,y*32,Meta[0], 32, 32);
        break;
        case 4: //Recolectable
        screen.drawRGBBitmap(x*32,y*32, Cereza[0], 32,32);
        break;
        case 5: //Trampa
        screen.drawRGBBitmap(x*32,y*32, Fuego[0], 32,32);
        break;;
    }
}
// Dibuja al jugador en la pantalla
void dibujarJugador() {
 // Usa el sprite definido en Sprite.h
 if(prevPlayerX!=playerX|| prevPlayerY!=playerY){
    limpiarCelda(prevPlayerX, prevPlayerY);
 }
 screen.drawRGBBitmap(playerX * 32, playerY * 32, Player1[0], 32, 32); // Sprite estático
}
// Mueve al jugador en la dirección dada
void moverPlayer(uint8_t direccion) {
    uint8_t delta = 1; // Cantidad de movimiento en cada dirección

    int newX = playerX;
    int newY = playerY;

    switch (direccion)
    {
    case 0:
        newY = newY - delta;
        break;
    case 1:
      newY = newY+ delta;
        break;
    case 2:
        newX=newX+ delta;
        break;
    case 3:
       newX = newX - delta;
        break;
    }
    // Verifica si el movimiento es válido antes de actualizar la posición
    if (esMovimientoValido(newX, newY)) {

        prevPlayerX = playerX;
        prevPlayerY = playerY;

        if (maze[newY][newX] == 4) {
             itemsRecogidos=itemsRecogidos+1; 
             DibujarPuntuacion();
             maze[newY][newX]=2; // Se quita el item
        }
        playerX = newX;
        playerY = newY;
        dibujarJugador();
        // if (maze[playerX][playerY] == 4) {
           // screen.drawRGBBitmap(playerX*32,playerY*32, Cereza[0], 32, 32);            
       // }
        // Actualiza las coordenadas del jugador
         playerX = newX;
         playerY = newY;
        // Dibuja al jugador en la nueva posición
         dibujarJugador();
         if (maze[newY][newX] == 5) {
         vidas=vidas-1;
         DibujarVidas();
         if (vidas > 0) {
            delay(10);
            playerX = 0;
            playerY = 1;
            dibujarLaberinto();
            dibujarJugador();
            //ActualizarTiempo();
            DibujarPuntuacion();
            DibujarVidas();
        }else  {
            estado_juego=GAME_OVER;      
        } }
        if (maze[playerY][playerX] == 3) {
            if(itemsRecogidos<totalitems){
                estado_juego=GAME_OVER;
                return;
            }
            nivel= nivel + 1;
            if (nivel < NumNiveles){
            screen.fillScreen(WALL_COLOR);
            screen.setTextColor(ILI9341_BLACK);
            screen.setTextSize(2);
            screen.setCursor(20, 140);
            screen.println("¡SIGUIENTE NIVEL!");
            estado_juego = NIVEL_CUMPLIDO;         
           }else{
           estado_juego= GANAR;         
    }
}
}}
// Verifica si el movimiento es válido
bool esMovimientoValido(int x, int y) {
    return (x >= 0 && x < MAZE_WIDTH && y >= 0 && y < MAZE_HEIGHT && maze[y][x] != 1);
}
void DibujarVidas(){
    screen.fillRect(220, 0, 75, 20, ILI9341_BLACK);
    for (int i = 0; i <3; i++){
        if (i<vidas){
        screen.drawRGBBitmap(177+i*20,0, Corazon[0], 20, 20); 
        }else{
        }
    }
}
void ActualizarTiempo(){
    //if(estado_juego!=JUGANDO) return;
    unsigned long tiempoActual = millis();
    int tiempoJuego = (tiempoActual - TiempoInicio)/1000;
    int tiempoRestante = tiempolimite- tiempoJuego;
          screen.fillRect(0, 0, 62, 20, ILI9341_BLACK); 
          screen.setCursor(0,0);
          screen.setTextColor(ILI9341_WHITE);
          screen.setTextSize(2);
          screen.drawRGBBitmap(6,0, Tiempo1[0], 20, 20);
          screen.setCursor(25,0);
          screen.print(":");
          screen.print(tiempoRestante);
     if(tiempoRestante <= 0 ){
        estado_juego=GAME_OVER;
     }
    }

void DibujarPuntuacion(){
     screen.fillRect(120, 0,35, 20, ILI9341_BLACK); 
     screen.setCursor(0, 16);
     screen.setTextColor(ILI9341_WHITE);
     screen.setTextSize(2);
     screen.drawRGBBitmap(100,0, Cereza2[0], 20, 20);
     screen.setCursor(120,0);
     screen.print(":");
     screen.print(itemsRecogidos);   
}
void MostrarGameOver(){ 
    //sonidoDerrota();
     if (!sonidoDerrotaReproducido){
            sonidoDerrota();
            sonidoDerrotaReproducido=true;
        }
    screen.fillScreen(TRAP_COLOR);
    screen.setTextColor(ILI9341_WHITE);
    screen.setTextSize(2);
    screen.setCursor(70, 130);
    screen.println("GAME OVER");
    screen.setCursor(60, 150);
    screen.println("¡Perdiste!");
    return;
}
void mostrarVictoria(){ 
     if (!sonidoVictoriaReproducido){
            SonidoVictoria();
            sonidoVictoriaReproducido=true;
        }
    screen.fillScreen(PLAYER_COLOR);
    screen.setTextColor(ILI9341_WHITE);
    screen.setTextSize(2);
    screen.setCursor(30, 130);
    screen.println("¡FELICIDADES!");
    screen.setCursor(25, 160);
    screen.println("¡GANASTE TODOS");
    screen.setCursor(28, 190);
    screen.println("LOS NIVELES!");
    return;  
}
void SonidoVictoria(){
    // Melodía alegre: C5 - E5 - G5 - C6
    tone(Buzzer, NOTE_C5);
    delay(160);
    tone(Buzzer, NOTE_E5);
    delay(140);
    tone(Buzzer, NOTE_G5);
    delay(160);
    tone(Buzzer, NOTE_E5);
    delay(130);
    tone(Buzzer, NOTE_G5);
    delay(180);
    tone(Buzzer, NOTE_C6);
    delay(230);
    tone(Buzzer, NOTE_E6);
    delay(350);
    tone(Buzzer, NOTE_C6);
    delay(350);
    noTone(Buzzer);
}

void sonidoDerrota(){
    tone(Buzzer, NOTE_DS5);  
    delay(300);
    tone(Buzzer, NOTE_D5); 
    delay(300);
    tone(Buzzer, NOTE_CS5); 
    delay(300);
    noTone(Buzzer);
     // Efecto wah wah descendente
  for (byte i = 0; i < 10; i++) {
    for (int pitch = -10; pitch <= 10; pitch++) {
      tone(Buzzer, NOTE_C5 + pitch);
      delay(5);
    }
  }
  noTone(Buzzer);
  delay(500);
}
// Función para mover al jugador hacia la derecha
void moverPlayerDerecha() {
     if(estado_juego!=JUGANDO) return;
     if (millis()-UltimoMovimiento> intervaloantirebote){
     if (digitalRead(botonRight) == HIGH) {
        Serial.println("Botón RIGHT presionado");
        moverPlayer(RIGHT);
    }
}}
// Función para mover al jugador hacia la izquierda
void moverPlayerIzquierda() {
     if(estado_juego!=JUGANDO) return;
     if(millis()-UltimoMovimiento> intervaloantirebote){
     if (digitalRead(botonLeft) == HIGH) {
        Serial.println("Botón LEFT presionado");
        moverPlayer(LEFT);
   }
}}
// Función para mover al jugador hacia arriba
void moverPlayerArriba() {
     if(estado_juego!=JUGANDO) return;
     if(millis()- UltimoMovimiento> intervaloantirebote){
     if (digitalRead(botonUp) == HIGH) {
        Serial.println("Botón UP presionado");
        moverPlayer(UP);
    }
}}
// Función para mover al jugador hacia abajo
void moverPlayerAbajo() {
     if(estado_juego!=JUGANDO) return;
     if(millis()- UltimoMovimiento> intervaloantirebote){
     if (digitalRead(botonDown) == HIGH) {
        Serial.println("Botón DOWN presionado");
        moverPlayer(DOWN);
   }
}}

