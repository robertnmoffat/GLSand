#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <GL/glut.h>
#include <utility>

#include "data.h"

class GameState
{
private:
    int windowHeight = height;
    int windowWidth = width;

    Sand sands[height][width];

    static GameState* state;
    bool drawing=false;
    bool phase=0;

public:
    static GameState* getState();

    bool getPhase();
    void setPhase(bool phase);

    int getWindowHeight();

    int getWindowWidth();

    Sand getSand(int x, int y);

    void setWindowHeight(int height);

    void setWindowWidth(int width);

    void moveSand(int startx, int starty, int endx, int endy);

    void setSand(sType type, Colour colour, int x, int y);

    void setSand(int x, int y, int otherx, int othery);

    void setSand(Sand sand, int x, int y);

    void setSandMoving(bool moving, int x, int y);

    void setSandLeft(bool left, int x, int y);

    Colour getSandColour(int x, int y);
    void setSandColour(Colour c, int x, int y);

    void setSandBurning(bool isBurning, int x, int y);

    void setSandPhase(bool phase, int x, int y);
    void updateSandPhase(int x, int y);

    void swapCurrentPhase();

    void clearSand(int x, int y);

    void startDraw();
    void finishDraw();
};

#endif