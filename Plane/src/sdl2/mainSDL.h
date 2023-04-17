#include <SDL2/SDL.h>
#include "../core/Map.h" // 引入 Map 类
#include "../core/Player.h" // 在 Game 类的实现文件中引入 Player 类
#include "../core/Bullet.h" // 包含 Bullet 类的头文件
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

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    int windowWidth;
    int windowHeight;
    std::string title;
    bool isRunning;

    // 添加地图对象和滚动速度
    Map m_map;
    int scroll_speed = 3; // 设置滚动速度

    // 添加背景图片纹理
    SDL_Texture *bg_texture1;
    SDL_Texture *bg_texture2;

    // 添加玩家图片纹理
    Player player; // 添加 Player 对象
    SDL_Texture *playerTexture; // 添加玩家纹理

    SDL_Texture* bulletTexture; // 子弹纹理
    std::vector<Bullet> bullets; // 子弹对象容器
    Uint32 lastFireTime;         // 上次发射子弹的时间
    const int fireInterval = 200; // 子弹发射间隔（毫秒）
    const int magazineSize = 10;  // 弹夹容量
};
