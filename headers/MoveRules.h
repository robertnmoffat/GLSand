#ifndef MOVERULES_H
#define MOVERULES_H

class MoveRules{
    public:
    static bool fall(int x,int y);
    static bool sand(int x, int y);
    static bool liquid(int x, int y);
    static bool fire(int x, int y);
    static bool burning(int x, int y);
};

#endif