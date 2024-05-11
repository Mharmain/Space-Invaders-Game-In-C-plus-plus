#pragma once
#include <vector>
#include "block.hpp"
using namespace std;
class Obstacle
{
public:
    Obstacle(Vector2 position);
    void Draw();
    Vector2 Position;
    vector<Block> blocks;
    static vector<vector<int>> grid;
};