#include "../headers/GameState.h"

GameState* GameState::state = nullptr;

GameState *GameState::getState()
{
    if (GameState::state == nullptr)
    {
        GameState::state = new GameState();
    }
    return GameState::state;
}

int GameState::getWindowHeight()
{
    return windowHeight;
}

int GameState::getWindowWidth()
{
    return windowWidth;
}

Sand GameState::getSand(int x, int y)
{
    return sands[y][x];
}

void GameState::setWindowHeight(int height)
{
    windowHeight = height;
}

void GameState::setWindowWidth(int width)
{
    windowWidth = width;
}

void GameState::setSand(sType type, Colour colour, int x, int y)
{
    while(drawing){}//wait for draw
    sands[y][x].type = type;
    sands[y][x].colour = colour;
    sands[y][x].left=rand()%2;
}

void GameState::setSand(int x, int y, int otherx, int othery)
{
    while(drawing){}//wait for draw
    sands[othery][otherx].type = sands[y][x].type;
    sands[othery][otherx].colour = sands[y][x].colour;
}

void GameState::setSand(Sand sand, int x, int y){
    while(drawing){}//wait for draw
    sands[y][x].type = sand.type;
    sands[y][x].colour = sand.colour;
    sands[y][x].left = sand.left;
}

void GameState::setSandMoving(bool moving, int x, int y){
    sands[y][x].moving=moving;
}

bool GameState::getPhase(){
    return phase;
}

void GameState::setSandPhase(bool phase, int x, int y){
    sands[y][x].phase=phase;
}

void GameState::updateSandPhase(int x, int y){
    sands[y][x].phase = phase;
}

void GameState::setPhase(bool phase){
    GameState::phase=phase;
}

void GameState::swapCurrentPhase(){
    phase = !phase;
}

void GameState::setSandLeft(bool left, int x, int y){
    sands[y][x].left=left;
}

Colour GameState::getSandColour(int x, int y){
    return sands[y][x].colour;
}

void GameState::setSandBurning(bool isBurning, int x, int y){
    sands[y][x].burning = isBurning; 
    //if(sands[y][x].type==sType::oil)   
    //sands[y][x].colour = Colour{0,255,0};
}

void GameState::setSandColour(Colour c, int x, int y){
    sands[y][x].colour = c;
}

void GameState::clearSand(int x, int y){
    while(drawing){}//wait for draw
    sands[y][x].type=none;
    sands[y][x].burning=false;
}

void GameState::startDraw(){
    drawing=true;
}
void GameState::finishDraw(){
    drawing=false;
}

void GameState::moveSand(int startx, int starty, int endx, int endy){
    std::swap(sands[starty][startx], sands[endy][endx]);
    if(endy==height)
    GameState::clearSand(endx,endy);
}