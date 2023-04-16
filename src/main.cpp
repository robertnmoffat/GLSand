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

float angle = 0.0f;

GLuint texture_id;

GLubyte texture[height][width][3];

int frameCounter = 0;

GameState *state;

bool textureInUse = false;

bool firstThreadDone = true;
bool secondThreadDone = true;

// Load the texture
void loadTexture()
{
    Random::init();
    // Set a random color for the texture
    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (rand() % 20 == 10)
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
    for (int y = 110; y < 200 - 1; y++)
    {
        for (int x = 110; x < 190; x++)
        {
            Colour sandColour = Colour{10, 10, 10}; // grayish
            state->setSand(oil, sandColour, x, y);
        }
    }

    for (int y = 100; y < 150 - 1; y++)
    {
        for (int x = 350; x < 450; x++)
        {
            //Colour sandColour = Colour{10, 10, 10}; // grayish
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

void updatePosition(int x, int y)
{
    Sand curSand = state->getSand(x, y);

    if (curSand.moving == false)
        return;

    if (curSand.type == none)
    {
        texture[y][x][0] = 20;
        texture[y][x][1] = 60;
        texture[y][x][2] = 20;
        return;
    }
    texture[y][x][0] = curSand.colour.r;
    texture[y][x][1] = curSand.colour.g;
    texture[y][x][2] = curSand.colour.b;

    if(y==height-1){
        state->clearSand(x,y);
    }
    if (y > 1)
    {
        // if(state->getSand(x,y).phase==state->getPhase())return;//already updated
        if(state->getSand(x,y).burning){
            MoveRules::burning(x,y);
            // if(rand()%2==1&&state->getSand(x,y+1).type==sType::fire){
            //     state->moveSand(x,y,x,y+1);
            // }
        }

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

void updateTex1()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
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

void updateTex2()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        for (int y = 1; y < height; y += 2)
        {
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

        // int x_offset = 0, y_offset = 0, sub_width = width, sub_height = height;
        // glTexSubImage2D(GL_TEXTURE_2D, 0, x_offset, y_offset, sub_width, sub_height, GL_RGB, GL_UNSIGNED_BYTE, texture[x_offset][y_offset]);
    }
}

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
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    int x_offset = 0, y_offset = 0, sub_width = width, sub_height = height;
    glTexSubImage2D(GL_TEXTURE_2D, 0, x_offset, y_offset, sub_width, sub_height, GL_RGB, GL_UNSIGNED_BYTE, texture[x_offset][y_offset]);

    // glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // glLoadIdentity();

    // glRotatef(angle, 0.0f, 0.0f, 1.0f);
    //  glTranslatef(0.5f, 0.5f, 0.0f);

    drawSquare();
    state->swapCurrentPhase();
    glDisable(GL_TEXTURE_2D);

    glFlush();

    state->swapCurrentPhase();
    // glutSwapBuffers();

    // angle += 0.1f;
    //  loadTexture();
    // updateTex();

    // if (frameCounter == 2)
    // {
    //     updateTex();
    //     frameCounter=0;
    // }
    // else
    // {
    //     frameCounter++;
    // }
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
