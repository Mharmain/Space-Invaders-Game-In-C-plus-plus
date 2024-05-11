#include <raylib.h>
#include "game.hpp"
#include <string>
using namespace std;
string FormatWithLeadingZeros(int number, int width)
{
    string numberText = to_string(number);
    int leadingZeros = width - numberText.length();
    return numberText = string(leadingZeros, '0') + numberText;
}
int main()
{
    Color brown = {76, 63, 47, 255};
    Color blue = {102, 191, 255, 255};
    int offSet = 50;
    int windowWidth = 750;
    int windowHeight = 700;

    InitWindow(windowWidth + offSet, windowHeight + 2 * offSet, "C++ Space Invaders");
    InitAudioDevice();
    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");
    SetTargetFPS(60);
    Game game;
    while (WindowShouldClose() == false)
    {
        UpdateMusicStream(game.music);
        game.HandleInput();
        game.Update();
        BeginDrawing();
        ClearBackground(brown);
        DrawRectangleRoundedLines({10, 10, 780, 780}, 0.0, 20, 2, blue);
        DrawLineEx({25, 730}, {775, 730}, 3, blue);
        if (game.run)
        {
            DrawTextEx(font, "LEVEL 01", {50, 740}, 34, 2, blue);
        }
        else
        {
            DrawTextEx(font, "GAME OVER", {50, 740}, 34, 2, blue);
        }
        float x = 570;
        for (int i = 0; i < game.lives; i++)
        {
            DrawTextureV(spaceshipImage, {x, 740}, WHITE);
            x += 50;
        }
        DrawTextEx(font, "SCORE", {250, 15}, 34, 2, blue);
        string scoreText = FormatWithLeadingZeros(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), {250, 40}, 34, 2, blue);
        DrawTextEx(font, "HIGH-SCORE", {410, 15}, 34, 2, blue);
        string highscoreText = FormatWithLeadingZeros(game.highScore, 5);
        DrawTextEx(font, highscoreText.c_str(), {455, 40}, 34, 2, blue);
        game.Draw();
        EndDrawing();
    }
    CloseWindow();
    CloseAudioDevice();

    return 0;
}