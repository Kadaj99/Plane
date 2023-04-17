#include "mainSDL.h"
using namespace std;
#include <iostream>
#include <sstream>
#include <iomanip>


Game::Game(const std::string& title, int width, int height)
    : windowWidth(512), windowHeight(768), title("Dogfighter"), isRunning(true), player(windowWidth, windowHeight)
{
    if (!initSDL()) {
        isRunning = false;
    }

    currentState = GameState::MainMenu;
    menuSelection = 0;
    optionsMenuSelection = 0;

    // 默认选项
    useKeyboardControl = false;
    autoFire = false;
    controlSelection = 0;
}


Game::~Game() {
    cleanUpSDL();
}



bool Game::initSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    if (!IMG_Init(IMG_INIT_PNG)) {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL initialization error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window creation error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation error: " << SDL_GetError() << std::endl;
        return false;
    }

    // 初始化 SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image initialization error: " << IMG_GetError() << std::endl;
        return false;
    }
    //Menu
    titleTexture = IMG_LoadTexture(renderer,"data/title.png");
    startGameTextureSelected = IMG_LoadTexture(renderer,"data/start2.png");
    startGameTextureUnselected = IMG_LoadTexture(renderer,"data/start1.png");
    optionsTextureSelected = IMG_LoadTexture(renderer,"data/option2.png");
    optionsTextureUnselected = IMG_LoadTexture(renderer,"data/option1.png");

    // 加载背景图片
    SDL_Surface *bg_surface = IMG_Load("data/img_bg_level_1.jpg");
    if (!bg_surface) {
        std::cerr << "Error loading background image: " << IMG_GetError() << std::endl;
        return false;
    }
    // 加载玩家图片
    SDL_Surface *player_surface = IMG_Load("data/hero.png");
    if (!player_surface) {
        std::cerr << "Error loading player image: " << IMG_GetError() << std::endl;
        return false;
    }

    // 创建玩家纹理
    playerTexture = SDL_CreateTextureFromSurface(renderer, player_surface);

    // 创建纹理
    bg_texture1 = SDL_CreateTextureFromSurface(renderer, bg_surface);
    bg_texture2 = SDL_CreateTextureFromSurface(renderer, bg_surface);
    SDL_FreeSurface(bg_surface); // 释放已创建纹理的表面
    SDL_FreeSurface(player_surface); // 释放已创建纹理的表面




    // 加载子弹纹理
    bulletTexture = IMG_LoadTexture(renderer, "data/hero_bullet_7.png");
    if (!bulletTexture) {
        std::cerr << "Failed to load bullet texture: " << IMG_GetError() << std::endl;
        return false;
    }

    // 初始化子弹容器
    bullets.reserve(magazineSize);
    for (int i = 0; i < magazineSize; ++i) {
        bullets.emplace_back(player.getX() + 25, player.getY(), 10); // 假设子弹速度为 10
        bullets.back().setState(Bullet::BulletState::Inactive);
    }

    // 加载爆炸特效纹理
    for (int i = 1; i <= 7; ++i) {
        std::ostringstream oss;
        oss << "data/bomb-" << i << ".png";
        SDL_Surface* surface = IMG_Load(oss.str().c_str());
        if (!surface) {
            // 错误处理
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        bombTextures.push_back(texture);
    }

    controlSelection = 0;
    keyboardTextureSelected = IMG_LoadTexture(renderer, "data/k2.png");
    keyboardTextureUnselected = IMG_LoadTexture(renderer, "data/k1.png");
    mouseTextureSelected = IMG_LoadTexture(renderer, "data/m2.png");
    mouseTextureUnselected = IMG_LoadTexture(renderer, "data/m1.png");

    return true;
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
                    }
                 if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
                bullet.setState(Bullet::BulletState::Inactive);
                enemy.setState(Enemy::EnemyState::Inactive);

                // 创建爆炸特效
                Bomb bomb(enemy.getX(), enemy.getY());
                bomb.setState(Bomb::BombState::Active);
                bombs.push_back(bomb);
            }
            }

        }

            // 检查玩家与敌机的碰撞
            SDL_Rect playerRect = player.getCollisionRect();
            SDL_Rect enemyRect = enemy.getCollisionRect();
            if (SDL_HasIntersection(&playerRect, &enemyRect)) {
                enemy.setState(Enemy::EnemyState::Inactive);
                // 游戏结束，清空所有贴图
                cleanUpSDL();
                isRunning = false;
                break;
            }
        }
    }
}

void Game::cleanUpSDL() {
    // Add clean up code for SDL_image, SDL_ttf, and SDL_mixer here
    SDL_DestroyTexture(bg_texture1);
    SDL_DestroyTexture(bg_texture2);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
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

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void Game::run() {
    while (isRunning) {
        handleEvents();

        switch (currentState) {
            case GameState::MainMenu:
                renderMainMenu();
                break;

            case GameState::OptionsMenu:
                renderOptionsMenu();
                break;

            case GameState::Running:
                update();
                render();
                break;
        }

        SDL_Delay(16); // Approximately 60 FPS
    }
}



void Game::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }

        switch (currentState) {
            case GameState::MainMenu:
                handleMainMenuEvents(event);
                break;

            case GameState::OptionsMenu:
                handleOptionsMenuEvents(event);
                break;

            case GameState::Running:
                handleRunningEvents(event);
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

            default:
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
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return;
    }

    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int titleWidth, titleHeight;
    SDL_QueryTexture(titleTexture, nullptr, nullptr, &titleWidth, &titleHeight);
    int titleX = (windowWidth - titleWidth) / 2;
    int titleY = windowHeight / 4 - titleHeight / 2;

    // 渲染标题
    renderTexture(titleTexture, renderer, titleX, titleY);

    int optionsWidth, optionsHeight;
    SDL_QueryTexture(optionsTextureSelected, nullptr, nullptr, &optionsWidth, &optionsHeight);
    int optionsX = (windowWidth - optionsWidth) / 2;
    int optionsY = 3 * windowHeight / 4 - optionsHeight / 2;

    // 渲染 "Start Game" 菜单项
    if (menuSelection == 0) {
        renderTexture(startGameTextureSelected, renderer, optionsX, optionsY);
    } else {
        renderTexture(startGameTextureUnselected, renderer, optionsX, optionsY);
    }

    // 渲染 "Options" 菜单项
    if (menuSelection == 1) {
        renderTexture(optionsTextureSelected, renderer, optionsX, optionsY + optionsHeight);
    } else {
        renderTexture(optionsTextureUnselected, renderer, optionsX, optionsY + optionsHeight);
    }

    SDL_RenderPresent(renderer);
}



void Game::renderOptionsMenu() {
    int windowWidth = 512;
    int windowHeight = 768;
    if (renderer == nullptr) {
        // 渲染器创建失败，输出错误信息
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (currentState == GameState::OptionsMenu) {
        renderOptionsMenu();
    } else if (currentState == GameState::ControlMenu) {
        renderControlMenu();
    }
}



void Game::renderControlMenu() {
    // 渲染 "Keyboard" 选项
    if (optionsMenuSelection == 0) {
        renderTexture(keyboardTextureSelected, renderer, 100, 100);
    } else {
        renderTexture(keyboardTextureUnselected, renderer, 100, 100);
    }

    // 渲染 "Mouse" 选项
    if (optionsMenuSelection == 1) {
        renderTexture(mouseTextureSelected, renderer, 100, 200);
    } else {
        renderTexture(mouseTextureUnselected, renderer, 100, 200);
    }
}



void Game::handleMainMenuEvents(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                menuSelection = (menuSelection - 1 + 2) % 2;
                break;

            case SDLK_DOWN:
                menuSelection = (menuSelection + 1) % 2;
                break;

            case SDLK_RIGHT:
            case SDLK_RETURN:
                if (menuSelection == 0) {
                    currentState = GameState::Running;
                } else if (menuSelection == 1) {
                    currentState = GameState::OptionsMenu;
                } else if (menuSelection == 2) {
                    isRunning = false;
                }
            case SDLK_SPACE:
                if (menuSelection == 0) {
                    currentState = GameState::Running;
                } else if (menuSelection == 1) {
                    currentState = GameState::OptionsMenu;
                }
                break;

            case SDLK_LEFT:
                currentState = GameState::MainMenu;
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
                    useKeyboardControl = true;
                } else if (controlSelection == 1) {
                    useKeyboardControl = false;
                }
                currentState = GameState::OptionsMenu;
                break;
            case SDLK_ESCAPE:
                currentState = GameState::OptionsMenu;
                break;
            default:
                break;
        }
    }
}




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
    static Uint32 lastEnemySpawnTime = SDL_GetTicks(); // 上次生成敌机的时间
    Uint32 currentTime = SDL_GetTicks();

    // 按照指定间隔生成新敌机
    if (currentTime - lastEnemySpawnTime >= 500 && enemies.size() < 25) {
        int randomX = rand() % GAME_WIDTH;
        int speed = 5;
        enemies.emplace_back(randomX, 0, 500, speed);
        lastEnemySpawnTime = currentTime;
    }

    // 更新敌机状态
    for (auto& enemy : enemies) {
        enemy.update();
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
        case GameState::Running:
            renderGame();
            break;
        default:
            break;
    }

    // 更新屏幕
    SDL_RenderPresent(renderer);
}

void Game::renderGame() {
    // 渲染滚动的背景
    SDL_Rect src_rect = {0, 0, windowWidth, windowHeight};
    SDL_Rect dest_rect1 = {0, m_map.getMap1PosY(), windowWidth, windowHeight};
    SDL_Rect dest_rect2 = {0, m_map.getMap2PosY(), windowWidth, windowHeight};
    SDL_RenderCopy(renderer, bg_texture1, &src_rect, &dest_rect1);
    SDL_RenderCopy(renderer, bg_texture2, &src_rect, &dest_rect2);

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

    // 加载敌机纹理
    SDL_Texture* enemyTexture = IMG_LoadTexture(renderer, "data/img-plane_3.png");
    // 渲染激活状态的敌机
    for (const auto& enemy : enemies) {
        if (enemy.getState() == Enemy::EnemyState::Active) {
            SDL_Rect destRect = enemy.getCollisionRect();

            // 调整目标矩形的宽度和高度以放大敌机贴图
            destRect.w *= 2; // 将敌机宽度放大2倍
            destRect.h *= 2; // 将敌机高度放大2倍
            SDL_RenderCopy(renderer, enemyTexture, nullptr, &destRect);
            SDL_RenderCopy(renderer, enemyTexture, nullptr, &destRect);
            }
        }

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

