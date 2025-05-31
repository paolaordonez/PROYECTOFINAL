#ifndef MAZE_H
#define MAZE_H

#include <Adafruit_ILI9341.h>

#define MAZE_WIDTH 8
#define MAZE_HEIGHT 11

class Maze {
public:
    Maze();
    void loadLevel(int level);
    void draw(Adafruit_ILI9341& screen);
    void clearTile(int x, int y, Adafruit_ILI9341& screen);
    bool isMoveValid(int x, int y) const;
    int getTile(int x, int y) const;
    void setTile(int x, int y, int value);
    int countItems(int itemCode) const;

private:
    int maze[MAZE_HEIGHT][MAZE_WIDTH];
    int* mazes[3];
};

#endif