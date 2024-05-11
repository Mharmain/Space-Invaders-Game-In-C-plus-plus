#pragma once
#include <raylib.h>
class Enemy
{
    virtual void Update(int direction) = 0;
    virtual void Draw() = 0;
    virtual int GetType() = 0;
    virtual Rectangle getRect() = 0;
};