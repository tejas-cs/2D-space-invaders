#pragma once
#include "obstacle.hpp"
#include "spaceship.hpp"
#include <vector>
#include "alien.hpp"
#include "mysteryship.hpp"

class Game {
public:
  Game();
  ~Game();
  void Draw();
  void Update();
  void HandleInput();
  bool run;
  int lives;
  int score;
  int highscore;
  Music music;
private:
  Spaceship spaceship;
  std::vector<Obstacle> createObstacles();
  std::vector<Alien> createAliens();
  void moveAliens();
  void moveDownAliens(int distance);
  void alienShootLaser();
  void checkForCollisons();
  void deleteInactiveLaser();
  void checkForHighScore();
  std::vector<Obstacle> obstacles;
  std::vector<Alien> aliens;
  int aliensDirection;
  std::vector<Laser> alienLasers;
  constexpr static float alienLaserShootInterval = 0.35;
  float timeLastAlienFire;
  MysteryShip mysteryship;
  float mysteryShipSpawnInterval;
  float timeLastSpawn;
  void gameOver();
  void Reset();
  void initGame();
  void savedHighScoreToFile(int highscore);
  int loadHighScoreFromFile();
  Sound explosionSound;

};
