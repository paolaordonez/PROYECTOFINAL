#include "Game.h"
#include "Entrada.h"
#include "SpriteCorazon.h"
#include "SpriteRecolectable.h"
#include "SpriteStart.h"
#include "SpritePared.h"
//#include "SpriteTiempo.h"
//#include "SpriteMeta.h"
#define TFT_DC 7
#define TFT_CS 6
#define TFT_MOSI 11
#define TFT_CLK 13
#define TFT_RST 10
#define TFT_MISO 12
uint16_t convertirRGB(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

Game::Game()
    : screen(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO),
      estado(JUGANDO), nivel(0), vidas(3), puntaje(0),
      totalItems(0), itemsRecogidos(0), tiempoLimite(30), tiempoInicio(0) {}

void Game::mostrarPantallaInicio(){
    uint16_t VioletaFondo=convertirRGB(154,136,231);
    screen.begin();
    screen.setRotation(0); 
    screen.fillScreen(VioletaFondo);
    screen.setTextColor(ILI9341_CYAN);
    screen.setTextSize(3);
    screen.setCursor(25, 50);
    screen.println("Presiona el ");
    screen.setCursor(25, 100);
    screen.println("Boton LEFT");
    screen.drawRGBBitmap(90,150, Start[0], 50, 50);
    screen.drawRGBBitmap(130,220, Nube[0], 32, 32);
    screen.drawRGBBitmap(80,260, Nube[0], 32, 32);
    screen.drawRGBBitmap(200,130, Nube[0], 32, 32);
    screen.drawRGBBitmap(15,180, Nube[0], 32, 32);
    screen.drawRGBBitmap(200,280, Nube[0], 32, 32);
    screen.drawRGBBitmap(2,7, Nube[0], 32, 32);
    while(digitalRead(BotonLeft)==LOW){
        delay(10);
    }   
}
void Game::mostrarPantallaGameOver(){
    screen.fillScreen(ILI9341_RED);
    screen.setTextColor(ILI9341_WHITE);
    screen.setTextSize(2);
    screen.setCursor(70, 130);
    screen.println("GAME OVER");
    screen.setCursor(60, 150);
    screen.println("!Perdiste¡ ");
}
void Game::mostrarPantallaNivelCumplido(){
    screen.fillScreen(ILI9341_PINK);
    screen.setTextColor(ILI9341_WHITE);
    screen.setTextSize(2);
    screen.setCursor(30, 130);
    screen.println("!FELICIDADES");
    screen.setCursor(25, 160);
    screen.println("¡GANASTE TODOS!");
    screen.setCursor(28, 190);
    screen.println("¡LOS NIVELES!");
}
void Game::setup() {
    entrada = new Entrada(this);
    mostrarPantallaInicio();
    screen.begin();
    pinMode(8, OUTPUT);
    screen.fillScreen(ILI9341_PINK);
    cargarNivel(0);
}

void Game::update() {
   // entrada->leerEntrada();
    actualizarTiempo();
    if (estado == JUGANDO) {
        draw();
        //actualizarTiempo();
        verificarColisiones();
    } else if (estado == NIVEL_CUMPLIDO) {
        delay(10);
        cargarNivel(nivel);
        estado = JUGANDO;
    }
}

void Game::draw() {
    player.draw(screen,maze);
    dibujarHUD();
}

void Game::moverJugador(uint8_t direccion) {
    if (estado != JUGANDO) return;
    int dx = 0, dy = 0;
    switch (direccion) {
        case 0: dy = -1; break;
        case 1: dy = 1; break;
        case 2: dx = 1; break;
        case 3: dx = -1; break;
    }
    int newX = player.getX() + dx;
    int newY = player.getY() + dy;
    if (maze.isMoveValid(newX, newY)) {
        player.mover(dx, dy);
    }
}

void Game::dibujarHUD() {
    screen.fillRect(0, 0, 240, 20, ILI9341_BLACK);
    for (int i = 0; i < 3; i++) {
        if (i < vidas) {
            screen.drawRGBBitmap(177 + i * 20, 0, Corazon[0], 20, 20);
        }
    }
    screen.drawRGBBitmap(100, 0, Cereza2[0], 20, 20);
    screen.setCursor(120, 0);
    screen.setTextColor(ILI9341_WHITE);
    screen.setTextSize(2);
    screen.print(":");
    screen.print(itemsRecogidos);
    screen.drawRGBBitmap(6, 0, Tiempo1[0], 20, 20);
    screen.setCursor(25, 0);
    screen.print(":");
    screen.print(tiempoLimite - (millis() - tiempoInicio) / 1000);
}

void Game::cargarNivel(int nivelActual) {
    maze.loadLevel(nivelActual);
    maze.draw(screen);
    player.reset();
    player.draw(screen,maze);
    vidas = 3;
    itemsRecogidos = 0;
    totalItems = maze.countItems(4);
    tiempoLimite = max(10, 40 - nivelActual * 5);
    tiempoInicio = millis();
}

void Game::verificarColisiones() {
    int x = player.getX();
    int y = player.getY();
    int celda = maze.getTile(x, y);
    if (celda == 4) {
        itemsRecogidos++;
        puntaje += 10;
        maze.setTile(x, y, 2);
        maze.clearTile(x, y, screen);
    } else if (celda == 5) {
        vidas--;
        if (vidas <= 0) {
            estado = GAME_OVER;
            if(!sonidoDerrotaReproducido){
                sonido.reproducirDerrota();
                sonidoDerrotaReproducido = true;
            }
         mostrarPantallaGameOver();
        }
        player.reset();
        player.draw(screen,maze);
    } else if (celda == 3) {
        if (itemsRecogidos < totalItems) {
            estado = GAME_OVER;
             if(!sonidoDerrotaReproducido){
                sonido.reproducirDerrota();
                sonidoDerrotaReproducido = true;
            }
            mostrarPantallaGameOver();
        } else {
            nivel++;
            if (nivel >= 3) {
                estado = GANAR;
            if(!sonidoVictoriaReproducido){
                 sonido.reproducirVictoria();
                sonidoVictoriaReproducido = true;
                }
             mostrarPantallaNivelCumplido();
                
            } else {
                estado = NIVEL_CUMPLIDO;
                screen.fillScreen(ILI9341_PINK);
                screen.setTextColor(ILI9341_WHITE);
                screen.setTextSize(2);
                screen.setCursor(20, 140);
                screen.println("SIGUIENTE NIVEL");
            }
        }
    }
}

void Game::actualizarTiempo() {
    unsigned long tiempoActual = millis();
    if ((tiempoActual - tiempoInicio) / 1000 >= tiempoLimite) {
        estado = GAME_OVER;
         if(!sonidoDerrotaReproducido){
                sonido.reproducirDerrota();
                sonidoDerrotaReproducido = true;
            }
        mostrarPantallaGameOver();
    }
}




