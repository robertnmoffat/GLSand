
#include <cstdio>

#include "../headers/IO.h"
#include "../headers/data.h"
#include "../headers/GameState.h"

int currentMouseState = 0;

void handleMouseInput(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        GameState *state = GameState::getState();
        y = height - (int)((float)y / (float)state->getWindowHeight() * (float)height);
        x = (int)((float)x / (float)state->getWindowWidth() * (float)width);
        printf("WindowHeight:%d AdjustedPos:%d\n", state->getWindowHeight(), y);
        Colour sandColour;
        int brightness;
        for (int j = 0; j < 10; j++)
        {
            for (int i = 0; i < 10; i++)
            {
                switch (currentMouseState)
                {
                case 0:
                    state->setSand(water, WATER_COLOUR, x + j, y + i);
                    // sands[y+i][x].colour = Colour{rand()%255,rand()%255,0};
                    break;
                case 1:
                    sandColour = SAND_COLOUR; // slightly randomize sand colouring
                    brightness = rand() % 40 - 20;
                    sandColour.r += brightness;
                    sandColour.b += brightness;
                    sandColour.g += brightness;
                    state->setSand(sand, sandColour, x + j, y + i);
                    break;
                case 2:
                    sandColour = Colour{50, 50, 50}; // grayish
                    brightness = rand() % 40 - 20;
                    sandColour.r += brightness;
                    sandColour.b += brightness;
                    sandColour.g += brightness;
                    state->setSand(solid, sandColour, x + j, y + i);
                    break;
                case 3:
                    sandColour = OIL_COLOUR; // grayish
                    brightness = rand() % 10 - 5;
                    sandColour.r += brightness;
                    sandColour.b += brightness;
                    sandColour.g += brightness;
                    state->setSand(oil, sandColour, x + j, y + i);
                    break;
                case 4:
                    sandColour = FIRE_COLOUR; // grayish
                    brightness = rand() % 10 - 5;
                    sandColour.r += brightness;
                    sandColour.b += brightness;
                    sandColour.g += brightness;
                    state->setSand(fire, sandColour, x + j, y + i);
                    break;
                case 5:
                    sandColour = FIRE_COLOUR; // grayish
                    brightness = rand() % 10 - 5;
                    sandColour.r += brightness;
                    sandColour.b += brightness;
                    sandColour.g += brightness;
                    //state->setSand(burning, sandColour, x + j, y + i);
                    break;
                }
            }
        }
    }
}

void handleInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 49:
        currentMouseState = 0;
        break;
    case 50:
        currentMouseState = 1;
        break;
    case 51:
        currentMouseState = 2;
        break;
    case 52:
        currentMouseState = 3;
        break;
    case 53:
        currentMouseState = 4;
        break;
    case 54:
        currentMouseState = 5;
        break;
    case 27: // escape key
        exit(0);
        break;
    case 32: // space key
        //GameState::getState()->setSand(sand, SAND_COLOUR, 100, 100);

        break;
    }
}