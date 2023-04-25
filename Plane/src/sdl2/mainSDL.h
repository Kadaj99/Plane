#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <cmath>

#include "../core/Map.h" 
#include "../core/ScoreManager.h"
#include "../core/Drop.h"

class Game {
public:
    // 构造函数和析构函数
    Game();
    Game(const std::string& title, int width, int height);
    ~Game();

    // 初始化和清理函数
    bool initSDL();
    void cleanUpSDL();

    // 游戏主循环
    void run();

    // 事件处理函数
    void handleEvents();

    // 更新函数
    void update();
    void updateBombs();
    void checkCollisions(); // 添加碰撞检测函数声明
    void fireBullet();
    void resetGameObjects();
    void updateMusic();

    // 渲染函数
    void render();
    void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y);
    void renderGame();
    void renderMainMenu();
    void renderOptionsMenu();
    void renderControlMenu();
    void renderKeyboardControlMenu();
    void renderMouseControlMenu();
    void renderPausedScreen();
    void renderStop();
    void renderPausedScore();
    void renderScore();
    void renderGameOver();
    void renderLevelMenu();

    // 游戏状态处理函数
    void handleMainMenuEvents(SDL_Event &event);
    void handleOptionsMenuEvents(SDL_Event &event);
    void handleRunningEvents(SDL_Event &event);
    void handleControlMenuEvents(SDL_Event &event);
    void handlePausedEvents(SDL_Event &event);
    void handleKeyboardControlMenuEvents(SDL_Event& event);
    void handleMouseControlMenuEvents(SDL_Event& event);
    void handleGameOverEvents(SDL_Event &event, Uint32 gameTime);
    void handleMenuEvents(SDL_Event &event);
    void handleLevelMenuEvents(const SDL_Event& event);
    


    // getter 和 setter 函数
    int getPlayerScore() const;
    void setPlayerScore(int score);
    float getGameTime() const;

private:
    // 成员变量
    SDL_Window *window;
    SDL_Renderer *renderer;
    int windowWidth;
    int windowHeight;
    std::string title;
    bool isRunning;

    Map m_map;
    int scroll_speed = 3;
    SDL_Texture* bg_texture1_easy;
    SDL_Texture* bg_texture1_normal;
    SDL_Texture* bg_texture1_hard;
    SDL_Texture* bg_texture2_easy;
    SDL_Texture* bg_texture2_normal;
    SDL_Texture* bg_texture2_hard;
    SDL_Texture *texture;
    SDL_Texture *mainMenuBackgroundTexture;

    
    enum class LevelSelection {
    Easy,
    Normal,
    Hard
    };
    Uint32 lastEnemySpawnTime; // 上次生成敌机的时间
    int enemySpawnInterval;
    int maxEnemyCount;
    LevelSelection currentLevel = LevelSelection::Normal;


    int playerScore = 0;

    Mix_Music *menuMusic = nullptr;
    Mix_Music *gameMusic = nullptr;
    Mix_Chunk *bombSound = nullptr;
    bool isMusicPaused;

    TTF_Font *pauseFont;
    TTF_Font *scoreFont;
    TTF_Font *gameOverFont;

    Player player;
    SDL_Texture* playerTexture;
    ScoreManager scoreManager;
    SDL_Texture* bulletTexture;
    std::vector<Bullet> bullets;
    Uint32 lastFireTime;
    const int fireInterval = 500;
    const int magazineSize = 50;
    void fireBullet(Bullet::BulletTrajectory trajectory);
    void fireSingleBullet(Bullet::BulletTrajectory trajectory);
    void fireMultipleBullets(Bullet::BulletTrajectory trajectory, int bulletCount);
    Bullet::BulletTrajectory currentTrajectory;



    std::vector<Enemy> enemies;
    SDL_Texture* enemyTexture;

    std::vector<Bomb> bombs;
    std::vector<SDL_Texture*> bombTextures;

    bool autoFire;
    Uint32 autoFireTimer = 0;
    const Uint32 autoFireInterval = 500;

    std::vector<Drop> drops;
    Uint32 lastDropSpawnTime;
    const Uint32 dropSpawnInterval = 10000; // 10秒

    bool useKeyboardControl;
    enum class ControlType {
        Keyboard,
        Mouse
    };

    ControlType currentControlType = ControlType::Keyboard;
    
    enum class GameState {
        MainMenu,
        Level,
        OptionsMenu,
        ControlMenu,
        MouseControl,
        KeyboardControl,
        Running,
        Pause,
        GameOver
};
    GameState currentState;
    int menuSelection;
    int optionsMenuSelection;
    int controlSelection;
    int controlMenuSelection;
    int levelMenuSelection;

    // 纹理对象
    SDL_Texture *titleTexture;
    SDL_Texture *stopTexture;
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
    SDL_Texture *levelTextureSelected;
    SDL_Texture *levelTextureUnselected;
    SDL_Texture *easyTextureSelected;
    SDL_Texture *easyTextureUnselected;
    SDL_Texture *normalTextureSelected;
    SDL_Texture *normalTextureUnselected;
    SDL_Texture *hardTextureSelected;
    SDL_Texture *hardTextureUnselected;

    // 函数声明
    void playMusic(Mix_Music* music);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();

    };
