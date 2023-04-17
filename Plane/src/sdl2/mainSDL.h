#include <SDL2/SDL.h>
#include "../core/Map.h" // 引入 Map 类
#include "../core/Player.h" // 在 Game 类的实现文件中引入 Player 类
#include "../core/Bullet.h" // 包含 Bullet 类的头文件
#include "../core/Bomb.h"
#include "../core/Enemy.h"
#include <SDL2/SDL_image.h>
#include <vector>


class Game {
public:
    Game(const std::string &title, int width, int height);
    ~Game();
    bool initSDL();
    void cleanUpSDL();
    void run();
    void handleEvents();
    void update();
    void render();

    void updateBombs();

    void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y);




private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    int windowWidth;
    int windowHeight;
    std::string title;
    bool isRunning;
    void renderGame();

    void checkCollisions(); // 添加碰撞检测函数声明
    // 添加地图对象和滚动速度
    Map m_map;
    int scroll_speed = 3; // 设置滚动速度

    // 添加背景图片纹理
    SDL_Texture *bg_texture1;
    SDL_Texture *bg_texture2;

    // 添加玩家图片纹理
    Player player; // 添加 Player 对象
    SDL_Texture* playerTexture; // 添加玩家纹理

    //Bullet bullet;
    SDL_Texture* bulletTexture; // 子弹纹理
    std::vector<Bullet> bullets; // 子弹对象容器
    Uint32 lastFireTime;         // 上次发射子弹的时间
    const int fireInterval = 200; // 子弹发射间隔（毫秒）
    const int magazineSize = 10;  // 弹夹容量

    //Enemy enemy;// 添加 Enemy 对象
    std::vector<Enemy> enemies;//添加敌机向量
    // 添加敌机图片纹理
    SDL_Texture* enemyTexture;

    std::vector<Bomb> bombs;
    std::vector<SDL_Texture*> bombTextures; // 爆炸特效的纹理列表

    //Menu
    enum class GameState {
         MainMenu,
         ControlMenu,
         OptionsMenu,
         Running,
         Paused
    };

    GameState currentState;
    int menuSelection;
    int optionsMenuSelection;

    // 用户选项
    bool autoFire;
    bool useKeyboardControl; // 定义 useKeyboardControl 变量

    // ...

    void renderMainMenu();
    void renderOptionsMenu();
    void renderControlMenu();
    void handleMainMenuEvents(SDL_Event &event);
    void handleOptionsMenuEvents(SDL_Event &event);
    void handleRunningEvents(SDL_Event &event);
    void handleControlMenuEvents(SDL_Event &event);


    SDL_Texture *titleTexture;
    SDL_Texture *startGameTextureSelected;
    SDL_Texture *startGameTextureUnselected;
    SDL_Texture *optionsTextureSelected;
    SDL_Texture *optionsTextureUnselected;
    SDL_Texture *controlsTextureSelected;
    SDL_Texture *controlsTextureUnselected;

    SDL_Texture *keyboardTextureSelected;
    SDL_Texture *keyboardTextureUnselected;
    SDL_Texture *mouseTextureSelected;
    SDL_Texture *mouseTextureUnselected;
    int controlSelection;
};


