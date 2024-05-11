#pragma once
#include <raylib.h>
#include "enemy.hpp"
class Alien: public Enemy
{
public:
    Alien(int type, Vector2 position);
    void Update(int direction);
    void Draw();
    int GetType();
    static void UnloadImages();
    Rectangle getRect();
    static Texture2D alienImages[3];
    int type;
    Vector2 position;
};