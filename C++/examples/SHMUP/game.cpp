#include "pico/rand.h"
#include "pico/time.h"
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
    MENU, PLAY, LIFE_LOST, WIN, GAME_OVER,
};

static const uint8_t scrollSpeed = 15;   // px/sec

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

static const uint8_t startingLives = 3;
static const uint8_t startingHealth = 10;

static const uint8_t playerProjectileVel = 160;  // px/sec

int starshipX, starshipY;
int starshipVx, starshipVy;

uint8_t score;
uint8_t lives;
uint8_t health;

float currentTime;

struct Vec2D
{
    int x, y;
};

static constexpr uint16_t numStars{500};
static constexpr uint16_t numBigStars{100};
static constexpr uint16_t numDistantStars{100};
Vec2D stars[numStars];

struct Projectile
{
    Vec2D pos;
    bool alive = true;
};

std::vector<Projectile> playerProjectiles;
std::vector<Projectile> enemyProjectiles;

struct Particle
{
    Vec2D pos;
    Vec2D vel;
    float lifetime;
    float duration = 0.0f;
    bool alive = true;
};

std::list<Particle> particles;

static const uint8_t numParticlesPerExplosion = 50;
static const float particlesSpeedMean = 10.0f;
static const float particlesLifeTimeMean = 50.0f;

struct EnemyDef
{
    enum Sprite
    {
        ENEMY1 = 0, ENEMY2, ENEMY3, BOMB,
    } sprite;

    float triggerTime;
    uint8_t offset;
    uint8_t speed;
    float health = 0.0f;
    uint8_t fireRate = 0;
};

struct Enemy
{
    EnemyDef def;
    Vec2D pos;   

    float elapsed = 0.0f;

    void update(float deltaTimeSec)
    {
        pos.y += def.speed * deltaTimeSec;

        elapsed += deltaTimeSec;

        if (def.fireRate && elapsed >= 1.0f / def.fireRate)
        {
            elapsed = 0.0f;

            uint8_t spriteWidth, spriteHeight;
            switch (def.sprite)
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

            enemyProjectiles.push_back({{pos.x + spriteWidth / 2, pos.y + spriteHeight}});
        }
    } 
};

std::list<EnemyDef> enemyDefs;
std::list<Enemy> enemies;

void updateMenu(float deltaTimeSec);
void renderMenu();

void updatePlay(float deltaTimeSec);
void renderPlay();

void updateLifeLost(float deltaTimeSec);
void renderLifeLost();

void updateWin(float deltaTimeSec);
void renderWin();

void updateGameOver(float deltaTimeSec);
void renderGameOver();

GameState gameState = GameState::MENU;

int main()
{
    absolute_time_t previousTime = get_absolute_time();

    while (true)
    {
        // calculate frame time
        absolute_time_t currentTime = get_absolute_time();
        uint64_t deltaTimeUs = absolute_time_diff_us(previousTime, currentTime);
        float deltaTimeMs = deltaTimeUs / 1000.0f;
        float deltaTimeSec = deltaTimeMs / 1000.0f;
        previousTime = currentTime;

        // update state 
        switch (gameState)
        {
            case GameState::MENU:
                updateMenu(deltaTimeSec);
                renderMenu();
                break;

            case GameState::PLAY:
                updatePlay(deltaTimeSec);
                renderPlay();
                break;

            case GameState::LIFE_LOST:
                updateLifeLost(deltaTimeSec);
                renderLifeLost();
                break;

            case GameState::WIN:
                updateWin(deltaTimeSec);
                renderWin();
                break;

            case GameState::GAME_OVER:
                updateGameOver(deltaTimeSec);
                renderGameOver();
        }
    }
}

void updateMenu(float deltaTimeSec)
{
    // start game
    if (sensei.getButtonState(A) == JUST_PRESSED)
    {
        for (auto &star : stars)
        {
            star.x = get_rand_32() % DISPLAY_WIDTH;
            star.y = get_rand_32() % DISPLAY_HEIGHT;
        }
    
        score = 0;
        lives = startingLives;
        health = startingHealth;
        currentTime = 0.0f;

        enemies.clear();
        enemyDefs.clear();
        enemyDefs.push_back({EnemyDef::ENEMY1, 1, 120, 50, 100.0f, 2});
        enemyDefs.push_back({EnemyDef::ENEMY2, 3, 100, 100, 100.0f, 1});
        enemyDefs.push_back({EnemyDef::ENEMY3, 4, 80, 80, 100.0f, 3});
        enemyDefs.push_back({EnemyDef::BOMB, 1, 140, scrollSpeed * 3, 100.0f, 0});
        enemyDefs.push_back({EnemyDef::ENEMY1, 6, 200, 50, 100.0f, 4});
        enemyDefs.push_back({EnemyDef::ENEMY2, 7, 10, 100, 100.0f, 1});
        enemyDefs.push_back({EnemyDef::ENEMY3, 8, 180, 30, 100.0f, 2});
        enemyDefs.push_back({EnemyDef::BOMB, 8, 20, scrollSpeed * 3, 100.0f, 0});
        enemyDefs.push_back({EnemyDef::ENEMY3, 9, 180, 30, 100.0f, 2});
        enemyDefs.push_back({EnemyDef::BOMB, 10, 20, scrollSpeed * 3, 100.0f, 0});
                    
        starshipX = {(DISPLAY_WIDTH - starshipWidth) / 2};
        starshipY = {DISPLAY_HEIGHT - starshipHeight - 1};

        gameState = GameState::PLAY;
    }
}

void renderMenu()
{
    sensei.clearScreen(0x00, 0x00, 0x00);

    sensei.drawSprite(menu2, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

    sensei.presentScreen();
}

void updatePlay(float deltaTimeSec)
{
    // exit to menu
    if (sensei.getButtonState(B) == JUST_PRESSED)
    {
        gameState = GameState::MENU;
        return;
    }

    currentTime += deltaTimeSec;

    // update background
    for (int i = 0; i < numStars; i++)
    {
        if (i < numBigStars)
            stars[i].y += scrollSpeed * 3 * deltaTimeSec;
        else if (i < numBigStars + numDistantStars)
            stars[i].y += scrollSpeed * deltaTimeSec;
        else
            stars[i].y += scrollSpeed * 2 * deltaTimeSec;

        if (stars[i].y >= DISPLAY_HEIGHT)
        {
            stars[i].x = get_rand_32() % DISPLAY_WIDTH;
            stars[i].y = 0;
        }
    }

    // spawn enemies
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

    // update enemies
    for (auto &enemy : enemies)
    {
        enemy.update(deltaTimeSec);

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
                    gameState = GameState::LIFE_LOST;
                else 
                    gameState = GameState::GAME_OVER;
            }         
        }

        // remove off-screen and dead enemies
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy &enemy){ return enemy.pos.y >= DISPLAY_HEIGHT || enemy.def.health <= 0.0f;}), enemies.end());

        // update particles
        for (auto &particle : particles)
        {
            particle.pos.x += particle.vel.x;
            particle.pos.y += particle.vel.y;

            particle.duration += 5.0f;

            if (particle.duration >= particle.lifetime)
                particle.alive = false;
        }

        // remove dead particles
        particles.erase(std::remove_if(particles.begin(), particles.end(), [](const Particle &particle){ return !particle.alive;}), particles.end());

        // update player's projectiles
        for (auto &projectile : playerProjectiles)
        {
            projectile.pos.y -= playerProjectileVel * deltaTimeSec;

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

                    for (int i = 0; i < numParticlesPerExplosion; i++)
                    {   
                        float speed = (float)rand() / RAND_MAX * particlesSpeedMean;  
                        float angle = (float)rand() / RAND_MAX * 2.0f * 3.14f; 
                        float lifetime = (float)get_rand_32() / UINT32_MAX * particlesLifeTimeMean;
                        particles.push_back({{enemy.pos.x + spriteWidth / 2, enemy.pos.y + spriteHeight / 2}, speed * cos(angle), speed * sin(angle), lifetime});
                    }
                }
            }

            if (projectile.pos.y < 0)
                projectile.alive = false;
        }

        // remove off-screen and collided projectiles
        playerProjectiles.erase(std::remove_if(playerProjectiles.begin(), playerProjectiles.end(), [](const Projectile &p){ return !p.alive; }), playerProjectiles.end()); 

        // update enemies' projectiles
        for (auto &projectile : enemyProjectiles)
        {
            projectile.pos.y += playerProjectileVel * deltaTimeSec;

            if (projectile.pos.x >= starshipX && projectile.pos.x <= starshipX + starshipWidth && projectile.pos.y >= starshipY && projectile.pos.y <= starshipY + starshipHeight)
            {
                projectile.alive = false;
                
                sensei.playNote(600.f, 300, 1.0f, false, 100.0f);
                
                health--;

                if (health == 0)
                {
                    lives--;

                    if (lives)
                        gameState = GameState::LIFE_LOST;
                    else 
                        gameState = GameState::GAME_OVER;
                }

            }

            if (projectile.pos.x < 0 || projectile.pos.x >= DISPLAY_WIDTH || projectile.pos.y < 0 || projectile.pos.y >= DISPLAY_HEIGHT)
                projectile.alive = false;
        }

        // remove off-screen and collided projectiles
        enemyProjectiles.erase(std::remove_if(enemyProjectiles.begin(), enemyProjectiles.end(), [](const Projectile &p){ return !p.alive; }), enemyProjectiles.end());

        // process player input
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
            playerProjectiles.push_back({starshipX + starshipWidth / 2, starshipY + 10});
            sensei.playNote(440.f, 100, 1.0f, true, 100.0f);
        }

        // update player position
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

void renderPlay()
{
        sensei.clearScreen(0x00, 0x00, 0x00);

        // render background
        for (int i = 0; i < numStars /*sizeof(stars) / sizeof(Vec2D)*/; i++ )
        {
            if (i < numBigStars)
            {
                sensei.drawPixel(stars[i].x, stars[i].y, 0xFF, 0xFF, 0xFF);
                sensei.drawPixel(stars[i].x + 1, stars[i].y, 0xFF, 0xFF, 0xFF);
                sensei.drawPixel(stars[i].x, stars[i].y + 1, 0xFF, 0xFF, 0xFF);
                sensei.drawPixel(stars[i].x + 1, stars[i].y + 1, 0xFF, 0xFF, 0xFF);
            }  
            else if (i < numBigStars + numDistantStars)
                sensei.drawPixel(stars[i].x, stars[i].y, 0x1F, 0x1F, 0x1F);
            else
                sensei.drawPixel(stars[i].x, stars[i].y, 0xFF, 0xFF, 0xFF);
        }

        // render player projectiles
        for (auto &projectile : playerProjectiles)
        {
            sensei.drawPixel(projectile.pos.x, projectile.pos.y, 0x00, 0xFF, 0x00);
            sensei.drawPixel(projectile.pos.x + 1, projectile.pos.y, 0x00, 0xFF, 0x00);
            sensei.drawPixel(projectile.pos.x, projectile.pos.y + 1, 0x00, 0xFF, 0x00);
            sensei.drawPixel(projectile.pos.x + 1, projectile.pos.y + 1, 0x00, 0xFF, 0x00);
            sensei.drawPixel(projectile.pos.x -1 , projectile.pos.y, 0x00, 0xFF, 0x00);
            sensei.drawPixel(projectile.pos.x, projectile.pos.y - 1, 0x00, 0xFF, 0x00);
            sensei.drawPixel(projectile.pos.x - 1, projectile.pos.y - 1, 0x00, 0xFF, 0x00);
        }

        // render enemy projectiles
        for (auto &projectile : enemyProjectiles)
        {
            sensei.drawPixel(projectile.pos.x, projectile.pos.y, 0xFF, 0x00, 0x00);
            sensei.drawPixel(projectile.pos.x + 1, projectile.pos.y, 0xFF, 0x00, 0x00);
            sensei.drawPixel(projectile.pos.x, projectile.pos.y + 1, 0xFF, 0x00, 0x00);
            sensei.drawPixel(projectile.pos.x + 1, projectile.pos.y + 1, 0x00, 0xFF, 0x00);
            sensei.drawPixel(projectile.pos.x -1 , projectile.pos.y, 0xFF, 0x00, 0x00);
            sensei.drawPixel(projectile.pos.x, projectile.pos.y - 1, 0xFF, 0x00, 0x00);
            sensei.drawPixel(projectile.pos.x - 1, projectile.pos.y - 1, 0xFF, 0x00, 0x00);
        }

        // render player
        sensei.drawSprite(starship_idle, starshipX, starshipY, starshipWidth, starshipHeight);

        // render enemies
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

        // render particles
        for (auto &particle : particles)
        {
            sensei.drawPixel(particle.pos.x, particle.pos.y, 0xFF - particle.duration, 0xAA, 0x00);
            sensei.drawPixel(particle.pos.x + 1, particle.pos.y, 0xFF - particle.duration, 0xAA, 0x00);
            sensei.drawPixel(particle.pos.x, particle.pos.y + 1, 0xFF - particle.duration, 0xAA, 0x00);
            sensei.drawPixel(particle.pos.x + 1, particle.pos.y + 1, 0xFF - particle.duration, 0xAA, 0x00);
            sensei.drawPixel(particle.pos.x - 1, particle.pos.y, 0xFF - particle.duration, 0xAA, 0x00);
            sensei.drawPixel(particle.pos.x, particle.pos.y - 1, 0xFF - particle.duration, 0xAA, 0x00);
            sensei.drawPixel(particle.pos.x - 1, particle.pos.y - 1, 0xFF - particle.duration, 0xAA, 0x00);
        }

        // render HUD
        sensei.setTextFont(FREE_SANS_BOLD, SMALL);
        sensei.setTextColor(0xFF, 0x00, 0x00);
        sensei.print(10, 10, (std::string("score: ") + std::to_string(score)).c_str());
        sensei.print(10, 30, (std::string("health: ") + std::to_string(health)).c_str());
        sensei.print(10, 50, (std::string("lives: ") + std::to_string(lives)).c_str());

        sensei.presentScreen();
}

#define LIFE_LOST_SCREEN_TIME 50.0f

void updateLifeLost(float deltaTimeSec)
{
    static float lifeLostTime = 0.0f;

    lifeLostTime += 1.0f;

    if (lifeLostTime >= LIFE_LOST_SCREEN_TIME)
    {
        lifeLostTime = 0.0f;

        gameState = GameState::PLAY;

        health = startingHealth;

        playerProjectiles.clear();
        enemyProjectiles.clear();
        particles.clear();

        enemies.clear();
        enemyDefs.clear();
        enemyDefs.push_back({EnemyDef::ENEMY1, 1, 120, 50, 100.0f, 2});
        enemyDefs.push_back({EnemyDef::ENEMY2, 3, 100, 100, 100.0f, 1});
        enemyDefs.push_back({EnemyDef::ENEMY3, 4, 80, 80, 100.0f, 3});
        enemyDefs.push_back({EnemyDef::BOMB, 1, 140, scrollSpeed * 3, 100.0f, 0});
        enemyDefs.push_back({EnemyDef::ENEMY1, 6, 200, 50, 100.0f, 4});
        enemyDefs.push_back({EnemyDef::ENEMY2, 7, 10, 100, 100.0f, 1});
        enemyDefs.push_back({EnemyDef::ENEMY3, 8, 180, 30, 100.0f, 2});
        enemyDefs.push_back({EnemyDef::BOMB, 8, 20, scrollSpeed * 3, 100.0f, 0});
        enemyDefs.push_back({EnemyDef::ENEMY3, 9, 180, 30, 100.0f, 2});
        enemyDefs.push_back({EnemyDef::BOMB, 10, 20, scrollSpeed * 3, 100.0f, 0});
        
        currentTime = 0.0f;

        score = 0;
    }
}

void renderLifeLost()
{
    sensei.clearScreen(0x00, 0x00, 0x00);

    //sensei.drawSprite(destroyed, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

    sensei.presentScreen();
}

void updateWin(float deltaTimeSec)
{

}

void renderWin()
{

}

void updateGameOver(float deltaTimeSec)
{
        if (sensei.getButtonState(A) == JUST_PRESSED)
                    gameState = GameState::MENU;
}

void renderGameOver()
{
    sensei.clearScreen(0x00, 0x00, 0x00);

    //sensei.drawSprite(game_over, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

    sensei.presentScreen();
}



