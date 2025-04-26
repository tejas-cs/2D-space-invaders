#include "game.hpp"
#include <raylib.h>
#include <iostream>
#include <string>

std::string FormatWithLeadingZeros(int number, int width){
     std::string numberText = std::to_string(number);
     int leadingZeros = 6 - numberText.length();
     return numberText = std::string(leadingZeros, '0') + numberText;
 }

int main() {
  Color black = {0, 0, 0, 0};
  int offset = 50;
  int windowWidth = 750;
  int windowHeight = 700;

  InitWindow(windowWidth + offset, windowHeight + 2 * offset, "Space Invaders using raylib");
  InitAudioDevice();

  Font font = LoadFontEx("Font/monogram.ttf", 64, 0 ,0);
  Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");

  Game game;
  SetTargetFPS(60);

  while (WindowShouldClose() == false) {

     UpdateMusicStream(game.music);

    game.HandleInput();
    game.Update();

    BeginDrawing();
    ClearBackground(black);
    DrawRectangleRoundedLines({10, 10, 780, 780}, 0.0f, 20, WHITE);
    DrawLineEx({25, 730}, {775, 730}, 3, WHITE);
    if (game.run){
          DrawTextEx(font, "Level 01", {570, 740}, 34, 2, WHITE);
     }
     else{
          DrawTextEx(font, "GAME OVER", {570, 740}, 34, 2, WHITE);
     }

     float x = 50.0;
        for(int i = 0; i < game.lives; i++){
            DrawTextureV(spaceshipImage, {x, 745}, {57, 255, 20, 255});
            x += 50;
        }
     
     DrawTextEx(font, "SCORE", {50, 15}, 34, 2, WHITE);
     std::string scoreText = FormatWithLeadingZeros(game.score, 5);
     DrawTextEx(font, scoreText.c_str(), {50, 40}, 34, 2, WHITE);

     DrawTextEx(font, "HIGH SCORE", {570, 15}, 34, 2, WHITE);
     std::string highscoreText = FormatWithLeadingZeros(game.highscore, 5);
     DrawTextEx(font, highscoreText.c_str(), {655, 40}, 34, 2, WHITE);


    game.Draw();
    EndDrawing();
  }

  CloseWindow();
  CloseAudioDevice();
}
