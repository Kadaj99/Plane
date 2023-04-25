#include "mainSDL.h"
using namespace std;
#include <iostream>
#include <sstream>
#include <iomanip>



Game::Game(const std::string& title, int width, int height)
    : windowWidth(512), windowHeight(768), title("Dogfighter"), isRunning(true), player(windowWidth, windowHeight)
{
    // Initialize SDL video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        isRunning = false;
    }

    // Create window
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        isRunning = false;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        isRunning = false;
    }

    if(TTF_Init() == -1)
    {
        std::cout << "TTF_init is failed: " << TTF_GetError() << std::endl;
        isRunning = false;
    }

    currentState = GameState::MainMenu;
    menuSelection = 0;
    optionsMenuSelection = 0;

    // 默认选项
    useKeyboardControl = false;
    autoFire = false;
    controlSelection = 0;
    controlMenuSelection = 0;
    levelMenuSelection = 0;
    currentTrajectory = Bullet::BulletTrajectory::Weapon1;
    
}



Game::~Game() {
    cleanUpSDL();
}



bool Game::initSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cout << "SDL initialization failed: " << SDL_GetError() << endl;
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Window creation error: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Renderer creation error: " << SDL_GetError() << endl;
        return false;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);
    if (texture == nullptr) {
        cout << "Failed to create texture: " << SDL_GetError() << endl;
        isRunning = false;
    }

    // 初始化 SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cout << "SDL_image initialization error: " << IMG_GetError() << endl;
        return false;
    }
    //Menu
    titleTexture = IMG_LoadTexture(renderer,"data/title.png");
    startGameTextureSelected = IMG_LoadTexture(renderer,"data/start2.png");
    startGameTextureUnselected = IMG_LoadTexture(renderer,"data/start1.png");
    optionsTextureSelected = IMG_LoadTexture(renderer,"data/option2.png");
    optionsTextureUnselected = IMG_LoadTexture(renderer,"data/option1.png");
    controlsTextureSelected = IMG_LoadTexture(renderer, "data/c2.png");
    controlsTextureUnselected = IMG_LoadTexture(renderer, "data/c1.png");

    levelTextureSelected = IMG_LoadTexture(renderer,"data/level2.png");
    levelTextureUnselected = IMG_LoadTexture(renderer,"data/level1.png");
    easyTextureSelected = IMG_LoadTexture(renderer,"data/easy2.png");
    easyTextureUnselected = IMG_LoadTexture(renderer,"data/easy1.png");
    normalTextureSelected = IMG_LoadTexture(renderer,"data/normal2.png");
    normalTextureUnselected = IMG_LoadTexture(renderer,"data/normal1.png");
    hardTextureSelected = IMG_LoadTexture(renderer,"data/hard2.png");
    hardTextureUnselected = IMG_LoadTexture(renderer,"data/hard1.png");

    
    // Load main menu background texture
    SDL_Surface *mainMenuBackgroundSurface = IMG_Load("data/backGroundMenu.jpg");
    if (!mainMenuBackgroundSurface) {
        cout << "Failed to load main menu background texture" << endl;
        return false;
    }

    mainMenuBackgroundTexture = SDL_CreateTextureFromSurface(renderer, mainMenuBackgroundSurface);
    if (!mainMenuBackgroundTexture) {
        cout << "Failed to create texture from main menu background surface" << endl;
        SDL_FreeSurface(mainMenuBackgroundSurface);
        return false;
    }

    SDL_FreeSurface(mainMenuBackgroundSurface);


    // 加载玩家图片
    SDL_Surface *player_surface = IMG_Load("data/hero.png");
    if (!player_surface) {
        cout << "Error loading player image: " << IMG_GetError() << endl;
        return false;
    }

    // 创建玩家纹理
    playerTexture = SDL_CreateTextureFromSurface(renderer, player_surface);

     // 加载敌机纹理
    enemyTexture = IMG_LoadTexture(renderer, "data/img-plane_3.png");

    // 创建纹理
    
    bg_texture1_easy = IMG_LoadTexture(renderer,"data/img_bg_level_1.jpg");
    bg_texture2_easy = IMG_LoadTexture(renderer,"data/img_bg_level_1.jpg");
    bg_texture1_normal = IMG_LoadTexture(renderer,"data/img_bg_level_3.jpg");
    bg_texture2_normal = IMG_LoadTexture(renderer,"data/img_bg_level_3.jpg");
    bg_texture1_hard = IMG_LoadTexture(renderer,"data/img_bg_level_5.jpg");
    bg_texture2_hard = IMG_LoadTexture(renderer,"data/img_bg_level_5.jpg");

 
    SDL_FreeSurface(player_surface); // 释放已创建纹理的表面





    // 加载子弹纹理
    bulletTexture = IMG_LoadTexture(renderer, "data/hero_bullet_7.png");
    if (!bulletTexture) {
        cout << "Failed to load bullet texture: " << IMG_GetError() << endl;
        return false;
    }

    // 初始化子弹容器
    bullets.reserve(magazineSize);
    for (int i = 0; i < magazineSize; ++i) {
        bullets.emplace_back(player.getX() + 25, player.getY(), 10, Bullet::BulletTrajectory::Weapon1);
        bullets.back().setState(Bullet::BulletState::Inactive);
    }

    // 加载爆炸特效纹理
    for (int i = 1; i <= 7; ++i) {
        std::ostringstream oss;
        oss << "data/bomb-" << i << ".png";
        SDL_Surface* surface = IMG_Load(oss.str().c_str());
        if (!surface) {
         std::cout << "Failed to load bomb-" << i << ".png. Error: " << IMG_GetError() << std::endl;
    } else {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        bombTextures.push_back(texture);
    }
    }

    keyboardTextureSelected = IMG_LoadTexture(renderer, "data/k2.png");
    keyboardTextureUnselected = IMG_LoadTexture(renderer, "data/k1.png");
    mouseTextureSelected = IMG_LoadTexture(renderer, "data/m2.png");
    mouseTextureUnselected = IMG_LoadTexture(renderer, "data/m1.png");
    //font.ttf
    TTF_Init();

    pauseFont = TTF_OpenFont("data/font1.ttf", 64);
    if (!pauseFont) {
        cout << "Failed to load pause font: " << TTF_GetError() << endl;
    }
    scoreFont = TTF_OpenFont("data/font3.ttf", 24);
    if (!scoreFont) {
        cout << "Failed to load score font: " << TTF_GetError() << endl;
    }

    gameOverFont = TTF_OpenFont("data/font2.ttf", 80); // 24是字体大小，可以根据需要更改
    if (!gameOverFont) {
        std::cout << "Failed to load over font: " << TTF_GetError() << std::endl;
        isRunning = false;
    }

    //BackGroundMusic
    // Handle the error
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    std::cerr << "Failed to initialize SDL audio: " << SDL_GetError() << std::endl;
    return -1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
    return -1;
    }   

    menuMusic = Mix_LoadMUS("data/menu_music.mp3");
    gameMusic = Mix_LoadMUS("data/game_play_music.mp3");
    bombSound = Mix_LoadWAV("data/sound.wav");
    if (!bombSound) {
        std::cerr << "Error loading bomb sound: " << Mix_GetError() << std::endl;
        // Handle the error
    }

    isMusicPaused = Mix_PlayingMusic() == 1; //pour le menu_pause


    return true;
}

void Game::cleanUpSDL() {
    // Add clean up code for SDL_image, SDL_ttf, and SDL_mixer here
    SDL_DestroyTexture(bg_texture1_easy);
    SDL_DestroyTexture(bg_texture1_hard);
    SDL_DestroyTexture(bg_texture1_normal);
    SDL_DestroyTexture(bg_texture2_easy);
    SDL_DestroyTexture(bg_texture2_hard);
    SDL_DestroyTexture(bg_texture2_normal);

    SDL_DestroyTexture(bulletTexture);

    SDL_DestroyTexture(enemyTexture);

    // 释放爆炸特效纹理
    for (auto& texture : bombTextures) {
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }
    bombTextures.clear();

    // Menu
    SDL_DestroyTexture(titleTexture);
    SDL_DestroyTexture(startGameTextureSelected);
    SDL_DestroyTexture(startGameTextureUnselected);
    SDL_DestroyTexture(optionsTextureSelected);
    SDL_DestroyTexture(optionsTextureUnselected);

    SDL_DestroyTexture(keyboardTextureSelected);
    SDL_DestroyTexture(keyboardTextureUnselected);
    SDL_DestroyTexture(mouseTextureSelected);
    SDL_DestroyTexture(mouseTextureUnselected);
    SDL_DestroyTexture(mainMenuBackgroundTexture);

    SDL_DestroyTexture(levelTextureSelected);
    SDL_DestroyTexture(levelTextureUnselected);
    SDL_DestroyTexture(easyTextureSelected);
    SDL_DestroyTexture(easyTextureUnselected);
    SDL_DestroyTexture(normalTextureSelected);
    SDL_DestroyTexture(normalTextureUnselected);
    SDL_DestroyTexture(hardTextureSelected);
    SDL_DestroyTexture(hardTextureUnselected);

    if (controlsTextureSelected != nullptr) {
    SDL_DestroyTexture(controlsTextureSelected);
    }
    if (controlsTextureUnselected != nullptr) {
        SDL_DestroyTexture(controlsTextureUnselected);
    }
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }

    if (gameOverFont||pauseFont||scoreFont) {
        TTF_CloseFont(pauseFont);
        TTF_CloseFont(scoreFont);
        TTF_CloseFont(gameOverFont);
        pauseFont = nullptr;
        scoreFont = nullptr;
        gameOverFont = nullptr;
    }
    

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    if (menuMusic) {
            Mix_FreeMusic(menuMusic);
            menuMusic = nullptr;
        }

        if (gameMusic) {
            Mix_FreeMusic(gameMusic);
            gameMusic = nullptr;
        }

        if (bombSound) {
            Mix_FreeChunk(bombSound);
            bombSound = nullptr;
        }
    Mix_CloseAudio();    


    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void Game::run() {
    Uint32 gameTime;
    Uint32 lastFrameTime = SDL_GetTicks();

    while (isRunning) {
        handleEvents();
        gameTime = SDL_GetTicks() - lastFrameTime;

        if (currentState == GameState::Running) {
            update();
        }

        render(); // 在用 render 函数
        updateMusic(); // 在主循环中调用 updateMusic

        SDL_Delay(1000 / 60); // Limit the frame rate to 60 FPS
    }
}



void Game::playMusic(Mix_Music* music) {
    Mix_PlayMusic(music, -1);  // -1表示循环播放
};

void Game::stopMusic() {
    Mix_HaltMusic();
}

void Game::pauseMusic() {
    Mix_PauseMusic();
}

void Game::resumeMusic() {
    Mix_ResumeMusic();
}

void Game::updateMusic() {
    
    if (currentState == GameState::MainMenu) {
        if (Mix_PlayingMusic() == 0) {
            playMusic(menuMusic);
        }
    } else if (currentState == GameState::Running) {
        if (Mix_PlayingMusic() == 0) {
            playMusic(gameMusic);
            cout<<"gameM is playing"<<endl;
        }
        

    } else if (currentState == GameState::GameOver) {
        stopMusic();
        playMusic(menuMusic);
}
}

void Game::checkCollisions() {
    for (auto& enemy : enemies) {
        if (enemy.getState() == Enemy::EnemyState::Active) {
            // 检查子弹与敌机的碰撞
            for (auto& bullet : bullets) {
                if (bullet.getState() == Bullet::BulletState::Active) {
                    SDL_Rect bulletRect = bullet.getCollisionRect();
                    SDL_Rect enemyRect = enemy.getCollisionRect();
                    if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
                        bullet.setState(Bullet::BulletState::Inactive);
                        enemy.setState(Enemy::EnemyState::Inactive);

                        // 创建爆炸特效
                        Bomb bomb(enemy.getX(), enemy.getY());
                        bomb.setState(Bomb::BombState::Active);
                        bombs.push_back(bomb);

                        // 增加玩家分数
                        scoreManager.setPlayerScore(scoreManager.getPlayerScore() + 1);

                        //播放音效
                        Mix_PlayChannel(-1, bombSound, 0); // 参数-1表示自动选择空闲的通道，0表示音效播放一次
                    }
                }
            }
        }

        // 检查玩家与敌机的碰撞
        SDL_Rect playerRect = player.getCollisionRect();
        SDL_Rect enemyRect = enemy.getCollisionRect();
        if (SDL_HasIntersection(&playerRect, &enemyRect)) {
            enemy.setState(Enemy::EnemyState::Inactive);
            currentState = GameState::GameOver;
        }
    }
    // 检查玩家与补给之间的碰撞
    for (auto& drop : drops) {

        SDL_Rect playerRect = player.getCollisionRect();
        SDL_Rect dropRect = drop.getCollisionRect();
        if (SDL_HasIntersection(&playerRect, &dropRect)) 
        {
            // 处理碰撞效果
            switch (drop.getType()) {
            case DropType::IncreaseAttackPower:
                // 增加攻击力逻辑
                break;
            case DropType::HealthRecovery:
                // 回复血量逻辑
                break;
            case DropType::DecreaseFireInterval:
                // 减少子弹发射间隔逻辑
                break;
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
    scoreManager.setPlayerScore(0);
    // 重置子弹为非激活状态
    for (Bullet &bullet : bullets) {
        bullet.reset();
    }

    // 重置敌机为非激活状态
    for (Enemy &enemy : enemies) {
        enemy.reset();
    }

}
    

    


void Game::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        std::cout << "Event type: " << event.type << std::endl; // Print event type

        if (event.type == SDL_QUIT) {

            isRunning = false;
            std::cout << "SDL_QUIT event detected. isRunning set to false.\n";
        }

        switch (currentState) {
            case GameState::MainMenu:
                handleMainMenuEvents(event);
                break;

            case GameState::OptionsMenu:
                handleOptionsMenuEvents(event);
                break;

            case GameState::Level:
                handleLevelMenuEvents(event);
                break;

            case GameState::Running:
                handleRunningEvents(event);
                break;

            case GameState::ControlMenu:
                handleControlMenuEvents(event);
                break;

            case GameState::Pause:
                handlePausedEvents(event);
                break;
            
            case GameState::GameOver: // 添加此 case 以处理 GameOver 状态的事件
                handleGameOverEvents(event, SDL_GetTicks());
                break;

            default:
                break;
        }
    }
}


void Game::handleRunningEvents(SDL_Event &event) {
     if (!useKeyboardControl) {
        if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                player.moveUp(10);
                break;

            case SDLK_DOWN:
                player.moveDown(10);
                break;

            case SDLK_LEFT:
                player.moveLeft(10);
                break;

            case SDLK_RIGHT:
                player.moveRight(10);
                break;

            case SDLK_g:
                autoFire = !autoFire; // Toggle auto-fire state
                break;

            case SDLK_p:
                    currentState = GameState::Pause;

            case SDLK_SPACE:
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                Uint32 currentTime = SDL_GetTicks();
                if (currentTime - lastFireTime > fireInterval) {
                    lastFireTime = currentTime;

                    // 寻找第一个非激活状态的子弹
                    for (Bullet &bullet : bullets) {
                        if (bullet.getState() == Bullet::BulletState::Inactive) {
                            // 设置子弹位置为飞机位置
                            bullet.setX(player.getX() + 25); // 假设飞机宽度为 50
                            bullet.setY(player.getY());

                            // 激活子弹
                            bullet.setState(Bullet::BulletState::Active);

                            // 打破循环，只激活一个子弹
                            break;
                        }
                    }
                }
            }
                break;

            case SDLK_ESCAPE: // 添加此 case 以处理 ESC 键事件
                    currentState = GameState::MainMenu;
                    scoreManager.saveScore();
                    break;

            case SDLK_1:
                fireBullet(Bullet::BulletTrajectory::Weapon1);
                break;
            case SDLK_2:
                fireBullet(Bullet::BulletTrajectory::Weapon2);
                break;
            case SDLK_3:
                fireBullet(Bullet::BulletTrajectory::Weapon3);
                break;
            case SDLK_4:
                fireBullet(Bullet::BulletTrajectory::Weapon4);
                break;
            case SDLK_5:
                fireBullet(Bullet::BulletTrajectory::Weapon5);
                break;
        }
    }
    }  else if (useKeyboardControl && (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEMOTION)) {
        if (event.button.button == SDL_BUTTON_LEFT || event.type == SDL_MOUSEMOTION) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            player.setX(mouseX - 25); // 假设飞机宽度为 50
            player.setY(mouseY - 25); // 假设飞机高度为 50
        }
    }
}

void Game::handleGameOverEvents(SDL_Event &event, Uint32 gameTime) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {

            case SDLK_ESCAPE:
                scoreManager.saveScore();
                scoreManager.recordGame(scoreManager.getPlayerScore(), gameTime / 1000.0f);
                currentState = GameState::MainMenu;    
                scoreManager.setPlayerScore(0);
                break;
            case SDLK_RETURN:
                scoreManager.saveScore();
                // 在这里记录游戏分数和时间
                scoreManager.recordGame(scoreManager.getPlayerScore(), gameTime / 1000.0f);
                // 重置游戏状态
                currentState = GameState::Running;
                // 重置游戏对象，例如玩家、敌人、子弹等
                resetGameObjects();    
                break;
            case SDLK_q:
                scoreManager.saveScore();
                scoreManager.recordGame(scoreManager.getPlayerScore(), gameTime / 1000.0f);
                exit(0);
        }
    }
}


void Game::handlePausedEvents(SDL_Event &event) {
    if (isMusicPaused) {
        pauseMusic();
    }
    

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
        resumeMusic();
        currentState = GameState::Running;
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) {
        scoreManager.saveScore();
        scoreManager.recordGame(scoreManager.getPlayerScore(), SDL_GetTicks() / 1000.0f);
        exit(0);
    }

}



void Game::handleMainMenuEvents(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                menuSelection = (menuSelection - 1 + 3) % 3;
                break;

            case SDLK_DOWN:
                menuSelection = (menuSelection + 3) % 3;
                break;

            case SDLK_RIGHT:
            case SDLK_RETURN:
                if (menuSelection == 0) {
                    currentState = GameState::Running;
                } else if (menuSelection == 1) {
                    currentState = GameState::OptionsMenu;
                } else if (menuSelection == 2) {
                    currentState = GameState::Level;
                } else if (menuSelection == 3) {
                    isRunning = false;
                }
                break;
                
            case SDLK_SPACE:
                if (menuSelection == 0) {
                    currentState = GameState::Running;
                } else if (menuSelection == 1) {
                    currentState = GameState::OptionsMenu;
                } else if (menuSelection == 2) {
                    currentState = GameState::Level;
                } else if (menuSelection == 3) {
                    isRunning = false;
                }
                break;

            case SDLK_LEFT:
                currentState = GameState::MainMenu;
                break;

            case SDLK_q:
                scoreManager.saveScore();
                exit(0);
                break;


            default:
                break;
        }
    }
}

void Game::handleLevelMenuEvents(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                levelMenuSelection = (levelMenuSelection - 1 + 3) % 3;
                break;

            case SDLK_DOWN:
                levelMenuSelection = (levelMenuSelection + 3) % 3;
                break;

            case SDLK_RIGHT:
            case SDLK_RETURN:
                if (levelMenuSelection == 0) {
                    currentLevel = LevelSelection::Easy;
                    resetGameObjects();
                } else if (levelMenuSelection == 1) {
                    currentLevel = LevelSelection::Normal;
                    resetGameObjects(); 
                } else if (levelMenuSelection == 2) {
                    currentLevel = LevelSelection::Hard;
                    resetGameObjects(); 
                } else if (levelMenuSelection == 3) {
                    isRunning = false;
                }
                currentState = GameState::MainMenu;
                break;

            case SDLK_SPACE:
                if (levelMenuSelection == 0) {
                    currentLevel = LevelSelection::Easy;
                    resetGameObjects(); 
                } else if (levelMenuSelection == 1) {
                    currentLevel = LevelSelection::Normal;
                    resetGameObjects(); 
                } else if (levelMenuSelection == 2) {
                    currentLevel = LevelSelection::Hard;
                    resetGameObjects(); 
                } else if (levelMenuSelection == 3) {
                    isRunning = false;
                }
                currentState = GameState::MainMenu;
                break;

            case SDLK_LEFT:
                currentState = GameState::MainMenu;
                break;

            case SDLK_ESCAPE:
                currentState = GameState::MainMenu;
                break;

            case SDLK_q:
                scoreManager.saveScore();
                exit(0);
                break;


            default:
                break;
        }
    }

}


void Game::handleOptionsMenuEvents(SDL_Event &event) {
    if (currentState == GameState::OptionsMenu) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    menuSelection = (menuSelection - 1 + 2) % 2;
                    break;
                case SDLK_DOWN:
                    menuSelection = (menuSelection + 1) % 2;
                    break;
                case SDLK_RETURN:
                    if (menuSelection == 0) {
                        currentState = GameState::ControlMenu;
                    } else if (menuSelection == 1) {
                        // ...处理其他选项
                    }
                    break;
                case SDLK_ESCAPE:
                    currentState = GameState::MainMenu;
                    break;
                default:
                    break;
            }
        }
    } else if (currentState == GameState::ControlMenu) {
        handleControlMenuEvents(event);
    }
}


void Game::handleControlMenuEvents(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                controlSelection = (controlSelection - 1 + 2) % 2; // 有两个控制选项
                break;
            case SDLK_DOWN:
                controlSelection = (controlSelection + 1) % 2;
                break;
            case SDLK_RETURN:
                if (controlSelection == 0) {
                    useKeyboardControl = false;
                } else if (controlSelection == 1) {
                    useKeyboardControl = true;
                }
                currentState = GameState::MainMenu;
                break;
            case SDLK_ESCAPE:
                currentState = GameState::MainMenu;
                break;
            default:
                break;
        }
    }
}


void Game::handleKeyboardControlMenuEvents(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_RETURN:
                // 确认选择
                currentControlType = ControlType::Keyboard;
                break;
            case SDLK_ESCAPE:
                // 返回上一级菜单
                currentState = GameState::ControlMenu;
                break;
        }
    }
}

void Game::handleMouseControlMenuEvents(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_RETURN:
                // 确认选择
                currentControlType = ControlType::Mouse;
                break;
            case SDLK_ESCAPE:
                // 返回上一级菜单
                currentState = GameState::ControlMenu;
                break;
        }
    }
}


void Game::renderScore() {
    int playerScore = scoreManager.getPlayerScore();
    int highScore = scoreManager.getHighScore();

    std::stringstream scoreText;
    scoreText << "SCORE: " << playerScore << "    HIGH SCORE: " << highScore;

    SDL_Surface* scoreSurface = TTF_RenderText_Solid(scoreFont, scoreText.str().c_str(), {255, 255, 255});
    if (!scoreSurface) {
        std::cerr << "Failed to render score text: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    if (!scoreTexture) {
        std::cerr << "Failed to create score texture: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect scoreRect;
    scoreRect.x = 20;
    scoreRect.y = 20;
    SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreRect.w, &scoreRect.h);

    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

    // 释放资源
    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);
}


void Game::renderPausedScore() {
    int playerScore = scoreManager.getPlayerScore();

    std::stringstream scoreText;
    scoreText << "SCORE: " << playerScore;

    SDL_Surface* scoreSurface = TTF_RenderText_Solid(scoreFont, scoreText.str().c_str(), {255, 255, 255});
    if (!scoreSurface) {
        std::cerr << "Failed to render score text: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    if (!scoreTexture) {
        std::cerr << "Failed to create score texture: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect scoreRect;
    scoreRect.x = (windowWidth - scoreSurface->w) / 2;
    scoreRect.y = (windowHeight - scoreSurface->h) / 2 + 50;
    scoreRect.w = scoreSurface->w;
    scoreRect.h = scoreSurface->h;

    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

    // 释放资源
    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);
}



void Game::renderPausedScreen() {
    // 使用先前渲染的画面作为背景
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);

    // 渲染半透明灰色覆盖层
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 128);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_Rect rect = {0, 0, windowWidth, windowHeight};
    SDL_RenderFillRect(renderer, &rect);

    // 渲染 "PAUSE" 文字
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Blended(pauseFont, "PAUSE", textColor);
    if (textSurface == nullptr) {
        cout << "Failed to render text: " << TTF_GetError() << endl;
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == nullptr) {
        cout << "Failed to create texture from surface: " << SDL_GetError() << endl;
        SDL_FreeSurface(textSurface);
        return;
    }

    SDL_Rect textRect;
    textRect.x = (windowWidth - textSurface->w) / 2;
    textRect.y = (windowHeight - textSurface->h) / 2;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    // 渲染暂停时的分数
    renderPausedScore();

    // 销毁临时创建的资源
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);

    // 更新屏幕
    SDL_RenderPresent(renderer);
}



void Game::renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y) {
    // 创建目标矩形，用于设置纹理渲染的位置和大小
    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;
    SDL_QueryTexture(texture, nullptr, nullptr, &dstRect.w, &dstRect.h);

    // 将纹理渲染到目标矩形上
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
}


void Game::renderMainMenu() {
    int windowWidth = 512;
    int windowHeight = 768;
    if (renderer == nullptr) {
        // 渲染器创建失败，输出错误信息
        cout << "Failed to create renderer: " << SDL_GetError() << endl;
        return;
    }

    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int titleWidth, titleHeight;
    SDL_QueryTexture(titleTexture, nullptr, nullptr, &titleWidth, &titleHeight);
    int titleX = (windowWidth - titleWidth) / 2;
    int titleY = windowHeight / 4 - titleHeight / 2;

    // Render main menu background
    SDL_Rect srcRect = {0, 0, windowWidth, windowHeight};
    SDL_Rect destRect = {0, 0, windowWidth, windowHeight};
    SDL_RenderCopy(renderer, mainMenuBackgroundTexture, &srcRect, &destRect);


    // 渲染标题
    renderTexture(titleTexture, renderer, titleX, titleY);

    int positionX = windowWidth / 4 ;
    int positionY = 3 * windowHeight / 4 - 120;

    // 渲染 "Start Game" 菜单项
    if (menuSelection == 0) {
        renderTexture(startGameTextureSelected, renderer, positionX, positionY);
    } else {
        renderTexture(startGameTextureUnselected, renderer, positionX, positionY);
    }

    // 渲染 "Options" 菜单项
    if (menuSelection  == 1) {
        renderTexture(optionsTextureSelected, renderer, positionX, positionY + 50);
    } else {
        renderTexture(optionsTextureUnselected, renderer, positionX, positionY + 50);
    }

    // 渲染 "Level" 菜单项
    if (menuSelection  == 2) {
        renderTexture(levelTextureSelected, renderer, positionX, positionY + 110);
    } else {
        renderTexture(levelTextureUnselected, renderer, positionX, positionY + 110);
    }


}

void Game::renderLevelMenu(){
    int windowWidth = 512;
    int windowHeight = 768;
    if (renderer == nullptr) {
        // 渲染器创建失败，输出错误信息
        cout << "Failed to create renderer: " << SDL_GetError() << endl;
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

     // Render main menu background
    SDL_Rect srcRect = {0, 0, windowWidth, windowHeight};
    SDL_Rect destRect = {0, 0, windowWidth, windowHeight};
    SDL_RenderCopy(renderer, mainMenuBackgroundTexture, &srcRect, &destRect);

    int positionX = windowWidth / 2 - 100 ;
    int positionY = windowHeight;

    // 渲染 "Easy" 菜单项
    if (levelMenuSelection == 0) {
        renderTexture(easyTextureSelected, renderer, positionX, positionY / 4);
    } else {
        renderTexture(easyTextureUnselected, renderer, positionX, positionY / 4);
    }

    // 渲染 "Normal" 菜单项
    if (levelMenuSelection == 1) {
        renderTexture(normalTextureSelected, renderer, positionX, positionY / 2);
    } else {
        renderTexture(normalTextureUnselected, renderer, positionX, positionY / 2);
    }

    // 渲染 "Hard" 菜单项
    if (levelMenuSelection == 2) {
        renderTexture(hardTextureSelected, renderer, positionX,  3 * positionY / 4);
    } else {
        renderTexture(hardTextureUnselected, renderer, positionX,  3 * positionY / 4);
    }

};

void Game::renderOptionsMenu() {
    int windowWidth = 512;
    int windowHeight = 768;
    if (renderer == nullptr) {
        // 渲染器创建失败，输出错误信息
        cout << "Failed to create renderer: " << SDL_GetError() << endl;
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

     // Render main menu background
    SDL_Rect srcRect = {0, 0, windowWidth, windowHeight};
    SDL_Rect destRect = {0, 0, windowWidth, windowHeight};
    SDL_RenderCopy(renderer, mainMenuBackgroundTexture, &srcRect, &destRect);

    if (currentState == GameState::OptionsMenu) {
        // 渲染 "Controls" 菜单项
        if (menuSelection == 0) {
            renderTexture(controlsTextureSelected, renderer, (windowWidth - 120) / 2, windowHeight / 2 - 52 / 2);
        } else {
            renderTexture(controlsTextureUnselected, renderer, (windowWidth - 120) / 2, windowHeight / 2 - 52 / 2);
        }

        // 添加其他选项菜单项的渲染代码（如果有的话）

    }
}


void Game::renderControlMenu() {
    if (renderer == nullptr) {
        // 渲染器创建失败，输出错误信息
        cout << "Failed to create renderer: " << SDL_GetError() << endl;
        return;
    }

    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);


     // Render main menu background
    SDL_Rect srcRect = {0, 0, windowWidth, windowHeight};
    SDL_Rect destRect = {0, 0, windowWidth, windowHeight};
    SDL_RenderCopy(renderer, mainMenuBackgroundTexture, &srcRect, &destRect);

    // 渲染 "Keyboard" 选项
    if (controlSelection == 0) {
        renderTexture(keyboardTextureSelected, renderer, 100, 100);
    } else {
        renderTexture(keyboardTextureUnselected, renderer, 100, 100);
    }

    // 渲染 "Mouse" 选项
    if (controlSelection == 1) {
        renderTexture(mouseTextureSelected, renderer, 100, 200);
    } else {
        renderTexture(mouseTextureUnselected, renderer, 100, 200);
    }
}


void Game::renderKeyboardControlMenu() {
    if (renderer == nullptr) {
        // 渲染器创建失败，输出错误信息
        cout << "Failed to create renderer: " << SDL_GetError() << endl;
        return;
    }

    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

     // Render main menu background
    SDL_Rect srcRect = {0, 0, windowWidth, windowHeight};
    SDL_Rect destRect = {0, 0, windowWidth, windowHeight};
    SDL_RenderCopy(renderer, mainMenuBackgroundTexture, &srcRect, &destRect);

    // 在这里渲染键盘控制子菜单的内容

}

void Game::renderMouseControlMenu() {
    if (renderer == nullptr) {
        // 渲染器创建失败，输出错误信息
        cout << "Failed to create renderer: " << SDL_GetError() << endl;
        return;
    }

    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // 在这里渲染鼠标控制子菜单的内容

}

void Game::renderGameOver() {

    SDL_Color textColor = {255, 255, 255}; // 字体颜色：黑色
    SDL_Surface* textSurface = TTF_RenderText_Blended(gameOverFont, "GAME OVER", textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {windowWidth / 2 - textSurface->w / 2, windowHeight / 2 - textSurface->h / 2, textSurface->w, textSurface->h};

    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    // 渲染玩家分数
    std::string scoreText = "YOUR SCORES: " + std::to_string(scoreManager.getPlayerScore());
    SDL_Color scoreColor = {255, 255, 255}; // 字体颜色：白色
    SDL_Surface* scoreSurface = TTF_RenderText_Blended(scoreFont, scoreText.c_str(), scoreColor);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    SDL_Rect scoreRect;
    scoreRect.x = (windowWidth - scoreSurface->w) / 2;
    scoreRect.y = (windowHeight - scoreSurface->h) / 2 + 100;
    scoreRect.w = scoreSurface->w;
    scoreRect.h = scoreSurface->h;

    SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);

    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);
};

void Game::fireBullet(Bullet::BulletTrajectory trajectory) {
    switch (trajectory) {
        case Bullet::BulletTrajectory::Weapon2:
            fireMultipleBullets(trajectory, 2);
            break;
        case Bullet::BulletTrajectory::Weapon3:
            fireMultipleBullets(trajectory, 3);
            break;
        case Bullet::BulletTrajectory::Weapon4:
            fireMultipleBullets(trajectory, 4);
            break;
        case Bullet::BulletTrajectory::Weapon5:
            fireMultipleBullets(trajectory, 5);
            break;
        default:
            fireSingleBullet(trajectory);
            break;
    }
}
void Game::fireSingleBullet(Bullet::BulletTrajectory trajectory) {
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
        int initialBulletX = playerX + (50 / 2);  
        int initialBulletY = playerY;            

        // 假设子弹速度为 5，根据实际情况调整
        int bulletSpeed = 10;

        inactiveBullet->setState(Bullet::BulletState::Active);
        inactiveBullet->setX(initialBulletX);    
        inactiveBullet->setY(initialBulletY);    
        inactiveBullet->setSpeed(bulletSpeed);
        inactiveBullet->setTrajectory(trajectory);
    }
};


void Game::update() {

    // Add game update logic here
    m_map.mapPosition(scroll_speed);
    // 更新子弹
    for (Bullet &bullet : bullets) {
        if (bullet.getState() == Bullet::BulletState::Active) {
            bullet.update();

            // 检查子弹是否超出窗口边界
            if (bullet.getY() < 0) {
            bullet.setState(Bullet::BulletState::Inactive);
            }
        }
    }
    if (autoFire) {
    // Add your auto-fire logic here, for example:
    if (SDL_GetTicks() - lastFireTime > fireInterval) {
        fireBullet(currentTrajectory);
        lastFireTime = SDL_GetTicks();
    }
}

    Uint32 currentTime = SDL_GetTicks();
    
    // 更新敌机生成和速度
    int speed;
    switch (currentLevel) {
        case LevelSelection::Easy:
            enemySpawnInterval = 1500;
            maxEnemyCount = 10;
            speed = 4;
            break;
        case LevelSelection::Normal:
            enemySpawnInterval = 1000;
            maxEnemyCount = 20;
            speed = 6;
            break;
        case LevelSelection::Hard:
            enemySpawnInterval = 500;
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

    // 每10秒生成一个补给
    if (currentTime - lastDropSpawnTime >= dropSpawnInterval) {
        int randomX = rand() % 512;
        int randomType = rand() % 3;
        DropType dropType = static_cast<DropType>(randomType);
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



void Game::render() {
    // 清除屏幕
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    switch (currentState) {
        case GameState::MainMenu:
            renderMainMenu();
            break;
        case GameState::OptionsMenu:
            renderOptionsMenu();
            break;
        case GameState::Level:
            renderLevelMenu();
            break;
        case GameState::ControlMenu:
            renderControlMenu();
            break;
        case GameState::MouseControl:
            renderMouseControlMenu();
            break;
        case GameState::KeyboardControl:
            renderKeyboardControlMenu();
            break;
        case GameState::Pause:
            renderPausedScreen();
            break;
        case GameState::Running:
            renderGame();
            break;
        case GameState::GameOver:
            renderGameOver();
            break;
        default:
            break;
    }

    // 更新屏幕
    SDL_RenderPresent(renderer);
}

void Game::renderGame() {
    // 渲染滚动的背景
    SDL_Texture* current_bg_texture1;
    SDL_Texture* current_bg_texture2;

switch (currentLevel) {
    case LevelSelection::Easy:
        current_bg_texture1 = bg_texture1_easy;
        current_bg_texture2 = bg_texture2_easy;
        break;
    case LevelSelection::Normal:
        current_bg_texture1 = bg_texture1_normal;
        current_bg_texture2 = bg_texture2_normal;
        break;
    case LevelSelection::Hard:
        current_bg_texture1 = bg_texture1_hard;
        current_bg_texture2 = bg_texture2_hard;
        break;
    default:
        break;
}

    // 渲染滚动的背景
    SDL_Rect src_rect = {0, 0, windowWidth, windowHeight};
    SDL_Rect dest_rect1 = {0, m_map.getMap1PosY(), windowWidth, windowHeight};
    SDL_Rect dest_rect2 = {0, m_map.getMap2PosY(), windowWidth, windowHeight};
    SDL_RenderCopy(renderer, current_bg_texture1, &src_rect, &dest_rect1);
    SDL_RenderCopy(renderer, current_bg_texture2, &src_rect, &dest_rect2);

    // 添加玩家渲染代码
    SDL_Rect playerRect;
    playerRect.x = player.getX();
    playerRect.y = player.getY();
    playerRect.w = 50; // 假设玩家宽度为 50
    playerRect.h = 50; // 假设玩家高度为 50
    SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

    // 渲染激活状态的子弹
    for (const Bullet &bullet : bullets) {
        if (bullet.getState() == Bullet::BulletState::Active) {
            // 使用 bulletTexture 渲染子弹
            SDL_Rect dstRect;
            dstRect.x = bullet.getX();
            dstRect.y = bullet.getY();
            SDL_QueryTexture(bulletTexture, nullptr, nullptr, &dstRect.w, &dstRect.h);
            SDL_RenderCopy(renderer, bulletTexture, nullptr, &dstRect);
        }
    }

   
    // 渲染激活状态的敌机
    for (const auto& enemy : enemies) {
        if (enemy.getState() == Enemy::EnemyState::Active) {
            SDL_Rect destRect = enemy.getCollisionRect();

            // 调整目标矩形的宽度和高度以放大敌机贴图
            destRect.w *= 2; // 将敌机宽度放大2倍
            destRect.h *= 2; // 将敌机高度放大2倍
            SDL_RenderCopy(renderer, enemyTexture, nullptr, &destRect);
            }
        }
    renderScore();
    
    // 渲染爆炸特效
        for (const auto& bomb : bombs) {
            if (bomb.getState() == Bomb::BombState::Active) {
                int currentFrame = bomb.getCurrentFrame();
                SDL_Rect srcRect = {0, 0, 86, 68}; // 假设爆炸特效的尺寸为 64x64
                SDL_Rect destRect = {bomb.getX(), bomb.getY(), 64, 64};
                SDL_RenderCopy(renderer, bombTextures[currentFrame], &srcRect, &destRect);
            }
        }

    
    }

