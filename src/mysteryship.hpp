#pragma once
#include <raylib.h>
#include "enemy.hpp"
class MysteryShip: public Enemy
{
public:
    MysteryShip();
    ~MysteryShip();
    int GetType();
    void Draw();
    void Update();
    void Update(int direction);
    void Spawn();
    Rectangle getRect();
    bool alive;

private:
    Texture2D image;
    Vector2 position;
    int speed;
};