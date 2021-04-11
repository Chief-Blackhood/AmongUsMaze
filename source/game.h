/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H

#include <vector>
#include <iostream>
#include <ctime>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Maze.h"
#include "Player.h"
#include "Tile.h"
#include "Imposter.h"
#include "Button.h"
#include "ScoreChanger.h"
#include "Text.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_LOST
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState                  State;
    int                        currentTime;
    bool                       Keys[1024];
    unsigned int               Width, Height;
    unsigned int               tasks = 0;
    int                        time = 90;
    std::string                Light = "ON";
    bool                       endVisible = false;
    Player*                    player;
    Imposter*                  imposter;
    Button*                    deathButton;
    Button*                    powerupButton;
    Text*                      youWinText;
    Text*                      gameOverText;
    std::vector<Tile*>         tiles, paths;
    std::vector<ScoreChanger*> bombs;
    std::vector<ScoreChanger*> coins;
    int                        score = 0;

    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    bool movePlayer(movement check, movement revert);
    void moveImposter();

    void playerDeathButtonCollision();
    void playerImposterCollision();
    void playerPowerupButtonCollision();
    void playerBombCollision();
    void playerCoinCollision();

    void checkTasksComplete();

    void reachedEnd();

    void timeOver();
};

#endif