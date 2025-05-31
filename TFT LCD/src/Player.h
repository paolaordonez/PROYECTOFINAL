#ifndef PLAYER_H
#define PLAYER_H
#include <Adafruit_ILI9341.h>
class Maze;
class Player {
public:
    Player();
    void reset();
    void draw(Adafruit_ILI9341& screen, Maze& maze);
    void mover(int dx, int dy);
    int getX() const;
    int getY() const;

private:
    int x, y;
    int prevX, prevY;
};

#endif