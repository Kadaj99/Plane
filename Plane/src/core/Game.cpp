#include "Game.h"

Game::Game()
    : Game(512, 768) { // 使用委托构造函数
}

Game::Game(int windowWidth, int windowHeight)
    : m_map(),
      player(windowWidth, windowHeight), // 在此处传入窗口的宽度和高度
      bullets(),
      enemies(),
      bombs(),
      drops()
    {
    // 初始化子弹容器
    bullets.reserve(magazineSize);
    for (int i = 0; i < magazineSize; ++i) {
        bullets.emplace_back(player.getX() + 25, player.getY(), 10, Bullet::BulletTrajectory::Weapon1);
        bullets.back().setState(Bullet::BulletState::Inactive);
    }

    currentState=GameState::MainMenu;
    currentLevel=LevelSelection::Normal;
    currentControlType=ControlType::Keyboard;
    currentTrajectory=Bullet::BulletTrajectory::Weapon1;
    menuSelection=0;
    scroll_speed = 3;
    lastFireTime = 0;
    lastDropSpawnTime = 0;
    useKeyboardControl = false;


    // 如果需要默认开启 autoFire，将其设置为 true
    autoFire = true;
    };

void Game::updateMyTimer(uint32_t deltaTime) {
    myTimer += deltaTime;
}


void Game::menuInput(const char input){
      switch (input) {
        case 'w':
            menuSelection = (menuSelection - 1 + 3) % 3;
            break;
        case 's':
            menuSelection = (menuSelection + 1) % 3;
            break;
        case '\r':
            if (menuSelection == 0) {
                    currentState = GameState::Running;
                } else if (menuSelection == 1) {
                    currentState = GameState::ControlMenu;
                } else if (menuSelection == 2) {
                    currentState = GameState::Level;
                }
                break;
      }
};

void Game::saveScore() {
        scoreManager.saveScore();
    }

void Game::runningInput(const char input){
    switch (input){
        case 'w':
            player.moveUp();
            break;
        case 'a':
            player.moveLeft();
            break;
        case 's':
            player.moveDown();
            break;
        case 'd':
            player.moveRight();
            break;
        case 'f':
            Game::fireBullet(currentTrajectory);
            break;
        case 'p':
            currentState = GameState::Pause;
            break;
        case 'e':
            saveScore();
            currentState = GameState::MainMenu;
            break;

    default:
        break;
    }
};

void Game::controlInput(const char input){
   switch (input) {
        case 'w':
            controlSelection = (controlSelection - 1 + 2) % 2; // 有两个控制选项
            break;
        case 's':
            controlSelection = (controlSelection + 1) % 2;
            break;
        case '\r': 
            if (controlSelection == 0) {
                useKeyboardControl = true;
                setControlType(Game::ControlType::Keyboard);   
            } else if (controlSelection == 1) {
                useKeyboardControl = false;
                setControlType(Game::ControlType::Mouse);
            }
            currentState = GameState::MainMenu;
            break;
}
};

void Game::levelInput(const char input){
    switch (input) {
        case 'w':
            levelMenuSelection = (levelMenuSelection - 1 + 3) % 3;
            break;
        case 's':
            levelMenuSelection = (levelMenuSelection + 1) % 3;
            break;
        case 'd':
        case '\r': 
            if (levelMenuSelection == 0) {
                currentLevel = LevelSelection::Easy;
                resetGameObjects();
            } else if (levelMenuSelection == 1) {
                currentLevel = LevelSelection::Normal;
                resetGameObjects();
            } else if (levelMenuSelection == 2) {
                currentLevel = LevelSelection::Hard;
                resetGameObjects();
            }
            currentState = GameState::MainMenu;
            break;
}
}
;
void Game::checkCollisions() {
    for (auto& enemy : enemies) {
        if (enemy.getState() == Enemy::EnemyState::Active) {
            // 检查子弹与敌机的碰撞
            for (auto& bullet : bullets) {
                if (bullet.getState() == Bullet::BulletState::Active) {
                    Rect::My_Rect bulletRect = bullet.getCollisionRect();
                    Rect::My_Rect enemyRect = enemy.getCollisionRect();
                    if (Rect::hasIntersection(&bulletRect, &enemyRect)) {
                        bullet.setState(Bullet::BulletState::Inactive);
                        enemy.setState(Enemy::EnemyState::Inactive);

                        // 创建爆炸特效
                        Bomb bomb(enemy.getX(), enemy.getY());
                        bomb.setState(Bomb::BombState::Active);
                        bombs.push_back(bomb);

                        // 增加玩家分数
                        scoreManager.setPlayerScore(scoreManager.getPlayerScore() + 1);

                        //播放音效
                        setBombSound(true);
                    }
                }
            }
        }

        // 检查玩家与敌机的碰撞
        Rect::My_Rect playerRect = player.getCollisionRect();
        Rect::My_Rect enemyRect = enemy.getCollisionRect();
        if (Rect::hasIntersection(&playerRect, &enemyRect)) {
            enemy.setState(Enemy::EnemyState::Inactive);
            currentState = GameState::GameOver;
        }
    }
    // 检查玩家与补给之间的碰撞
    for (auto& drop : drops) {

        Rect::My_Rect playerRect = player.getCollisionRect();
        Rect::My_Rect dropRect = drop.getCollisionRect();
        if (Rect::hasIntersection(&playerRect, &dropRect)) 
        {
            // 处理碰撞效果
            switch (drop.getType()) {
            /*
            case DropType::IncreaseAttackPower:
                // 增加攻击力逻辑
                break;
            case DropType::HealthRecovery:
                // 回复血量逻辑
                break;
            case DropType::DecreaseFireInterval:
                // 减少子弹发射间隔逻辑
                break;
            */
            case Drop::DropType::Bonus:
                scoreManager.setPlayerScore(scoreManager.getPlayerScore() + 1);
                break;

            case Drop::DropType::Malus:
                scoreManager.setPlayerScore(scoreManager.getPlayerScore() - 1);

            }

            // 将补给设置为非活动状态
            drop.setActive(false);
        }
}

}
void Game::resetGameObjects() {
    // 重置游戏对象
    player.reset();
    bombs.clear();
    // 重置玩家分数


    // 重置子弹为非激活状态
    for (Bullet &bullet : bullets) {
        bullet.reset();
    }

    // 重置敌机为非激活状态
    for (Enemy &enemy : enemies) {
        enemy.reset();
    }

}


void Game::switchWeapon(int weaponLevel) {
    switch (weaponLevel) {
        case 2:
            fireBullet(Bullet::BulletTrajectory::Weapon2);
            break;
        case 3:
            fireBullet(Bullet::BulletTrajectory::Weapon3);
            break;
        case 4:
            fireBullet(Bullet::BulletTrajectory::Weapon4);
            break;
        case 5:
            fireBullet(Bullet::BulletTrajectory::Weapon5);
            break;
        default:
            fireBullet(Bullet::BulletTrajectory::Weapon1);
            break;
    }
}


void Game::fireBullet(Bullet::BulletTrajectory trajectory) {
    switch (trajectory) {
        case Bullet::BulletTrajectory::Weapon2:
            fireEnhance(trajectory, 2);
            break;
        case Bullet::BulletTrajectory::Weapon3:
            fireEnhance(trajectory, 3);
            break;
        case Bullet::BulletTrajectory::Weapon4:
            fireEnhance(trajectory, 4);
            break;
        case Bullet::BulletTrajectory::Weapon5:
            fireEnhance(trajectory, 5);
            break;
        default:
            fireDefault(trajectory);
            break;
    }
}


void Game::fireDefault(Bullet::BulletTrajectory trajectory) {
    int playerX = player.getX();
    int playerY = player.getY();
    // 查找一个未激活的子弹
    Bullet *inactiveBullet = nullptr;
    for (Bullet &bullet : bullets) {
        if (bullet.getState() == Bullet::BulletState::Inactive) {
            inactiveBullet = &bullet;
            break;
        }
    }

    if (inactiveBullet != nullptr) {
        // 飞机对象具有 getX() 和 getY() 方法，返回飞机的位置
        int initialBulletX = playerX + 16;  
        int initialBulletY = playerY -25;            

        // 假设子弹速度为 5，根据实际情况调整
        int bulletSpeed = 10;

        inactiveBullet->setState(Bullet::BulletState::Active);
        inactiveBullet->setX(initialBulletX);    
        inactiveBullet->setY(initialBulletY);    
        inactiveBullet->setSpeedX(0);
        inactiveBullet->setSpeedY(-bulletSpeed);
        inactiveBullet->setTrajectory(Bullet::BulletTrajectory::Weapon1);
    }
    
};


void Game::fireEnhance(Bullet::BulletTrajectory trajectory, int bulletCount) {
    int playerX = player.getX();
    int playerY = player.getY();
    float angleIncrement;

    
    // 寻找未激活的子弹，发射它们
    int bulletsFired = 0;
    for (Bullet &bullet : bullets) {
        if (bullet.getState() == Bullet::BulletState::Inactive) {
            bullet.setState(Bullet::BulletState::Active);
            bullet.setSpeed(10);
            bullet.setTrajectory(trajectory);

            // 根据轨迹和子弹数量设置子弹的初始位置
            switch (trajectory) {
                case Bullet::BulletTrajectory::Weapon2:
                    if (bulletsFired == 0) {
                        bullet.setX(playerX);
                    } else {
                        bullet.setX(playerX+25);
                    }
                    bullet.setSpeedX(0);
                    bullet.setSpeedY(-10);
                    bullet.setY(playerY-25);
                    currentTrajectory = trajectory;
                    break;

                case Bullet::BulletTrajectory::Weapon3:
                    bullet.setX(playerX - 10 + bulletsFired * 25);
                    bullet.setY(playerY-25);
                    bullet.setSpeedX(0);
                    bullet.setSpeedY(-10);
                    currentTrajectory = trajectory;
                    break;

                case Bullet::BulletTrajectory::Weapon4:
                {
                    angleIncrement = 24.0 / (bulletCount - 1);
                    float angle1 = -12 + bulletsFired * angleIncrement;
                    bullet.setX(playerX+16);
                    bullet.setY(playerY-25);
                    bullet.setSpeedX(10 * cos((angle1 - 90) * M_PI / 180.0));
                    bullet.setSpeedY(10 * sin((angle1 - 90) * M_PI / 180.0));
                    currentTrajectory = trajectory;
                    break;
                }

                case Bullet::BulletTrajectory::Weapon5:
                {
                    angleIncrement = 36.0 / (bulletCount - 1);
                    float angle2 = -18 + bulletsFired * angleIncrement;
                    bullet.setX(playerX+16);
                    bullet.setY(playerY-25);
                    bullet.setSpeedX(10 * cos((angle2 - 90) * M_PI / 180.0));
                    bullet.setSpeedY(10 * sin((angle2 - 90) * M_PI / 180.0));
                    currentTrajectory = trajectory;
                    break;
                }

                default:
                
                    break;
            }

            bulletsFired++;
            if (bulletsFired >= bulletCount) {
                break;
            }
        }
    }
};




void Game::update() {

    

    // Add game update logic here
    m_map.mapPosition(scroll_speed);
    // 更新子弹
    for (Bullet &bullet : bullets) {
        if (bullet.getState() == Bullet::BulletState::Active) {
            bullet.update();
            //std::cout << "logic:bullet position (x, y): (" << bullet.getX() << ", " << bullet.getY() << ")\n";

            // 检查子弹是否超出窗口边界
            if (bullet.getY() < 0) {
            bullet.setState(Bullet::BulletState::Inactive);
            }
        }
    }
    if (autoFire) {
    // Add your auto-fire logic here, for example:
    if (getMyTimer() - lastFireTime > fireInterval) {
        fireBullet(currentTrajectory);
        lastFireTime = getMyTimer();
    }
}

    uint32_t currentTime = getMyTimer();
    
    // 更新敌机生成和速度
    int speed;
    switch (currentLevel) {
        case LevelSelection::Easy:
            enemySpawnInterval = 2000;
            maxEnemyCount = 10;
            speed = 4;
            break;
        case LevelSelection::Normal:
            enemySpawnInterval = 1500;
            maxEnemyCount = 20;
            speed = 6;
            break;
        case LevelSelection::Hard:
            enemySpawnInterval = 1000;
            maxEnemyCount = 30;
            speed = 8;
            break;
        default:
            break;
    }

    if (currentTime - lastEnemySpawnTime >= enemySpawnInterval && enemies.size() < maxEnemyCount) {
        int randomX = rand() % 512;
        enemies.emplace_back(randomX, 0, enemySpawnInterval, speed);
        lastEnemySpawnTime = currentTime;
    }

    // 更新敌机速度
    for (auto& enemy : enemies) {
        enemy.setSpeed(speed);
        enemy.update();
    }

    // 生成一个补给
    if (currentTime - lastDropSpawnTime >= dropSpawnInterval) {
        int randomX = rand() % 512;
        int randomType = rand() % 2;
        Drop::DropType dropType = static_cast<Drop::DropType>(randomType);
        drops.emplace_back(randomX, 0, 5, dropType);
        lastDropSpawnTime = currentTime;
    }

    // 更新补给状态
    for (auto& drop : drops) {
        drop.update();
    }

    checkCollisions(); // 在update()函数中添加碰撞检测

    
    // 更新爆炸特效
    for (auto& bomb : bombs) {
        bomb.update(currentTime);
    }
}
