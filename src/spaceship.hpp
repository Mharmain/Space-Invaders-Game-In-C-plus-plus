#pragma once
#include <raylib.h>
#include "laser.hpp"
#include <vector>
using namespace std;
class Spaceship
{
public:
    Spaceship();
    ~Spaceship();
    void Draw();
    void Moveleft();
    void MoveRight();
    void Firelaser();
    void Reset();
    Rectangle getRect();
    vector<Laser> lasers;

private:
    Texture2D image;
    Vector2 position;
    double lastFireTime;
    Sound laserSound;
};