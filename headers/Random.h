#ifndef RANDOM_H
#define RANDOM_H

#include<iostream>

class Random{
    public:
        static inline int next(){return Random::nums[pos++%length];}
        static void init();
    private:
        static const int length = 1000;
        static int pos;
        static int nums[length];
};

#endif