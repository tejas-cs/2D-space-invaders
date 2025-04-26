#include "laser.hpp"

Laser::Laser(Vector2 position, int speed)
{
    this -> position = position;
    this -> speed = speed;
    active = true;
}

void Laser::Draw() {
    if(active)
        DrawRectangle(position.x, position.y, 4, 15, {57, 255, 20, 255});
}

Rectangle Laser::getRect()
{
    Rectangle rect;
    rect.x = position.x;
    rect.y = position.y;
    rect.width = 4;
    rect.height = 15;
    return rect;
}

void Laser::Update() {
    position.y += speed;
    if(active) {
        if(position.x >GetScreenHeight() || position.y < 0) {
            active = false;
        }
    }
}