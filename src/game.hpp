#pragma once
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"
#include "mysteryship.hpp"
using namespace std;
class Game
{
public:
    Game();
    ~Game();
    void Update();
    void Draw();
    void HandleInput();
    bool run;
    int lives;
    int score;
    int highScore;
    Music music;

private:
    void DeleteInactiveLasers();
    vector<Obstacle> CreateObstacles();
    vector<Alien> CreateAliens();
    void MoveAliens();
    void MoveDownAliens(int distance);
    void alienShootLaser();
    void CheckForCollisions();
    void GameOver();
    void Reset();
    void InitGame();
    void CheckForHighScore();
    void SaveHighScoreToFile(int highScore);
    int LoadHighScoreFromFile();
    Spaceship spaceship;
    vector<Obstacle> obstacles;
    vector<Alien> aliens;
    int alienDirection;
    vector<Laser> alienLasers;
    constexpr static float alienLaserShootInterval = 0.35;
    float timeLastAlienFired;
    MysteryShip mysteryShip;
    float mysteryShipSpawnInterval;
    float timeLastSpawn;
    Sound explosionSound;
};