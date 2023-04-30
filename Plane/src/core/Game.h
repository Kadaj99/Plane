#pragma once

#include <vector>
#include <cmath>

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Bomb.h"
#include "Drop.h"
#include "Collision.h"
#include "ScoreManager.h"
#include "Map.h"
#include "Enhance.h"

class Game {
public:
    enum class GameState {
        MainMenu,
        Level,
        ControlMenu,
        MouseControl,
        KeyboardControl,
        Running,
        Pause,
        Store,
        GameOver
};
    GameState currentState;
    int menuSelection;
    int optionsMenuSelection;
    int controlSelection;
    int controlMenuSelection;
    int levelMenuSelection;
    int magazineSize = 100;
    GameState previousState;
    GameState getCurrentState() const { return currentState; }
    bool hasStateChanged() const { return currentState != previousState; }
    void setCurrentState(GameState state) { 
        previousState = currentState;
        currentState = state; }
    
    Game();
    Game(int windowWidth, int windowHeight);
    
    void menuInput(const char input);
    void runningInput(const char input);
    void controlInput(const char input);
    void levelInput(const char input);
    void update();
    void resetGameObjects();
    void saveScore();
    int getPlayerScore() const {
        return scoreManager.getPlayerScore();
    }

    void setPlayerScore(int score) {
        scoreManager.setPlayerScore(score);
    }

    void save() {
        scoreManager.saveScore();
    }

    void recordGame(int playerScore, float gameTime) {
        scoreManager.recordGame(playerScore, gameTime);
    }

    bool playSound = false;
    void setBombSound(bool value) { playSound = value; }
    bool getBombSound() const { return playSound; }


    bool useKeyboardControl;
    enum class ControlType {
        Keyboard,
        Mouse
    };
    ControlType currentControlType = ControlType::Keyboard;
    ControlType getControlType() const { 
        return  currentControlType;
        }
    void setControlType(ControlType controlType) { 
        currentControlType = controlType;
        }

    enum class LevelSelection {
        Easy,
        Normal,
        Hard
    };
    uint32_t lastEnemySpawnTime;
    int enemySpawnInterval;
    int maxEnemyCount;
    LevelSelection currentLevel = LevelSelection::Normal;
    LevelSelection getCurrentLevel() const {
        return currentLevel;
    }
    void setCurrentLevel(LevelSelection level) {
        currentLevel = level;
    }

    void switchWeapon(int weaponLevel);

    uint32_t getMyTimer() const { return myTimer; }
    void updateMyTimer(uint32_t deltaTime);

     // Player getter
    Player& getPlayer() { return player; }

    // Bullets getter
    std::vector<Bullet>& getBullets() { return bullets; }

    // Enemies getter
    std::vector<Enemy>& getEnemies() { return enemies; }

    // Map getter
    Map& getMap() { return m_map; }

    // Bombs getter
    std::vector<Bomb>& getBombs() { return bombs; }

    // Drops getter
    std::vector<Drop>& getDrops() { return drops; }

    // ScoreManager getter
    ScoreManager& getScoreManager() { return scoreManager; }
    


private:

    uint32_t myTimer = 0;

    Map m_map;
    int scroll_speed;

    Player player;
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    std::vector<Bomb> bombs;
    std::vector<Drop> drops;
    ScoreManager scoreManager;
    Enhance enhance;

    void fireBullet(Bullet::BulletTrajectory trajectory);
    void fireDefault(Bullet::BulletTrajectory trajectory);
    void fireEnhance(Bullet::BulletTrajectory trajectory, int bulletCount);
    Bullet::BulletTrajectory currentTrajectory;
    bool autoFire = false;


    
    void checkCollisions();
    uint32_t lastFireTime;
    const int fireInterval = 500;
    uint32_t lastDropSpawnTime;
    const int dropSpawnInterval = 3000;
};