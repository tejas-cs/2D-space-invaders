#include "alien.hpp"

Texture2D Alien::alienimages[3] = {};

Alien::Alien(int type, Vector2 position)
{
    this -> type = type;
    this -> position = position;

    if(alienimages[type -1].id == 0) {

        switch(type) {
            case 1:
                alienimages[0] = LoadTexture("Graphics/alien_1.png");
                break;
            case 2:
                alienimages[1] = LoadTexture("Graphics/alien_2.png");
                break;
            case 3:
                alienimages[2] = LoadTexture("Graphics/alien_3.png");
                break;
            default:
                alienimages[0] = LoadTexture("Graphics/alien_1.png");
                break;
        }
    }
}

void Alien::Draw() {
    DrawTextureV(alienimages[type -1], position, {57, 255, 20, 255});
}

int Alien::GetType() {
    return type;
}

void Alien::unloadImages()
{
    for(int i = 0; i < 3; i++) {
        UnloadTexture(alienimages[i]);
    }
}

Rectangle Alien::getRect()
{
    return {position.x, position.y, float(alienimages[type - 1].width), float(alienimages[type - 1].height)};
}

void Alien::Update(int direction) {
    position.x += direction;
}