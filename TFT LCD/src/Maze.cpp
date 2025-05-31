#include "Maze.h"
#include <Arduino.h>
#include "SpritePared.h"
#include "SpriteLava.h"
#include "SpriteRecolectable.h"
//#include "SpriteMeta.h"

Maze::Maze() {
    static int maze1[MAZE_HEIGHT][MAZE_WIDTH] = {
        {1,1,1,1,1,1,1,1},
        {2,2,2,5,2,2,5,1},
        {1,2,2,4,1,2,1,1},
        {1,1,1,2,5,2,1,1},
        {1,2,1,4,4,4,2,1},
        {1,2,4,1,2,1,4,1},
        {1,4,1,1,1,5,2,1},
        {1,4,4,4,4,2,2,1},
        {1,1,1,1,4,1,4,1},
        {1,1,1,1,1,1,3,1},
        {1,1,1,1,1,1,1,1}
    };
    static int maze2[MAZE_HEIGHT][MAZE_WIDTH] = {
        {1,1,1,1,1,1,1,1},
        {2,2,2,5,4,4,5,1},
        {1,5,2,4,1,4,1,1},
        {1,2,2,2,4,2,4,1},
        {1,1,1,4,4,5,2,1},
        {1,4,4,1,2,1,3,1},
        {1,5,4,2,2,5,5,1},
        {1,4,1,1,4,2,2,1},
        {1,2,2,2,4,2,4,1},
        {1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1}
    };
    static int maze3[MAZE_HEIGHT][MAZE_WIDTH] = {
        {1,1,1,1,1,1,1,1},
        {2,2,4,2,4,5,3,1},
        {1,5,4,4,2,5,2,1},
        {1,2,5,4,2,2,2,1},
        {1,1,4,4,1,1,5,1},
        {1,4,2,5,2,2,4,1},
        {1,2,4,4,2,5,2,1},
        {1,4,5,4,4,1,2,1},
        {1,2,2,1,4,1,4,1},
        {1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1}
    };
    mazes[0] = (int*)maze1;
    mazes[1] = (int*)maze2;
    mazes[2] = (int*)maze3;
}

void Maze::loadLevel(int level) {
    memcpy(maze, mazes[level], sizeof(maze));
}

void Maze::draw(Adafruit_ILI9341& screen) {
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
            switch (maze[i][j]) {
                case 1:
                    screen.drawRGBBitmap(j * 32, i * 32, Pared[0], 32, 32);
                    break;
                case 2:
                    screen.fillRect(j * 32, i * 32, 32, 32, ILI9341_BLACK);
                    break;
                case 3:
                    screen.drawRGBBitmap(j * 32, i * 32, Meta[0], 32, 32);
                    break;
                case 4:
                    screen.drawRGBBitmap(j * 32, i * 32, Cereza[0], 32, 32);
                    break;
                case 5:
                    screen.drawRGBBitmap(j * 32, i * 32, Fuego[0], 32, 32);
                    break;
            }
        }
    }
}

void Maze::clearTile(int x, int y, Adafruit_ILI9341& screen) {
    switch (maze[y][x]) {
        case 1:
            screen.drawRGBBitmap(x * 32, y * 32, Pared[0], 32, 32);
            break;
        case 2:
            screen.fillRect(x * 32, y * 32, 32, 32, ILI9341_BLACK);
            break;
        case 3:
            screen.drawRGBBitmap(x * 32, y * 32, Meta[0], 32, 32);
            break;
        case 4:
            screen.drawRGBBitmap(x * 32, y * 32, Cereza[0], 32, 32);
            break;
        case 5:
            screen.drawRGBBitmap(x * 32, y * 32, Fuego[0], 32, 32);
            break;
    }
}

bool Maze::isMoveValid(int x, int y) const {
    return (x >= 0 && x < MAZE_WIDTH && y >= 0 && y < MAZE_HEIGHT && maze[y][x] != 1);
}

int Maze::getTile(int x, int y) const {
    return maze[y][x];
}

void Maze::setTile(int x, int y, int value) {
    maze[y][x] = value;
}

int Maze::countItems(int itemCode) const {
    int count = 0;
    for (int i = 0; i < MAZE_HEIGHT; ++i) {
        for (int j = 0; j < MAZE_WIDTH; ++j) {
            if (maze[i][j] == itemCode) count++;
        }
    }
    return count;
}
