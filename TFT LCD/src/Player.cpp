#include "Player.h"
#include "SpriteP.h"
#include "Maze.h"

Player::Player() {

    reset();
}
void Player::reset() {
    x = 0;
    y = 1;
    prevX = x;
    prevY = y;
}
void Player::draw(Adafruit_ILI9341& screen,Maze&maze) {
    if (x != prevX || y != prevY) {
     maze.clearTile(prevX,prevY,screen);
    // screen.fillRect(prevX * 32, prevY * 32, 32, 32, ILI9341_BLACK);
    }
    screen.drawRGBBitmap(x * 32, y * 32, Player1[0], 32, 32);
}
void Player::mover(int dx, int dy) {
    prevX = x;
    prevY = y;
    x += dx;
    y += dy;
}
int Player::getX() const { return x; }
int Player::getY() const { return y; }