#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <map>
#include <thread>
#include <chrono>

#include "soil/SOIL/SOIL.h"
#include "../headers/data.h"
#include "../headers/IO.h"
#include "../headers/GameState.h"
#include "../headers/MoveRules.h"
#include "../headers/Random.h"


GLuint texture_id;

GLubyte texture[height][width][3];//Texture array being used as screen. To be drawn to rectangle the size of view.

GameState *state;//Singleton that contains all sand states and functions to manipulate them.

const int WAIT = 15;//How long to wait in milliseconds between particle updates.

// Load the texture
void loadTexture()
{
    //Initilize array of random numbers
    Random::init();

    //Draw a bunch of random sand all over the screen (for testing)
    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (rand() % 20 == 10)//Random chance that sand is drawn on each pixel
            {
                Colour sandColour = SAND_COLOUR; // slightly randomize sand colouring
                int brightness = rand() % 40 - 20;//random fluctuations in sand colouring
                sandColour.r += brightness;
                sandColour.b += brightness;
                sandColour.g += brightness;
                state->setSand(sand, sandColour, x, y);
            }
        }
    }
    //Draw heavier sand on one side of dip
    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if(x>350&&x<450)continue;
            if (rand() % 10 == 1)
            {
                Colour sandColour = SAND_COLOUR; // slightly randomize sand colouring
                int brightness = rand() % 40 - 20;
                sandColour.r += brightness;
                sandColour.b += brightness;
                sandColour.g += brightness;
                state->setSand(sand, sandColour, x, y);
            }
        }
    }

    //Draw heavier sand on the other side of dip
    for (int y = 100; y < 110 - 1; y++)
    {
        for (int x = 100; x < 200; x++)
        {
            Colour sandColour = Colour{50, 50, 50}; // grayish
            int brightness = rand() % 40 - 20;
            sandColour.r += brightness;
            sandColour.b += brightness;
            sandColour.g += brightness;
            state->setSand(solid, sandColour, x, y);
        }
    }

    //Draw solid rock structure
    for (int y = 100; y < 200 - 1; y++)
    {
        for (int x = 100; x < 110; x++)
        {
            Colour sandColour = Colour{50, 50, 50}; // grayish
            int brightness = rand() % 40 - 20;
            sandColour.r += brightness;
            sandColour.b += brightness;
            sandColour.g += brightness;
            state->setSand(solid, sandColour, x, y);
        }
    }
    for (int y = 100; y < 200 - 1; y++)
    {
        for (int x = 200; x < 210; x++)
        {
            Colour sandColour = Colour{50, 50, 50}; // grayish
            int brightness = rand() % 40 - 20;
            sandColour.r += brightness;
            sandColour.b += brightness;
            sandColour.g += brightness;
            state->setSand(solid, sandColour, x, y);
        }
    }

    //Draw some oil
    for (int y = 110; y < 200 - 1; y++)
    {
        for (int x = 110; x < 190; x++)
        {
            Colour sandColour = Colour{10, 10, 10};
            state->setSand(oil, sandColour, x, y);
        }
    }

    //Draw some water
    for (int y = 100; y < 150 - 1; y++)
    {
        for (int x = 350; x < 450; x++)
        {
            state->setSand(water, WATER_COLOUR, x, y);
        }
    }

    // Generate the texture
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

/*
    Update positions of sand particles at the given coordinates.
*/
void updatePosition(int x, int y)
{
    //Current sand being updated.
    Sand curSand = state->getSand(x, y);

    //If the sand is not moving, don't bother updating it.
    if (curSand.moving == false)
        return;

    //If the current particle is open air, draw it blank and skip updates.
    if (curSand.type == none)
    {
        texture[y][x][0] = 20;
        texture[y][x][1] = 60;
        texture[y][x][2] = 20;
        return;
    }
    //Set texture at this position to sand colour at this position.
    texture[y][x][0] = curSand.colour.r;
    texture[y][x][1] = curSand.colour.g;
    texture[y][x][2] = curSand.colour.b;

    //If sand is going over the top of the screen just delete it
    if(y==height-1){
        state->clearSand(x,y);
    }
    if (y > 1)
    {
        //If it is a burning particle, first perform is burning functionality
        if(state->getSand(x,y).burning){
            MoveRules::burning(x,y);
        }

        //Depending on the particle type, handle it's movement
        switch (state->getSand(x, y).type)
        {
        case sand:
            MoveRules::sand(x, y);
            state->updateSandPhase(x, y);
            break;
        case water:
            MoveRules::liquid(x, y);
            state->updateSandPhase(x, y);
            break;
        case oil:
            MoveRules::liquid(x, y);
            state->updateSandPhase(x, y);
            break;
        case fire:
            MoveRules::fire(x, y);
            state->updateSandPhase(x, y);
            break;
        }
    }
}

// Function to get current time in millis 
long long getCurrentTimeInMillis() 
{ 
    return std::chrono::duration_cast<std::chrono::milliseconds>( 
        std::chrono::system_clock::now().time_since_epoch()).count(); 
}

/*
    Update for the first thread.
    TODO: Change to single function for both.
*/
void updateTex1()
{
    long curTime=0, lastTime=0, diff;//variables for dynamic wait times
    while (true)
    {
        curTime = getCurrentTimeInMillis();
        diff = (curTime-lastTime);
        if(WAIT-diff<0)diff = WAIT;//if if took longer than the full wait time, just set to the wait time so that wait will be 0 rather than negative
        std::this_thread::sleep_for(std::chrono::milliseconds(WAIT-diff));//adjust the wait time based on how long the particle update is taking. Smoothes times
        lastTime = getCurrentTimeInMillis();
        for (int y = 0; y < height; y += 2)
        {
            for (int x = 0; x < width; x += 2)
            {
                updatePosition(x, y);
            }
            for (int x = 1; x < width; x += 2) // count by 2's to prevent teleporting
            {
                updatePosition(x, y);
            }
        }        
    }
}

/*
    Update for the second thread.
    TODO: Change to single function for both.
*/
void updateTex2()
{
    long curTime=0, lastTime=0, diff;
    while (true)
    {
        curTime = getCurrentTimeInMillis();
        diff = (curTime-lastTime);
        if(WAIT-diff<0)diff = WAIT;//if if took longer than the full wait time, just set to the wait time so that wait will be 0 rather than negative
        std::this_thread::sleep_for(std::chrono::milliseconds(WAIT-diff));//adjust the wait time based on how long the particle update is taking. Smoothes times
        lastTime = getCurrentTimeInMillis();
        for (int y = 1; y < height; y += 2)
        {
            //Move through the particles by 2's to avoid updating the same particle again and again. (teleporting)
            for (int x = width - 1; x >= 0; x -= 2)
            {
                updatePosition(x, y);
            }
            for (int x = width - 2; x >= 0; x -= 2)
            {
                updatePosition(x, y);
            }
        }
    }
}

/*
    Old single-threaded texture update.
*/
void updateTex()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                updatePosition(x, y);
            }
            y++;
            if (y == height)
                continue;
            for (int x = width - 1; x >= 0; x--)
            {
                updatePosition(x, y);
            }
        }
    }
}

/*
    Draw the screen-sized rectangle that the texture pixels will be drawn to.
*/
void drawSquare()
{
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1.0, -1.0, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1.0, -1.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1.0, 1.0, 0.0);
    glEnd();
}

void display()
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(30));

    int x_offset = 0, y_offset = 0, sub_width = width, sub_height = height;
    glTexSubImage2D(GL_TEXTURE_2D, 0, x_offset, y_offset, sub_width, sub_height, GL_RGB, GL_UNSIGNED_BYTE, texture[x_offset][y_offset]);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    drawSquare();
    state->swapCurrentPhase();
    glDisable(GL_TEXTURE_2D);

    glFlush();

    state->swapCurrentPhase();
}

void reshape(int w, int h)
{
    state->setWindowHeight(h);
    state->setWindowWidth(w);
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    srand(time(0));
    state = GameState::getState();
    printf("size:%d\n", sizeof(Sand));
    printf("int:%d\n", sizeof(int));
    printf("char:%d\n", sizeof(char));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sand Simulation");
    glutKeyboardFunc(handleInput);
    glutMouseFunc(handleMouseInput);
    loadTexture();
    std::thread t1(updateTex1);
    std::thread t2(updateTex2);
    glutDisplayFunc(display);
    glutIdleFunc([]()
                 { glutPostRedisplay(); });
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
