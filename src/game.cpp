#include "game.hpp"
#include <raylib.h>
#include <iostream>
#include <fstream>

Game::Game() { 
    music = LoadMusicStream("Sounds/music.ogg");
    explosionSound = LoadSound("Sounds/explosion.ogg");
    PlayMusicStream(music);
    initGame();

}

Game::~Game() {
    Alien::unloadImages();
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
}

void Game::Update() {

    if(run) {
        double currentTime = GetTime();

        if(currentTime - timeLastSpawn > mysteryShipSpawnInterval) {
            mysteryship.Spawn();
            timeLastSpawn = GetTime();
            mysteryShipSpawnInterval = GetRandomValue(10, 20);
        }
    
      for (auto& laser : spaceship.lasers) {
        laser.Update();
      }
    
      for(auto& laser: alienLasers) {
        laser.Update();
      }
    
      moveAliens();
      alienShootLaser();
      deleteInactiveLaser();
      mysteryship.Update();
      checkForCollisons();
    
    } else if(IsKeyDown(KEY_ENTER)) {
        Reset();
        initGame();
    }

}

void Game::Draw() {
  spaceship.Draw();
  mysteryship.Draw();

  for (auto& laser : spaceship.lasers) {
    laser.Draw();
  }

  for (auto& obstacle : obstacles) {
    obstacle.Draw();
  }

  for(auto& alien: aliens) {
    alien.Draw();
  }

  for(auto& laser: alienLasers) {
    laser.Draw();
  }
}

void Game::HandleInput() {

    if(run) {
        if (IsKeyDown(KEY_LEFT)) {
            spaceship.MoveLeft();
          } else if (IsKeyDown(KEY_RIGHT)) {
            spaceship.MoveRight();
          }
          if (IsKeyDown(KEY_SPACE)) {
            spaceship.FireLaser();
          }
    }

}

std::vector<Obstacle> Game::createObstacles() {
  int obstaclewidth = Obstacle::grid[0].size() * 3;
  float gap = (GetScreenWidth() - (4 * obstaclewidth)) / 5;

  for (int i = 0; i < 4; i++) {
    float offsetX = (i + 1) * gap + i * obstaclewidth;
    float offsetY = GetScreenHeight() - 200;
    obstacles.push_back(Obstacle({offsetX, offsetY}));
  }
  return obstacles;
}

std::vector<Alien> Game::createAliens()
{
    std::vector<Alien> aliens;
    // hard coded cellsize , row and column size
    for(int row = 0; row < 5; row++) {
        for(int column = 0; column < 11; column++) {

            int alientype;
            if(row == 0) {
                alientype = 3;
            } else if(row == 1 || row == 2) {
                alientype = 2;
            } else {
                alientype = 1;
            }

            float x = 75 + column * 55;
            float y = 110 + row * 55;
            aliens.push_back(Alien(alientype, {x, y}));
        }
    }
    return aliens;
}

void Game::deleteInactiveLaser() {
  for (auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();) {
    if (!it->active) {
      it = spaceship.lasers.erase(it);
    } else {
      ++it;
    }
  }
  for (auto it = alienLasers.begin(); it != alienLasers.end();) {
    if (!it->active) {
      it = alienLasers.erase(it);
    } else {
      ++it;
    }
  }
}

void Game::checkForHighScore()
{
    if(score > highscore) {
        highscore = score;
        savedHighScoreToFile(highscore);
    }
}

void Game::gameOver()
{
    run = false;
}

void Game::initGame()
{
    obstacles = createObstacles();
    aliens = createAliens();
    aliensDirection = 1;
    timeLastAlienFire = 0.0;
    timeLastSpawn = 0.0;
    mysteryShipSpawnInterval = GetRandomValue(10, 20);
    lives = 3;
    run = true;
    score = 0;
    highscore =  loadHighScoreFromFile();
}

void Game::savedHighScoreToFile(int highscore)
{
    std::ofstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open()) {
        highscoreFile << highscore;
        highscoreFile.close();
    } else {
        std::cerr << "Failed to Save highscore file" << std::endl;
    }
}

int Game::loadHighScoreFromFile()
{
    int loadedHighScore = 0;
    std::ifstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open()) {
        highscoreFile >> loadedHighScore;
        highscoreFile.close();
    } else {
        std::cerr << "Failed to Load highscore file" << std::endl;
    }
    return loadedHighScore;
}

void Game::Reset() {
    spaceship.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();
}

void Game::moveAliens() {
    for(auto& alien: aliens) {
        if(alien.position.x + alien.alienimages[alien.type - 1].width > GetScreenWidth() - 25) {
            aliensDirection = -1;
            moveDownAliens(4);
        }
        if(alien.position.x < 25) {
            aliensDirection = 1;
            moveDownAliens(4);
        }
        alien.Update(aliensDirection);
    }
}

void Game::moveDownAliens(int distance)
{
    for(auto& alien: aliens) {
        alien.position.y += distance;
    }
}

void Game::alienShootLaser()
{
    double currentTime = GetTime();
    if(currentTime -  timeLastAlienFire >= alienLaserShootInterval && !aliens.empty()){
        int randomIdx = GetRandomValue(0, aliens.size() -  1);
        Alien& alien = aliens[randomIdx];
        alienLasers.push_back(Laser({alien.position.x + alien.alienimages[alien.type - 1].width / 2, alien.position.y + alien.alienimages[alien.type - 1].height}, 6));
        timeLastAlienFire = GetTime();
    }
}

void Game::checkForCollisons()
{
    // Spaceship Lasers

    for(auto& laser: spaceship.lasers) {
        auto it = aliens.begin();
        while(it != aliens.end()) {
            if(CheckCollisionRecs(it -> getRect(), laser.getRect())) {

                PlaySound(explosionSound);

                if(it -> type == 1) {
                    score += 100;
                } else if (it -> type == 1) {
                    score += 200;
                } else if (it -> type == 3) {
                    score += 300;
                }

                checkForHighScore();

                it = aliens.erase(it);
                laser.active = false;
            } else {
                ++it;
            }
        }

        for(auto& obstacle: obstacles){
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()){
                if (CheckCollisionRecs(it -> getRect(), laser.getRect())){
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                }
                else{
                    ++it;
                }
            }
        }

        if(CheckCollisionRecs(mysteryship.getRect(), laser.getRect())) {
            mysteryship.alive = false;
            laser.active = false;
            score += 500;
            checkForHighScore();
            PlaySound(explosionSound);
        }
    }

    // Alien Lasers

    for(auto& laser: alienLasers) {
        if(CheckCollisionRecs(laser.getRect(), spaceship.getRect())) {
            laser.active = false;
            lives --;
            if(lives == 0) {
                gameOver();
            }
        }

        
        for(auto& obstacle: obstacles){
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()){
                if (CheckCollisionRecs(it -> getRect(), laser.getRect())){
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                }
                else{
                    ++it;
                }
            }
        }
    }

    // Alien collision with obstacles

    for(auto& alien: aliens) {
        for(auto& obstacle: obstacles) {
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()) {
                if(CheckCollisionRecs(it -> getRect(), alien.getRect())) {
                    it = obstacle.blocks.erase(it);
                } else {
                    it ++;
                }
            }
        }

        if(CheckCollisionRecs(alien.getRect(), spaceship.getRect())) {
            gameOver();
        }
    }
}
