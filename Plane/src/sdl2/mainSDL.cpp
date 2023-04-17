#include "mainSDL.h"
using namespace std;
#include <iostream>

Game::Game(const std::string& title, int width, int height)
    : windowWidth(512), windowHeight(768), title("Dogfighter"), isRunning(true), player(windowWidth, windowHeight)
    {
    if (!initSDL()) {
        isRunning = false;
    }
}

Game::~Game() {
    cleanUpSDL();
}

//data/img_bg_level_1.jpg


bool Game::initSDL() {

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
        bullets.back().setState(Bullet::State::Inactive);
    }

    return true;
}


void Game::cleanUpSDL() {
    // Add clean up code for SDL_image, SDL_ttf, and SDL_mixer here
    SDL_DestroyTexture(bg_texture1);
    SDL_DestroyTexture(bg_texture2);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(bulletTexture);
    SDL_Quit();
}

void Game::run() {
    while (isRunning) {
        handleEvents();
        update();
        render();
        SDL_Delay(16); // Approximately 60 FPS
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        // 添加其他事件处理代码
         // 检查是否按下空格键
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastFireTime > fireInterval) {
            lastFireTime = currentTime;

            // 寻找第一个非激活状态的子弹
            for (Bullet &bullet : bullets) {
                if (bullet.getState() == Bullet::State::Inactive) {
                    // 设置子弹位置为飞机位置
                    bullet.setX(player.getX() + 25); // 假设飞机宽度为 50
                    bullet.setY(player.getY());

                    // 激活子弹
                    bullet.setState(Bullet::State::Active);

                    // 打破循环，只激活一个子弹
                    break;
                }
            }
        }
    }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    player.moveLeft(5);
                    break;
                case SDLK_RIGHT:
                    player.moveRight(5);
                    break;
                case SDLK_UP:
                    player.moveUp(5);
                    break;
                case SDLK_DOWN:
                    player.moveDown(5);
                    break;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEMOTION) {
            if (event.button.button == SDL_BUTTON_LEFT || event.type == SDL_MOUSEMOTION) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                player.setX(mouseX - 25); // 假设飞机宽度为 50
                player.setY(mouseY - 25); // 假设飞机高度为 50
            }
        }
    }
}

void Game::update() {

    // Add game update logic here
    m_map.mapPosition(scroll_speed);
    // 更新子弹
    for (Bullet &bullet : bullets) {
        if (bullet.getState() == Bullet::State::Active) {
            bullet.update();

            // 检查子弹是否超出窗口边界
            if (bullet.getY() < 0) {
            bullet.setState(Bullet::State::Inactive);
            }
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Add rendering code here

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
        if (bullet.getState() == Bullet::State::Active) {
            // 使用 bulletTexture 渲染子弹
            SDL_Rect dstRect;
            dstRect.x = bullet.getX();
            dstRect.y = bullet.getY();
            SDL_QueryTexture(bulletTexture, nullptr, nullptr, &dstRect.w, &dstRect.h);
            SDL_RenderCopy(renderer, bulletTexture, nullptr, &dstRect);
        }
    }


    SDL_RenderPresent(renderer);
}
