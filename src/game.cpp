#include "game.hpp"
#include <iostream>
#include <fstream>
using namespace std;
Game::Game()
{
    music = LoadMusicStream("Sounds/music.ogg");
    explosionSound = LoadSound("Sounds/explosion.ogg");
    PlayMusicStream(music);
    InitGame();
}
Game::~Game()
{
    Alien::UnloadImages();
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
}
void Game::Update()
{
    if (run)
    {
        double currentTime = GetTime();
        if (currentTime - timeLastSpawn > mysteryShipSpawnInterval)
        {
            mysteryShip.Spawn();
            timeLastSpawn = GetTime();
            mysteryShipSpawnInterval = GetRandomValue(10, 20);
        }
        for (auto &Laser : spaceship.lasers)
        {
            Laser.update();
        }
        MoveAliens();
        alienShootLaser();
        for (auto &Laser : alienLasers)
        {
            Laser.update();
        }
        DeleteInactiveLasers();
        mysteryShip.Update();
        CheckForCollisions();
    }
    else
    {
        if (IsKeyDown(KEY_ENTER))
        {
            Reset();
            InitGame();
        }
    }
}
void Game::Draw()
{
    spaceship.Draw();
    for (auto &Laser : spaceship.lasers)
    {
        Laser.Draw();
    }
    for (auto &obstacle : obstacles)
    {
        obstacle.Draw();
    }
    for (auto &alien : aliens)
    {
        alien.Draw();
    }
    for (auto &Laser : alienLasers)
    {
        Laser.Draw();
    }
    mysteryShip.Draw();
}
void Game::HandleInput()
{
    if (run)
    {
        if (IsKeyDown(KEY_LEFT))
        {
            spaceship.Moveleft();
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            spaceship.MoveRight();
        }
        else if (IsKeyDown(KEY_SPACE))
        {
            spaceship.Firelaser();
        }
    }
}
void Game::DeleteInactiveLasers()
{
    for (auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();)
    {
        if (!it->active)
        {
            it = spaceship.lasers.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for (auto it = alienLasers.begin(); it != alienLasers.end();)
    {
        if (!it->active)
        {
            it = alienLasers.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
vector<Obstacle> Game::CreateObstacles()
{
    int obstaclewidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth() - (4 * obstaclewidth)) / 5;

    for (int i = 0; i < 4; i++)
    {
        float offSetX = (i + 1) * gap + i * obstaclewidth;
        obstacles.push_back(Obstacle({offSetX, float(GetScreenHeight() - 200)}));
    }
    return obstacles;
}
vector<Alien> Game::CreateAliens()
{
    vector<Alien> aliens;
    for (int row = 0; row < 5; row++)
    {
        for (int col = 0; col < 11; col++)
        {
            int alienType;
            if (row == 0)
            {
                alienType = 3;
            }
            else if (row == 1 || row == 2)
            {
                alienType = 2;
            }
            else
            {
                alienType = 1;
            }
            float x = 75 + col * 55;
            float y = 110 + row * 55;
            aliens.push_back(Alien(alienType, {x, y}));
        }
    }
    return aliens;
}
void Game::MoveAliens()
{
    for (auto &alien : aliens)
    {
        if (alien.position.x + alien.alienImages[alien.type - 1].width > GetScreenWidth() - 25)
        {
            alienDirection = -1;
            MoveDownAliens(4);
        }
        if (alien.position.x < 25)
        {
            alienDirection = 1;
            MoveDownAliens(4);
        }
        alien.Update(alienDirection);
    }
}
void Game::MoveDownAliens(int distance)
{
    for (auto &alien : aliens)
    {
        alien.position.y += distance;
    }
}
void Game::alienShootLaser()
{
    double currentTime = GetTime();
    if (currentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty())
    {
        int randomIndex = GetRandomValue(0, aliens.size() - 1);
        Alien &alien = aliens[randomIndex];
        alienLasers.push_back(Laser({alien.position.x + alien.alienImages[alien.type - 1].width / 2, alien.position.y + alien.alienImages[alien.type - 1].height}, 6));
        timeLastAlienFired = GetTime();
    }
}
void Game::CheckForCollisions()
{
    // spaceshipLasers
    for (auto &laser : spaceship.lasers)
    {
        auto it = aliens.begin();
        while (it != aliens.end())
        {
            if (CheckCollisionRecs(it->getRect(), laser.getRect()))
            {
                PlaySound(explosionSound);
                if (it->type == 1)
                {
                    score += 100;
                }
                else if (it->type == 2)
                {
                    score += 200;
                }
                else if (it->type == 3)
                {
                    score += 300;
                }
                CheckForHighScore();
                it = aliens.erase(it);
                laser.active = false;
            }
            else
            {
                ++it;
            }
            if (aliens.empty())
            {
                GameOver();
            }
        }
        for (auto &obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->getRect(), laser.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                }
                else
                {
                    ++it;
                }
            }
        }
        if (CheckCollisionRecs(mysteryShip.getRect(), laser.getRect()))
        {
            mysteryShip.alive = false;
            laser.active = false;
            score += 500;
            CheckForHighScore();
            PlaySound(explosionSound);
        }
    }
    // alienLasers
    for (auto &laser : alienLasers)
    {
        if (CheckCollisionRecs(laser.getRect(), spaceship.getRect()))
        {
            laser.active = false;
            lives--;
            if (lives == 0)
            {
                GameOver();
            }
        }
        for (auto &obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->getRect(), laser.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                }
                else
                {
                    ++it;
                }
            }
        }
    }
    // aliens colliding with obstacles
    for (auto &alien : aliens)
    {
        for (auto &obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->getRect(), alien.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                }
                else
                {
                    it++;
                }
            }
        }
        if (CheckCollisionRecs(alien.getRect(), spaceship.getRect()))
        {
            GameOver();
        }
    }
}
void Game::GameOver()
{
    run = false;
}
void Game::InitGame()
{
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    alienDirection = 1;
    timeLastAlienFired = 0;
    timeLastSpawn = 0.0;
    mysteryShipSpawnInterval = GetRandomValue(10, 20);
    lives = 3;
    run = true;
    score = 0;
    highScore = LoadHighScoreFromFile();
}
void Game::CheckForHighScore()
{
    if (score > highScore)
    {
        highScore = score;
        SaveHighScoreToFile(highScore);
    }
}
void Game::SaveHighScoreToFile(int highScore)
{
    ofstream highscoreFile("highscore.txt");
    if (highscoreFile.is_open())
    {
        highscoreFile << highScore;
        highscoreFile.close();
    }
    else
    {
        cerr << "Failed To Save High Score in File!" << endl;
    }
}
int Game::LoadHighScoreFromFile()
{
    int loadedHighScore = 0;
    ifstream highscoreFile("highscore.txt");
    if (highscoreFile.is_open())
    {
        highscoreFile >> loadedHighScore;
        highscoreFile.close();
    }
    else
    {
        cerr << "Failed To Load High Score from File!" << endl;
    }
    return loadedHighScore;
}
void Game::Reset()
{
    spaceship.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();
}