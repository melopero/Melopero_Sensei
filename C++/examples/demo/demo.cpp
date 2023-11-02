#include "../../src/MeloperoSensei.hpp"
#include "sprites.h"
#include "game sprites.h"
#include "pico/time.h"
#include "pico/rand.h"
#include <math.h>
#include <list>
#include <algorithm>

enum class DemoState
{
    SHOW_FONT, SHOW_LOGO, MENU, PLAY, HEART_BEAT,
};

DemoState state = DemoState::SHOW_FONT;

void updateShowFont(float dt);
void renderShowFont();

void updateShowLogo(float dt);
void renderShowLogo();

void updateMenu(float dt);
void renderMenu();

void updatePlay(float dt);
void renderPlay();

void updateHeartBeat(float dt);
void renderHeartBeat();

static const float timeTitle = 5.0f;
static const float timeLogo = 10.0f;
static const float timeHeartBeat = 5.0f;
static const float timeMenu = 5.0f;
static const float timePlay = 10.0f;
static float elapsed = 0.0f;

MeloperoSensei sensei;

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

        switch (state)
        {
            case DemoState::SHOW_FONT:
                updateShowFont(deltaTimeSec);
                renderShowFont();
                break;

            case DemoState::SHOW_LOGO:
                updateShowLogo(deltaTimeSec);
                renderShowLogo();
                break;

            case DemoState::MENU:
                updateMenu(deltaTimeSec);
                renderMenu();
                break;

            case DemoState::PLAY:
                updatePlay(deltaTimeSec);
                renderPlay();
                break;

            case DemoState::HEART_BEAT:
                updateHeartBeat(deltaTimeSec);
                renderHeartBeat();
                break;
        }
    }
}

#include <vector>

float stringTime = 1.5f;
float stringElapsed = 1.5;

struct StringToPrint
{
    int posX, posY;
    enum font_name font;
    enum font_size size;
    uint8_t red, green, blue;
};

std::vector<StringToPrint> strings;
int posX = 40, posY = 25;
enum font_name font = FREE_MONO_BOLD;
uint8_t red = (float)get_rand_32() / UINT32_MAX * 0xFF, green = (float)get_rand_32() / UINT32_MAX * 0xFF, blue = (float)get_rand_32() / UINT32_MAX * 0xFF;

void updateShowFont(float dt)
{
    stringElapsed += dt;
    if (stringElapsed >= stringTime)
    {
        stringElapsed = 0.0f;

        strings.push_back({posX, posY, font, SMALL, red, green, blue});

        font = static_cast<enum font_name>(font + 1);
        posY += 70;

        red = (float)get_rand_32() / UINT32_MAX * 0xFF;
        green = (float)get_rand_32() / UINT32_MAX * 0xFF;
        blue = (float)get_rand_32() / UINT32_MAX * 0xFF;
    }   

    if (posY >= DISPLAY_HEIGHT)
    {
        strings.clear();
        posX = 40, posY = 25;
        font = FREE_MONO_BOLD;
        red = 0xFF, green = 0x00, blue = 0x00;
        stringElapsed = 1.5f;

        state = DemoState::SHOW_LOGO;
    }
}

void renderShowFont()
{
    sensei.clearScreen(0xAA, 0xAA, 0xAA);
    
    for (auto string : strings)
    {
        sensei.setTextFont(string.font, string.size);
        sensei.setTextColor(string.red, string.green, string.blue);
        sensei.print(string.posX, string.posY, "MELOPERO\n     Sensei");
    }
    
    sensei.presentScreen();
}

int logoX = 50, logoY = 0; 
int logoVx = 3, logoVy = 3;
const uint8_t logoWidth = 78, logoHeight = 78;

void updateShowLogo(float dt)
{
    elapsed += dt;
    if (elapsed >= timeLogo)
    {
        elapsed = 0.0f;
        state = DemoState::HEART_BEAT;
    }

    logoX += logoVx;
    logoY += logoVy;

    if (logoX <= 0)
    {
        logoX = 0;
        logoVx = -logoVx;
    }

    if (logoX + logoWidth >= DISPLAY_WIDTH)
    {
        logoX = DISPLAY_WIDTH - logoWidth;
        logoVx = -logoVx;
    }

    if (logoY <= 0)
    {
        logoY = 0;
        logoVy = -logoVy;
    }

    if (logoY + logoHeight >= DISPLAY_HEIGHT)
    {
        logoY = DISPLAY_HEIGHT - logoHeight;
        logoVy = -logoVy;
    }
}

void renderShowLogo()
{
    sensei.clearScreen(0xCD, 0x23, 0x055);
    sensei.drawSprite(melopero_logo, logoX, logoY, logoWidth, logoHeight);
    sensei.presentScreen();
}

static constexpr uint16_t numStars{500};
static constexpr uint16_t numBigStars{100};
static constexpr uint16_t numDistantStars{100};

struct Vec2D
{
    int x, y;
};

Vec2D stars[numStars];

bool initStars = []() {
        for (auto &star : stars)
        {
            star.x = get_rand_32() % DISPLAY_WIDTH;
            star.y = get_rand_32() % DISPLAY_HEIGHT;
        }

        return true;
}();

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

int starshipX{(DISPLAY_WIDTH - starshipWidth) / 2}, starshipY{DISPLAY_HEIGHT - starshipHeight - 1};
int starshipVx, starshipVy;

struct Projectile
{
    Vec2D pos;
    bool alive = true;
};

std::vector<Projectile> playerProjectiles;
std::vector<Projectile> enemyProjectiles;

static const uint8_t playerProjectileVel = 160;  // px/sec

struct Particle
{
    Vec2D pos;
    Vec2D vel;
    float lifetime;
    float duration = 0.0f;
    bool alive = true;
};

static const uint8_t numParticlesPerExplosion = 50;
static const float particlesSpeedMean = 10.0f;
static const float particlesLifeTimeMean = 50.0f;

std::list<Particle> particles;

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

bool initEnemyDefs = []() {
    enemyDefs.push_back({EnemyDef::ENEMY1, 1, 120, 50, 100.0f, 2});
    enemyDefs.push_back({EnemyDef::ENEMY2, 3, 80, 100, 100.0f, 1});
    enemyDefs.push_back({EnemyDef::ENEMY3, 4, 50, 80, 100.0f, 3});
    enemyDefs.push_back({EnemyDef::BOMB, 5, 140, scrollSpeed * 3, 100.0f, 0});
    enemyDefs.push_back({EnemyDef::ENEMY1, 6, 200, 50, 100.0f, 4});
    enemyDefs.push_back({EnemyDef::ENEMY2, 7, 10, 100, 100.0f, 1});
    enemyDefs.push_back({EnemyDef::ENEMY3, 8, 180, 30, 100.0f, 2});
    enemyDefs.push_back({EnemyDef::BOMB, 8, 20, scrollSpeed * 3, 100.0f, 0});
    enemyDefs.push_back({EnemyDef::ENEMY3, 9, 50, 50, 100.0f, 2});
    enemyDefs.push_back({EnemyDef::BOMB, 10, 20, scrollSpeed * 3, 100.0f, 0});

    return true;
}();

uint8_t score;
uint8_t lives = 3;
uint8_t health = 10;

void updateMenu(float dt)
{
    elapsed += dt;
    if (elapsed >= timeMenu)
    {
        elapsed = 0.0f;

        enemies.clear();
    
        score = 0;
        health = 10;
        lives = 3;  

        starshipX = {(DISPLAY_WIDTH - starshipWidth) / 2}, starshipY = {DISPLAY_HEIGHT - starshipHeight - 1};

        enemyDefs.clear();    
        enemyDefs.push_back({EnemyDef::ENEMY1, 1, 120, 50, 100.0f, 2});
        enemyDefs.push_back({EnemyDef::ENEMY2, 3, 80, 100, 100.0f, 1});
        enemyDefs.push_back({EnemyDef::ENEMY3, 4, 50, 80, 100.0f, 3});
        enemyDefs.push_back({EnemyDef::BOMB, 5, 140, scrollSpeed * 3, 100.0f, 0});
        enemyDefs.push_back({EnemyDef::ENEMY1, 6, 200, 50, 100.0f, 4});
        enemyDefs.push_back({EnemyDef::ENEMY2, 7, 10, 100, 100.0f, 1});
        enemyDefs.push_back({EnemyDef::ENEMY3, 8, 180, 30, 100.0f, 2});
        enemyDefs.push_back({EnemyDef::BOMB, 8, 20, scrollSpeed * 3, 100.0f, 0});
        enemyDefs.push_back({EnemyDef::ENEMY3, 9, 50, 50, 100.0f, 2});
        enemyDefs.push_back({EnemyDef::BOMB, 10, 20, scrollSpeed * 3, 100.0f, 0});

        state = DemoState::PLAY;
    }
}

void renderMenu()
{
    sensei.clearScreen(0x00, 0x00, 0x00);
    sensei.drawSprite(menu, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    sensei.presentScreen();
}

void updatePlay(float dt)
{
    elapsed += dt;
    if (elapsed >= timePlay)
    {
        elapsed = 0.0f;
        state = DemoState::SHOW_FONT;
    }

    // update background
    for (int i = 0; i < numStars; i++)
    {
        if (i < numBigStars)
            stars[i].y += scrollSpeed * 3 * dt;
        else if (i < numBigStars + numDistantStars)
            stars[i].y += scrollSpeed * dt;
        else
            stars[i].y += scrollSpeed * 2 * dt;

        if (stars[i].y >= DISPLAY_HEIGHT)
        {
            stars[i].x = get_rand_32() % DISPLAY_WIDTH;
            stars[i].y = 0;
        }
    }

    // spawn enemies
    while (!enemyDefs.empty() && elapsed >= enemyDefs.front().triggerTime)
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
        enemy.update(dt);

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
            projectile.pos.y -= playerProjectileVel * dt;

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
            projectile.pos.y += playerProjectileVel * dt;

            if (projectile.pos.x >= starshipX && projectile.pos.x <= starshipX + starshipWidth && projectile.pos.y >= starshipY && projectile.pos.y <= starshipY + starshipHeight)
            {
                projectile.alive = false;
                
                sensei.playNote(600.f, 300, 1.0f, false, 100.0f);
                
                health--;

                if (health == 0)
                    health = 0;
            }

            if (projectile.pos.x < 0 || projectile.pos.x >= DISPLAY_WIDTH || projectile.pos.y < 0 || projectile.pos.y >= DISPLAY_HEIGHT)
                projectile.alive = false;
        }

        // remove off-screen and collided projectiles
        enemyProjectiles.erase(std::remove_if(enemyProjectiles.begin(), enemyProjectiles.end(), [](const Projectile &p){ return !p.alive; }), enemyProjectiles.end());

        // move player
        starshipVx = starshipVy = 0;

        static float elapsedFire = 0.0f;
        static float timeFire = 1.5f;
        elapsedFire += dt;

        if (elapsedFire >= timeFire)
        {
            elapsedFire = 0.f;

            playerProjectiles.push_back({starshipX + starshipWidth / 2, starshipY + 10});
            sensei.playNote(440.f, 100, 1.0f, true, 100.0f);
        }

        if (elapsed < 0.5f)
            starshipVx = 2;
        else if (elapsed > 1.5 && elapsed < 2.5f)
            starshipVx = -2;
        else if (elapsed > 3.5f && elapsed < 4.5f)
            starshipVx = -2;
        else if (elapsed > 5.5f && elapsed < 7.0f)
        {
            starshipVx = 2;
            starshipVy = -2;
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
    sensei.setTextFont(FREE_SANS, VERY_SMALL);
    sensei.setTextColor(0xFF, 0xFF, 0x00);
    sensei.print(5, 20, (std::string("score: ") + std::to_string(score)).c_str());
        
    if (health < 3)
        sensei.setTextColor(0xFF, 0x00, 0x00);
    else if (health < 6)
        sensei.setTextColor(0xFF, 0xFF, 0x00);
    else
        sensei.setTextColor(0xFF, 0xFF, 0xFF);
    sensei.print(5, 40, (std::string("health: ") + std::to_string(health)).c_str());
        
    sensei.setTextColor(0x00, 0x00, 0xFF);
    sensei.print(5, 60, (std::string("lives: ") + std::to_string(lives)).c_str());

    sensei.presentScreen();
}

const uint16_t *currentHeart = heartSmall;
float timeBeat = 0.5f;
float elapsedBeat = 0.0f;

void updateHeartBeat(float dt)
{
    elapsed += dt;
    if (elapsed >= timeHeartBeat)
    {
        elapsed = 0.0f;
        state = DemoState::MENU;
    }

    elapsedBeat += dt;
    if (elapsedBeat >= timeBeat)
    {
        elapsedBeat = 0.0f;
        currentHeart = currentHeart == heartSmall ? heartBig : heartSmall;
        sensei.playNote(600.f, 50, 1.0f, false, 100.0f);
        sensei.playNote(PAUSE, 50, 1.0f, false, 100.0f);
        sensei.playNote(600.f, 200, 1.0f, false, 100.0f);
    }
}

void renderHeartBeat()
{
    sensei.clearScreen(0x00, 0xAA, 0xFF);
    if (currentHeart == heartBig)
        sensei.drawSprite(currentHeart, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    else
        sensei.drawSprite(currentHeart, 45, 45, 150, 150);
    sensei.presentScreen();
}
