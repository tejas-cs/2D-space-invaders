#pragma once
#include <raylib.h>

class Alien {
    public:
        Alien(int type, Vector2 position);
        void Update(int direction);
        void Draw();
        int GetType();
        static void unloadImages();
        Rectangle getRect();
        static Texture2D alienimages[3];
        int type;
        Vector2 position;
    private:
};