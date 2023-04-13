#include "../headers/MoveRules.h"
#include "../headers/GameState.h"
#include "../headers/data.h"
#include <cstdio>
bool MoveRules::fall(int x, int y)
{
    GameState *state = GameState::getState();

    if (state->getSand(x, y - 1).type < state->getSand(x, y).type)
    {
        state->moveSand(x, y, x, y - 1);
        // state->setSandMoving(true, x, y-1);
        return true;
    }

    // state->setSandMoving(false,x,y);

    return false;
}

bool MoveRules::sand(int x, int y)
{
    GameState *state = GameState::getState();

    if (MoveRules::fall(x, y))
    {
        return true;
    }
    bool dir = rand() % 2;
    if (dir)
    {
        if (state->getSand(x - 1, y - 1).type < sType::sand)
        {
            state->moveSand(x, y, x - 1, y - 1);
            // state->setSandMoving(true,x-1,y-1);
            return true;
        }
        else if (state->getSand(x + 1, y - 1).type < sType::sand)
        {
            state->moveSand(x, y, x + 1, y - 1);
            // state->setSandMoving(true,x+1,y-1);
            return true;
        }
    }
    else
    {
        if (state->getSand(x + 1, y - 1).type < sType::sand)
        {
            state->moveSand(x, y, x + 1, y - 1);
            // state->setSandMoving(true,x+1,y-1);
            return true;
        }
        else if (state->getSand(x - 1, y - 1).type < sType::sand)
        {
            state->moveSand(x, y, x - 1, y - 1);
            // state->setSandMoving(true,x-1,y-1);
            return true;
        }
    }
    // state->setSandMoving(false,x,y);
    return false;
}

bool MoveRules::liquid(int x, int y)
{
    GameState *state = GameState::getState();

    if (MoveRules::fall(x, y))
    {
        y -= 1;
        if (rand() % 4 < 3)
            return true;
        if (rand() % 10 == 1)
            state->setSandLeft(rand() % 2, x, y); // randomize flow direction while falling
    }
    bool dir = state->getSand(x, y).left;
    if (dir == 0)
    {
        if (state->getSand(x + 1, y).type < state->getSand(x, y).type) // if direction open move there and hold the direction to encourage flow
        {
            state->moveSand(x, y, x + 1, y);
            return true;
        }
        else if (state->getSand(x + 2, y).type < state->getSand(x, y).type) // if direction open move there and hold the direction to encourage flow
        {
            state->moveSand(x, y, x + 2, y);
            return true;
        }
        state->setSandLeft(true, x, y); // if direction blocked change direction
        return false;
    }
    if (dir == 1)
    {
        if (state->getSand(x - 1, y).type < state->getSand(x, y).type)
        {
            state->moveSand(x, y, x - 1, y);
            return true;
        }
        else if (state->getSand(x - 2, y).type < state->getSand(x, y).type)
        {
            state->moveSand(x, y, x - 2, y);
            return true;
        }
        state->setSandLeft(false, x, y);
        return false;
    }
    // Sand s = state->getSand(x,y);
    // s.left=rand()%2;
    // state->setSand(s,x,y);
    return false;
}

bool MoveRules::fire(int x, int y)
{
    GameState *state = GameState::getState();
    char brightness;

    if (rand() % 2)
        return true;

    if (state->getSand(x + 1, y).type == oil)
    {
        if (rand() % 10 == 1)
        {
            //state->setSand(sType::fire, FIRE_COLOUR, x + 1, y);
            //state->setSandColour(FIRE_COLOUR, x+1,y);
            state->setSand(state->getSand(x+1,y).type, FIRE_COLOUR, x+1,y);
            state->setSandBurning(true,x+1,y);
        }
        else
        {
            //state->setSand(sType::fire, FIRE_COLOUR, x, y + 1);
        }
    }
    if (state->getSand(x - 1, y).type == oil)
    {
        if (rand() % 10 == 1)
        {
            //state->setSand(sType::fire, FIRE_COLOUR, x - 1, y);
            state->setSand(state->getSand(x-1,y).type, FIRE_COLOUR, x-1,y);
            state->setSandBurning(true,x-1,y);
            //state->setSandColour(FIRE_COLOUR, x-1,y);
        }
        else
        {
            //state->setSand(sType::fire, FIRE_COLOUR, x, y + 1);
        }
    }
    if (state->getSand(x, y - 1).type == oil)
    {
        if (rand() % 15 == 1){
            //state->setSand(sType::fire, FIRE_COLOUR, x, y - 1);
            //state->setSandColour(FIRE_COLOUR, x,y-1);
            state->setSand(state->getSand(x,y-1).type, FIRE_COLOUR, x,y-1);
            state->setSandBurning(true,x,y-1);
        }
        else{
            //state->setSand(sType::fire, FIRE_COLOUR, x, y + 1);
        }
    }
    if (state->getSand(x, y + 1).type == oil)
    {
        // if(rand()%10==1)
        state->setSand(sType::fire, FIRE_COLOUR, x, y + 1);
    }
    if (state->getSand(x, y + 1).type == sType::fire)
    {
        if (rand() % 4 == 1)//was 4
            state->setSandColour(FIRE_COLOUR, x, y);
    }

    int dir = rand() % 6;
    if (dir == 0)
    {
        if (state->getSand(x + 1, y).type <= sType::fire)
        {
            state->moveSand(x, y, x++, y);
        }
    }
    if (dir == 1)
    {
        if (state->getSand(x - 1, y).type <= sType::fire)
        {
            state->moveSand(x, y, x--, y);
        }
    }
    if (state->getSand(x, y + 1).type <= sType::fire)
    {
        state->moveSand(x, y, x, y++);
    }else {
        if(rand()%4==1)//chance to clear if up against roof
        state->clearSand(x,y);
    }


    Colour c = state->getSandColour(x, y);

    // if (c.g == FIRE_COLOUR.g && rand() % 100 == 1)
    // {
    //     if (state->getSand(x - 1, y).type == sType::solid)
    //     {
    //         state->setSand(sType::burning, FIRE_COLOUR, x - 1, y);
    //     }
    //     if (state->getSand(x + 1, y).type == sType::solid)
    //     {
    //         state->setSand(sType::burning, FIRE_COLOUR, x + 1, y);
    //     }
    //     if (state->getSand(x, y + 1).type == sType::solid)
    //     {
    //         state->setSand(sType::burning, FIRE_COLOUR, x, y + 1);
    //     }
    // }

    brightness = rand() % 10 + 5;
    if (c.g + brightness < 255)
    {
        c.g += brightness;
    }
    else
    {
        if (rand() % 5 == 1)
        { // done burning
            state->clearSand(x, y);
            return false;
        }
    }
    state->setSandColour(c, x, y);

    return true;
}

bool MoveRules::burning(int x, int y)
{
    GameState *state = GameState::getState();

    // if (rand() % 25000 == 1)
    // {
    //     state->setSand(sType::burning, FIRE_COLOUR, x, y - 1);
    // }

    int burnLength = 800;

    if (rand() % burnLength == 1 || state->getSand(x, y + 1).type == sType::water)
    { // done burning
        if (0)
            state->setSand(sType::sand, SAND_COLOUR, x, y);
        else
            state->clearSand(x, y);
        return false;
    }
    if (rand() % 4 == 1 && state->getSand(x, y + 1).type != state->getSand(x, y).type)
    {
        state->setSand(sType::fire, FIRE_COLOUR, x, y + 1);
    }

    if(rand()%5==1&&state->getSand(x+1,y).type==sType::oil){//state->getSand(x,y).type){
        state->setSandBurning(true, x+1,y);
    }
    if(rand()%5==1&&state->getSand(x-1,y).type==sType::oil){//state->getSand(x,y).type){
        state->setSandBurning(true, x-1,y);
    }
    if(rand()%75==1&&state->getSand(x,y-1).type==sType::oil){//state->getSand(x,y).type){
        state->setSandBurning(true, x,y-1);
    }

    // if (MoveRules::fall(x, y))
    // {
    //     return true;
    // }
    // bool dir = rand() % 2;
    // if (dir)
    // {
    //     if (state->getSand(x - 1, y - 1).type < sType::sand || state->getSand(x - 1, y - 1).type == sType::fire)
    //     {
    //         state->moveSand(x, y, x - 1, y - 1);
    //         // state->setSandMoving(true,x-1,y-1);
    //         return true;
    //     }
    //     else if (state->getSand(x + 1, y - 1).type < sType::sand || state->getSand(x + 1, y - 1).type == sType::fire)
    //     {
    //         state->moveSand(x, y, x + 1, y - 1);
    //         // state->setSandMoving(true,x+1,y-1);
    //         return true;
    //     }
    // }
    // else
    // {
    //     if (state->getSand(x + 1, y - 1).type < sType::sand || state->getSand(x + 1, y - 1).type == sType::fire)
    //     {
    //         state->moveSand(x, y, x + 1, y - 1);
    //         // state->setSandMoving(true,x+1,y-1);
    //         return true;
    //     }
    //     else if (state->getSand(x - 1, y - 1).type < sType::sand || state->getSand(x - 1, y - 1).type == sType::fire)
    //     {
    //         state->moveSand(x, y, x - 1, y - 1);
    //         // state->setSandMoving(true,x-1,y-1);
    //         return true;
    //     }
    // }

    return true;
}