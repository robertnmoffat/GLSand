#include"../headers/Random.h"

int Random::nums[Random::length];
int Random::pos=0;

void Random::init(){
    for(int i=0; i<length; i++){
        Random::nums[i]=std::rand();
    }
    pos=0;
}
