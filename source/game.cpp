/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include <sstream>
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "TextRenderer.h"


// Game-related State data
SpriteRenderer  *Renderer;
auto row = 10, col = 10;
Maze World(row, col);
auto sizeOfTile = 40;
TextRenderer  *Text;

int imposterPositionY;

bool CheckCollision(sprite &one, sprite &two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.position.x + one.size.x >= two.position.x &&
                      two.position.x + two.size.x >= one.position.x;
    // collision y-axis?
    bool collisionY = one.position.y + one.size.y >= two.position.y &&
                      two.position.y + two.size.y >= one.position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

Game::Game(unsigned int width, unsigned int height)
        : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer;
}



void Game::Init()
{
    srand(std::time(nullptr));
    World.Init();
    // load shaders
    ResourceManager::LoadShader("../source/shaders/shader.vert", "../source/shaders/shader.frag", nullptr, "shader");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("shader").Use().SetInteger("image", 0);
    ResourceManager::GetShader("shader").SetMatrix4("projection", projection);
    // set render-specific controls
    auto shader = ResourceManager::GetShader("shader");
    Renderer = new SpriteRenderer(shader);
    // load textures
    ResourceManager::LoadTexture("../assets/textures/imposter.png", true, "imposter");
    ResourceManager::LoadTexture("../assets/textures/tile.png", true, "tile");
    ResourceManager::LoadTexture("../assets/textures/deathButton.png", true, "deathButton");
    ResourceManager::LoadTexture("../assets/textures/powerupButton.png", true, "powerupButton");
    ResourceManager::LoadTexture("../assets/textures/coin.png", true, "coin");
    ResourceManager::LoadTexture("../assets/textures/bomb.png", true, "bomb");
    for(int i=0; i < 24; i++) {
        if (i < 10) {
            ResourceManager::LoadTexture("../assets/textures/leftPlayer/tile00" + std::to_string(i) + ".png", true,
                                         "leftPlayer/tile" + std::to_string(i));
            ResourceManager::LoadTexture("../assets/textures/rightPlayer/tile00" + std::to_string(i) + ".png", true,
                                         "rightPlayer/tile" + std::to_string(i));
        }
        else {
            ResourceManager::LoadTexture("../assets/textures/leftPlayer/tile0" + std::to_string(i) + ".png", true,
                                         "leftPlayer/tile" + std::to_string(i));
            ResourceManager::LoadTexture("../assets/textures/rightPlayer/tile0" + std::to_string(i) + ".png", true,
                                         "rightPlayer/tile" + std::to_string(i));
        }
    }
    for(int i=0; i< 12; i++) {
        if (i < 9) {
            ResourceManager::LoadTexture("../assets/textures/rightImposter/walkcolor000" + std::to_string(i+1) + ".png",
                                         true,
                                         "rightImposter/tile" + std::to_string(i));
            ResourceManager::LoadTexture("../assets/textures/leftImposter/walkcolor000" + std::to_string(i+1) + ".png",
                                         true,
                                         "leftImposter/tile" + std::to_string(i));
        }
        else {
            ResourceManager::LoadTexture("../assets/textures/rightImposter/walkcolor00" + std::to_string(i+1) + ".png", true,
                                         "rightImposter/tile" + std::to_string(i));
            ResourceManager::LoadTexture("../assets/textures/leftImposter/walkcolor00" + std::to_string(i+1) + ".png", true,
                                         "leftImposter/tile" + std::to_string(i));
        }
    }
    ResourceManager::LoadTexture("../assets/textures/rightPlayerStill.png", true, "rightPlayerStill");
    ResourceManager::LoadTexture("../assets/textures/leftPlayerStill.png", true, "leftPlayerStill");
    ResourceManager::LoadTexture("../assets/textures/rightImposterStill.png", true, "rightImposterStill");
    ResourceManager::LoadTexture("../assets/textures/gate.png", true, "gate");
    ResourceManager::LoadTexture("../assets/textures/youWin.png", true, "youWin");
    ResourceManager::LoadTexture("../assets/textures/gameOver2.png", true, "gameOver");
    ResourceManager::LoadTexture("../assets/textures/grassPath.png", true, "grass");

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("../assets/fonts/ocraext.TTF", 24);

    auto position = glm::vec2(0, 0);

    for(int i=0; i<World.M; i++, position.x=0, position.y+=sizeOfTile) {
        for (int j=0; j<World.N; j++, position.x+=sizeOfTile) {
            if(World.maze[i][j] == '#') {
                tiles.push_back(new Tile("tile", position, glm::vec2(sizeOfTile, sizeOfTile), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f)));
            }
            else if(World.maze[i][j] == 'E') {
                tiles.push_back(new Tile("gate", position, glm::vec2(sizeOfTile, sizeOfTile), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f)));
            } else if(World.maze[i][j] == ' ') {
                paths.push_back(new Tile("grass", position, glm::vec2(sizeOfTile, sizeOfTile), 0.0f, glm::vec3(0.95f, 0.95f, 0.95f)));
            }
        }
    }
    player = new Player("rightPlayerStill", glm::vec2(50, 50), glm::vec2(18, 26), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    for (int i = row; i < 2 * row; ++i) {
        if (World.maze[i][2*col-1] == ' '){
            imposterPositionY = i;
            break;
        }
    }
    imposter = new Imposter("rightImposterStill", glm::vec2(40*(2*row-1)+10, 40*(imposterPositionY)+8), glm::vec2(20, 24), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    deathButton = new Button("deathButton", glm::vec2(40*(2*row-1)+5, 50), glm::vec2(30, 30), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    powerupButton = new Button("powerupButton", glm::vec2(50, 40*(2*row-1) + 6), glm::vec2(25, 25), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    youWinText = new class Text("youWin", glm::vec2(Width/2-230.0f, Height/2- 190.0f), glm::vec2(450, 280), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    gameOverText = new class Text("gameOver", glm::vec2(Width/2-230.0f, Height/2- 130.0f), glm::vec2(450, 130), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}

void Game::moveImposter() {
    auto imposterCheckX = imposter->position.x + imposter->size.x/2 - sizeOfTile/2;
    auto imposterCheckY = imposter->position.y + imposter->size.y/2 - sizeOfTile/2;
    if (std::fmod(imposterCheckX, static_cast<decltype(imposterCheckX)>(1.0)) == 0.0 && std::fmod(imposterCheckY, static_cast<decltype(imposterCheckY)>(1.0)) == 0.0) {
        if ((int) imposterCheckX % sizeOfTile == 0 && (int) imposterCheckY % sizeOfTile == 0) {
            int direction;
            direction = World.shortestPathBFS(std::make_pair(imposter->position.y / 40, imposter->position.x / 40),
                                              std::make_pair(player->position.y / 40, player->position.x / 40));
            switch (direction) {
                case 0:
                    imposter->direction = RIGHT;
                    imposter->directionFacing = RIGHT;
                    imposter->animationFrame = 0;
                    break;
                case 1:
                    imposter->direction = DOWN;
                    break;
                case 2:
                    imposter->direction = LEFT;
                    imposter->directionFacing = LEFT;
                    imposter->animationFrame = 0;
                    break;
                case 3:
                    imposter->direction = UP;
                    break;
                default:
                    break;
            }
        }
    }
    imposter->move(0.125f, imposter->direction);
    imposter->moveAnimate(imposter->directionFacing);
}

void Game::playerPowerupButtonCollision() {
    if (CheckCollision(*powerupButton, *player)) {
        for(int i=0; i<World.M; i++) {
            for (int j=0; j<World.N; j++) {
                if(World.maze[i][j] == ' ') {
                    auto chance = ((double) rand() / (RAND_MAX));
                    if (chance<0.07f) {
                        auto chanceType = ((double) rand() / (RAND_MAX));
                        if (chanceType<0.5f) {
                            bombs.push_back(new ScoreChanger("bomb", glm::vec2(40 * j + 10, 40 * i + 10), glm::vec2(15, 15), 0.0f,
                                                     glm::vec3(1.0f, 1.0f, 1.0f)));
                        } else {
                            coins.push_back(new ScoreChanger("coin", glm::vec2(40 * j + 10, 40 * i + 10), glm::vec2(15, 15), 0.0f,
                                                             glm::vec3(1.0f, 1.0f, 1.0f)));
                        }
                    }
                }
            }
        }
        powerupButton->show = false;
        tasks++;
    }
}

void Game::playerDeathButtonCollision() {
    if (CheckCollision(*deathButton, *player)) {
        imposter->dead = true;
        deathButton->show = false;
        tasks++;
    }
}

void Game::playerCoinCollision() {
    auto iter = coins.begin();
    while (iter != coins.end())
    {
        if (CheckCollision(**iter, *player)) {
            iter = coins.erase(iter);
            score+=10;
        }
        else ++iter;
    }
}

void Game::playerBombCollision() {
    auto iter = bombs.begin();
    while (iter != bombs.end())
    {
        if (CheckCollision(**iter, *player)) {
            iter = bombs.erase(iter);
            score-=10;
        }
        else ++iter;
    }
}

void Game::playerImposterCollision() {
    if (CheckCollision(*imposter, *player)) {
        State=GAME_LOST;
        imposter->dead=true;
    }
}

void Game::checkTasksComplete() {
    if (tasks >= 2 && !endVisible) {
        auto itr = tiles.end();
        itr-=2;
        tiles.erase(itr);
        endVisible = true;
    }
}

void Game::reachedEnd() {
    if ((int)player->position.y/sizeOfTile == 2*row && (int)player->position.x/sizeOfTile == 2*col-1) {
        State = GAME_WIN;
    }
}

void Game::timeOver() {
    if(currentTime == time) {
        State = GAME_LOST;
    }
}

void Game::Update(float dt)
{
    if (!imposter->dead)
        moveImposter();
    if (deathButton->show)
        playerDeathButtonCollision();
    if (powerupButton->show)
        playerPowerupButtonCollision();
    if (!imposter->dead)
        playerImposterCollision();
    playerCoinCollision();
    playerBombCollision();
    if (tasks <= 2)
        checkTasksComplete();
    if (Light == "ON" and State == GAME_ACTIVE and currentTime != (int)glfwGetTime()) {
        score++;
    }
    if (Light == "OFF" and State == GAME_ACTIVE and currentTime != (int)glfwGetTime()) {
        score+=3;
    }
    reachedEnd();
    currentTime = (int)glfwGetTime();
    timeOver();
}

void Game::ProcessInput(float dt)
{
    bool moved = false;
    if (this->Keys[GLFW_KEY_LEFT]) {
        moved = movePlayer(LEFT, RIGHT);
    }
    else if (this->Keys[GLFW_KEY_RIGHT]) {
        moved = movePlayer(RIGHT, LEFT);
    }
    else if (this->Keys[GLFW_KEY_UP]) {
        moved = movePlayer(UP, DOWN);
    }
    else if (this->Keys[GLFW_KEY_DOWN]) {
        moved = movePlayer(DOWN, UP);
    }
    if (!moved)
    {
        player->stop();
    }
    if (this->Keys[GLFW_KEY_L]) {
        Light == "OFF" ? Light = "ON" : Light = "OFF";
        this->Keys[GLFW_KEY_L] = false;
    }
    if (this->Keys[GLFW_KEY_Q]) {
        State = GAME_LOST;
    }
}

void Game::Render()
{
    if (State == GAME_ACTIVE) {
        if (Light == "ON") {
            for (auto &tile : tiles) {
                tile->color = glm::vec3(1.0f);
                Renderer->DrawSprite(tile);
            }
            for (auto &path : paths) {
                path->color = glm::vec3(0.95f);
                Renderer->DrawSprite(path);
            }
            for (auto &coin : coins)
                Renderer->DrawSprite(coin);
            for (auto &bomb : bombs)
                Renderer->DrawSprite(bomb);
            if (deathButton->show)
                Renderer->DrawSprite(deathButton);
            if (powerupButton->show)
                Renderer->DrawSprite(powerupButton);
            if (!imposter->dead)
                Renderer->DrawSprite(imposter);
            Renderer->DrawSprite(player);
        } else {
            int currentRow = (int)player->position.y/sizeOfTile;
            int currentCol = (int)player->position.x/sizeOfTile;
            for (auto &tile : tiles){
                if (!(currentCol-1 <= (int)tile->position.x/sizeOfTile and (int)tile->position.x/sizeOfTile <= currentCol+1
                    and currentRow-1 <= (int)tile->position.y/sizeOfTile and (int)tile->position.y/sizeOfTile <= currentRow+1)) {
                    tile->color = glm::vec3(0.5f);
                } else {
                    tile->color = glm::vec3(1.0f);
                }
                Renderer->DrawSprite(tile);
            }
            for (auto &path : paths) {
                if (!(currentCol-1 <= (int)path->position.x/sizeOfTile and (int)path->position.x/sizeOfTile <= currentCol+1
                      and currentRow-1 <= (int)path->position.y/sizeOfTile and (int)path->position.y/sizeOfTile <= currentRow+1)) {
                    path->color = glm::vec3(0.75f);
                } else {
                    path->color = glm::vec3(0.95f);
                }
                Renderer->DrawSprite(path);
            }
            for (auto &coin : coins) {
                if ((currentCol-1 <= (int)coin->position.x/sizeOfTile and (int)coin->position.x/sizeOfTile <= currentCol+1
                      and currentRow-1 <= (int)coin->position.y/sizeOfTile and (int)coin->position.y/sizeOfTile <= currentRow+1)) {
                    Renderer->DrawSprite(coin);
                }
            }
            for (auto &bomb : bombs) {
                if ((currentCol - 1 <= (int) bomb->position.x / sizeOfTile and
                      (int) bomb->position.x / sizeOfTile <= currentCol + 1
                      and currentRow - 1 <= (int) bomb->position.y / sizeOfTile and
                      (int) bomb->position.y / sizeOfTile <= currentRow + 1)) {
                    Renderer->DrawSprite(bomb);
                }
            }
            if (deathButton->show) {
                if ((currentCol-1 <= (int)deathButton->position.x/sizeOfTile and (int)deathButton->position.x/sizeOfTile <= currentCol+1
                      and currentRow-1 <= (int)deathButton->position.y/sizeOfTile and (int)deathButton->position.y/sizeOfTile <= currentRow+1)) {
                    Renderer->DrawSprite(deathButton);
                }
            }
            if (powerupButton->show) {
                if ((currentCol - 1 <= (int) powerupButton->position.x / sizeOfTile and
                      (int) powerupButton->position.x / sizeOfTile <= currentCol + 1
                      and currentRow - 1 <= (int) powerupButton->position.y / sizeOfTile and
                      (int) powerupButton->position.y / sizeOfTile <= currentRow + 1)) {
                    Renderer->DrawSprite(powerupButton);
                }
            }
            if (!imposter->dead) {
                if ((currentCol - 1 <= (int) imposter->position.x / sizeOfTile and
                      (int) imposter->position.x / sizeOfTile <= currentCol + 1
                      and currentRow - 1 <= (int) imposter->position.y / sizeOfTile and
                      (int) imposter->position.y / sizeOfTile <= currentRow + 1)) {
                    Renderer->DrawSprite(imposter);
                }
            }
            Renderer->DrawSprite(player);
        }
        std::stringstream scoreText, taskText, LightText, Time;
        scoreText << score;
        taskText << tasks;
        Time << time - (int) glfwGetTime();
        LightText << Light;
        Text->RenderText("Score:" + scoreText.str(), 850.0f, 5.0f, 0.8f);
        Text->RenderText("Tasks:" + taskText.str() + "/ 2", 850.0f, 22.0f, 0.8f);
        Text->RenderText("Lights:" + LightText.str(), 850.0f, 39.0f, 0.8f);
        Text->RenderText("Time:" + Time.str() + "s", 850.0f, 56.0f, 0.8f);
    } else {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        std::stringstream scoreText;
        scoreText << score;
        Text->RenderText("Score:" + scoreText.str(), Width / 2 - 90.0f, Height / 2, 1.5f);
        if  (State == GAME_LOST) {
            Renderer->DrawSprite(gameOverText);
        } else if (State == GAME_WIN)
            Renderer->DrawSprite(youWinText);
    }
}

bool Game::movePlayer(movement check, movement revert) {
    float value = 0.275f;
    bool animate = true;
    player->move(value, check);
    if (player->position.x < 0 or player->position.y < 0 or player->position.x > Width or player->position.y > Height)
        player->move(value, revert);
    for(auto & tile: tiles)
    {
        if (CheckCollision(*tile, *player)) {
            player->move(value, revert);
            animate = false;
            break;
        }
    }
    if (animate) {
        if(check == LEFT) {
            player->directionFacing = LEFT;
        }
        else if (check == RIGHT) {
            player->directionFacing = RIGHT;
        }
        player->moveAnimate(player->directionFacing);
        return true;
    }
    return false;
}
