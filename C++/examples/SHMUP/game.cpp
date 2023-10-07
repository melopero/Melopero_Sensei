#include "pico/rand.h"
#include "../../src/MeloperoSensei.hpp"
#include "sprites.h"
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <cmath>
#include <limits>

MeloperoSensei sensei;

enum class GameState 
{
    MENU, PLAY, LIFE_LOST, GAME_OVER,
};

static const float scrollSpeed = 2.0f;

static const uint starshipWidth = 36;
static const uint starshipHeight = 33;

static const uint enemy1Width = 32;
static const uint enemy1Height = 42;

static const uint enemy2Width = 35;
static const uint enemy2Height = 27;

static const uint enemy3Width = 36;
static const uint enemy3Height = 35;

static const uint bombWidth = 23;
static const uint bombHeight = 23;

int starshipX{(DISPLAY_WIDTH - starshipWidth) / 2}, starshipY{DISPLAY_HEIGHT - starshipHeight - 1};
int starshipVx{0}, starshipVy{0};

uint16_t score = 0;
int lives = 3;
float currentTime = 0.0f;

struct Pos2d
{
    int x, y;
};

Pos2d stars[500];

struct Projectile
{
    Pos2d pos;
    bool alive = true;
};

std::vector<Projectile> projectiles;
std::vector<Projectile> enemyProjectiles;

struct Particle
{
    Pos2d pos;
    int vx, vy;
    float lifetime;
    float duration = 0.0f;
    bool alive = true;
};

std::list<Particle> particles;

struct EnemyDef
{
    enum Sprite
    {
        ENEMY1 = 0, ENEMY2, ENEMY3, BOMB,
    } sprite;

    float triggerTime;
    uint8_t offset;
    float health = 0.0f;
};

struct Enemy
{
    EnemyDef def;
    Pos2d pos;   

    void update()
    {
        pos.y += scrollSpeed * 3.0f;
    } 
};

std::list<EnemyDef> enemyDefs;
std::list<Enemy> enemies;

void updateMenu();
void renderMenu();

void updatePlay();
void renderPlay();

void updateLifeLost();
void renderLifeLost();

void updateGameOver();
void renderGameOver();

GameState gameState = GameState::MENU;

int main()
{

    while (true)
    {
        // calculate frame time


        // update state 
        switch (gameState)
        {
            case GameState::MENU:
                updateMenu();
                renderMenu();
                break;

            case GameState::PLAY:
                updatePlay();
                renderPlay();
                break;

            case GameState::LIFE_LOST:
                updateLifeLost();
                renderLifeLost();
                break;

            case GameState::GAME_OVER:
                updateGameOver();
                renderGameOver();
        }
    }
}

float lifeLostTime = 0.0f;

void updateLifeLost()
{
    lifeLostTime += 1.0f;

    if (lifeLostTime >= 50.0f)
    {
        gameState = GameState::PLAY;

        enemies.clear();
        enemyDefs.clear();
        enemyDefs.push_back({EnemyDef::ENEMY1, 0, 120, 100.0f});
        enemyDefs.push_back({EnemyDef::ENEMY2, 50, 100, 100.0f});
        enemyDefs.push_back({EnemyDef::ENEMY3, 80, 80, 100.0f});
        enemyDefs.push_back({EnemyDef::BOMB, 100, 140, 100.0f});

        currentTime = 0.0f;
    }
}

void renderLifeLost()
{
    sensei.clearScreen(0x00, 0x00, 0x00);

    sensei.print(100, 120, "try again", 0xFF, 0xFF, 0xFF);

    sensei.presentScreen();
}

void updateGameOver()
{
        if (sensei.getButtonState(A) == JUST_PRESSED)
                    gameState = GameState::MENU;
}

void renderGameOver()
{
    sensei.clearScreen(0x00, 0x00, 0x00);

    sensei.print(100, 110, "EARTH IS LOST!", 0xFF, 0xFF, 0xFF);
    sensei.print(100, 120, "GAME OVER", 0xFF, 0xFF, 0xFF);
    sensei.print(50, 130, "press \"A\" to play again", 0xFF, 0xFF, 0xFF);

    sensei.presentScreen();
}

void updateMenu()
{
    if (sensei.getButtonState(A) == JUST_PRESSED)
                {
                    for (auto &star : stars)
                    {
                        star.x = get_rand_32() % DISPLAY_WIDTH;
                        star.y = get_rand_32() % DISPLAY_HEIGHT;
                    }

                    currentTime = 0.0f;
                    
                    enemies.clear();
                    enemyDefs.clear();
                    enemyDefs.push_back({EnemyDef::ENEMY1, 0, 120, 100.0f});
                    enemyDefs.push_back({EnemyDef::ENEMY2, 50, 100, 100.0f});
                    enemyDefs.push_back({EnemyDef::ENEMY3, 80, 80, 100.0f});
                    enemyDefs.push_back({EnemyDef::BOMB, 100, 140, 100.0f});
                    
                    starshipX = {(DISPLAY_WIDTH - starshipWidth) / 2};
                    starshipY = {DISPLAY_HEIGHT - starshipHeight - 1};

                    lives = 3;

                    gameState = GameState::PLAY;
                }
}

void updatePlay()
{
    if (sensei.getButtonState(B) == JUST_PRESSED)
        gameState = GameState::MENU;

    currentTime += 1.0f;

    // update stars background
    for (auto &star : stars)
    {
        star.y += scrollSpeed;

        if (star.y >= DISPLAY_HEIGHT)
        {
            star.x = get_rand_32() % DISPLAY_WIDTH;
            star.y = 0;
        }
    }

    while (!enemyDefs.empty() && currentTime >= enemyDefs.front().triggerTime)
    {
        uint8_t spriteWidth, spriteHeight;
        switch (enemyDefs.front().sprite)
        {
                
            case EnemyDef::ENEMY1:
            spriteWidth = enemy1Width;
            spriteHeight = enemy1Height;
            break;

            case EnemyDef::ENEMY2:
                spriteWidth = enemy2Width;
                spriteHeight = enemy2Height;
                break;

            case EnemyDef::ENEMY3:
                spriteWidth = enemy3Width;
                spriteHeight = enemy3Height;
                break;

            case EnemyDef::BOMB:
                spriteWidth = bombWidth;
                spriteHeight = bombHeight;
                break;
        }

        enemies.push_back({enemyDefs.front(), {enemyDefs.front().offset, 0 - spriteHeight}});
        enemyDefs.pop_front();
    }

    for (auto &enemy : enemies)
    {
        enemy.update();

        uint8_t spriteWidth, spriteHeight;
        switch (enemyDefs.front().sprite)
        {
            case EnemyDef::ENEMY1:
                spriteWidth = enemy1Width;
                spriteHeight = enemy1Height;
                break;

            case EnemyDef::ENEMY2:
                spriteWidth = enemy2Width;
                spriteHeight = enemy2Height;
                break;

            case EnemyDef::ENEMY3:
                spriteWidth = enemy3Width;
                spriteHeight = enemy3Height;
                break;

            case EnemyDef::BOMB:
                spriteWidth = bombWidth;
                spriteHeight = bombHeight;
                break;
        }

            if ((enemy.pos.x >= starshipX && enemy.pos.x <= starshipX + starshipWidth || enemy.pos.x + spriteWidth >= starshipX && enemy.pos.x + spriteWidth <= starshipX + starshipWidth) && (enemy.pos.y >= starshipY && enemy.pos.y <= starshipY + starshipHeight || enemy.pos.y + spriteHeight >= starshipY && enemy.pos.y + spriteHeight <= starshipY + starshipHeight))
            {
                enemy.def.health = 0.0f;
                sensei.playNote(600.f, 300, 1.0f, false, 100.0f);
                lives--;

                if (lives)
                {
                    lifeLostTime = 0.0f;
                    gameState = GameState::LIFE_LOST;
                }
                else 
                    gameState = GameState::GAME_OVER;
            }         
        }

        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy &enemy){ return enemy.pos.y >= DISPLAY_HEIGHT || enemy.def.health <= 0.0f;}), enemies.end());

        // update particles
        for (auto &particle : particles)
        {
            particle.pos.x += particle.vx;
            particle.pos.y += particle.vy;

            particle.duration += 5.0f;

            if (particle.duration >= particle.lifetime)
                particle.alive = false;
        }

        particles.erase(std::remove_if(particles.begin(), particles.end(), [](const Particle &particle){ return !particle.alive;}), particles.end());


        // update player's projectiles
        for (auto &projectile : projectiles)
        {
            projectile.pos.y -= 5;

            for (auto &enemy : enemies)
            {
                uint8_t spriteWidth, spriteHeight;
                switch (enemy.def.sprite)
                {

                    case EnemyDef::ENEMY1:
                        spriteWidth = enemy1Width;
                        spriteHeight = enemy1Height;
                        break;

                    case EnemyDef::ENEMY2:
                        spriteWidth = enemy2Width;
                        spriteHeight = enemy2Height;
                        break;

                    case EnemyDef::ENEMY3:
                        spriteWidth = enemy3Width;
                        spriteHeight = enemy3Height;
                        break;

                    case EnemyDef::BOMB:
                        spriteWidth = bombWidth;
                        spriteHeight = bombHeight;
                        break;
                }

                // check for collision with enemy
                if (projectile.pos.x >= enemy.pos.x && projectile.pos.x <= enemy.pos.x + spriteWidth && projectile.pos.y >= enemy.pos.y && projectile.pos.y <= enemy.pos.y + spriteHeight)
                {
                    projectile.alive = false;
                    enemy.def.health = 0.0f;
                    sensei.playNote(440.f, 100, 1.0f, false, 100.0f);
                    score += 10;

                    for (int i = 0; i < 50; i++)
                    {   
                        float speed = (float)rand() / RAND_MAX * 10.0f;
                        float angle = (float)rand() / RAND_MAX * 2.0f * 3.14f; 
                        float lifetime = (float)get_rand_32() / UINT32_MAX * 50.0f;
                        particles.push_back({{enemy.pos.x + spriteWidth / 2, enemy.pos.y + spriteHeight / 2}, speed * cos(angle), speed * sin(angle), lifetime});
                    }
                }
            }

            if (projectile.pos.y < 0)
                projectile.alive = false;
        }

        // remove off-screen and collided bullets
        projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](const Projectile &p){ return !p.alive; }), projectiles.end()); 

        // update enemies' projectiles
        for (auto &enemyProjectile : enemyProjectiles)
        {
            

            if (enemyProjectile.pos.x < 0 || enemyProjectile.pos.x >= DISPLAY_WIDTH || enemyProjectile.pos.y < 0 || enemyProjectile.pos.y >= DISPLAY_HEIGHT)
                enemyProjectile.alive = false;
        }

        // remove off-screen bullets and collided bullets
        enemyProjectiles.erase(std::remove_if(enemyProjectiles.begin(), enemyProjectiles.end(), [](const Projectile &p){ return !p.alive; }), enemyProjectiles.end());

        starshipVx = starshipVy = 0;

        if (sensei.getButtonState(RIGHT) == PRESSED)
            starshipVx += 4;
        if (sensei.getButtonState(LEFT) == PRESSED)
            starshipVx -= 4;
        if (sensei.getButtonState(UP) == PRESSED)
            starshipVy -= 4;
        if (sensei.getButtonState(DOWN) == PRESSED)
            starshipVy += 4;
        if (sensei.getButtonState(A) == JUST_PRESSED)
        {
            projectiles.push_back({starshipX + starshipWidth / 2, starshipY + 10});
            sensei.playNote(440.f, 100, 1.0f, true, 100.0f);
        }
        if (sensei.getButtonState(B) == JUST_PRESSED)
            ;

        starshipX += starshipVx;
        starshipY += starshipVy;

        if (starshipX <= 0)
            starshipX = 0;
        
        if (starshipX + starshipWidth >= DISPLAY_WIDTH)
            starshipX = DISPLAY_WIDTH - starshipWidth;

        if (starshipY <= 0)
            starshipY = 0;

        if (starshipY + starshipHeight >= DISPLAY_HEIGHT)
            starshipY = DISPLAY_HEIGHT - starshipHeight;
}

void renderMenu()
{
    sensei.clearScreen(0x00, 0x00, 0x00);

    sensei.drawSprite(menu, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

    sensei.presentScreen();
}

void renderPlay()
{
        sensei.clearScreen(0x00, 0x00, 0x00);

        for (int i = 0; i < sizeof(stars) / sizeof(Pos2d); i++ )
        {
            if (i < 100)
                sensei.drawPixel(stars[i].x, stars[i].y, 0x1F, 0x1F, 0x1F);
            else if (i < 200)
            {
                sensei.drawPixel(stars[i].x, stars[i].y, 0xFF, 0xFF, 0xFF);
                sensei.drawPixel(stars[i].x + 1, stars[i].y, 0xFF, 0xFF, 0xFF);
                sensei.drawPixel(stars[i].x, stars[i].y + 1, 0xFF, 0xFF, 0xFF);
                sensei.drawPixel(stars[i].x + 1, stars[i].y + 1, 0xFF, 0xFF, 0xFF);
            }
            else
                sensei.drawPixel(stars[i].x, stars[i].y, 0xFF, 0xFF, 0xFF);
        }

        for (auto &projectile : projectiles)
        {
            sensei.drawPixel(projectile.pos.x, projectile.pos.y, 0x00, 0xFF, 0x00);
            sensei.drawPixel(projectile.pos.x + 1, projectile.pos.y, 0x00, 0xFF, 0x00);
            sensei.drawPixel(projectile.pos.x, projectile.pos.y + 1, 0x00, 0xFF, 0x00);
            sensei.drawPixel(projectile.pos.x + 1, projectile.pos.y + 1, 0x00, 0xFF, 0x00);
            sensei.drawPixel(projectile.pos.x -1 , projectile.pos.y, 0x00, 0xFF, 0x00);
            sensei.drawPixel(projectile.pos.x, projectile.pos.y - 1, 0x00, 0xFF, 0x00);
            sensei.drawPixel(projectile.pos.x - 1, projectile.pos.y - 1, 0x00, 0xFF, 0x00);
        }

        sensei.drawSprite(starship_idle, starshipX, starshipY, starshipWidth, starshipHeight);

        for (auto &enemy : enemies)
        {
            switch (enemy.def.sprite)
            {
                case EnemyDef::ENEMY1:
                    sensei.drawSprite(enemy1small, enemy.pos.x, enemy.pos.y, enemy1Width, enemy1Height);
                    break;

                case EnemyDef::ENEMY2:
                    sensei.drawSprite(enemy2small, enemy.pos.x, enemy.pos.y, enemy2Width, enemy2Height);
                    break;

                case EnemyDef::ENEMY3:
                    sensei.drawSprite(enemy3small, enemy.pos.x, enemy.pos.y, enemy3Width, enemy3Height);
                    break;

                case EnemyDef::BOMB:
                    sensei.drawSprite(bombSmall, enemy.pos.x, enemy.pos.y, bombWidth, bombHeight);
                    break;
            }
        }

        for (auto &particle : particles)
        {
            sensei.drawPixel(particle.pos.x, particle.pos.y, 0xFF, 0xAA, 0x00);
            sensei.drawPixel(particle.pos.x + 1, particle.pos.y, 0xFF, 0xAA, 0x00);
            sensei.drawPixel(particle.pos.x, particle.pos.y + 1, 0xFF, 0xAA, 0x00);
            sensei.drawPixel(particle.pos.x + 1, particle.pos.y + 1, 0xFF, 0xAA, 0x00);
        }

        sensei.print(10, 10, (std::string("score: ") + std::to_string(score)).c_str(), 0xFF, 0x00, 0x00);
        sensei.print(180, 10, (std::string("lives: ") + std::to_string(lives)).c_str(), 0xFF, 0x00, 0x00);

        sensei.presentScreen();
}