src/core/Bullet.cpp:5:19: warning: field 'speed' will be initialized after field 'trajectory' [-Wreorder-ctor]
    : x(x), y(y), speed(speed), trajectory(trajectory), state(BulletState::Active) {}
      ~~~~  ~~~~  ^~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~
      trajectory(trajectory) x(x) y(y) speed(speed)
1 warning generated.
clang++ -Wall -g -std=c++11 -fsanitize=address -c src/sdl2/mainSDL.cpp -o obj/Debug/sdl2/mainSDL.o
src/sdl2/mainSDL.cpp:321:12: warning: variable 'gameTime' set but not used [-Wunused-but-set-variable]
    Uint32 gameTime;
           ^
1 warning generated.
clang++ -Wall -g -std=c++11 -fsanitize=address -c src/sdl2/main.cpp -o obj/Debug/sdl2/main.o
clang++ -o bin/Debug/Plane obj/Debug/core/Enhance.o obj/Debug/core/Player.o obj/Debug/core/Drop.o obj/Debug/core/Map.o obj/Debug/core/ScoreManager.o obj/Debug/core/Enemy.o obj/Debug/core/Bomb.o obj/Debug/core/Bullet.o obj/Debug/sdl2/mainSDL.o obj/Debug/sdl2/main.o `sdl2-config --libs` -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -fsanitize=address
Undefined symbols for architecture x86_64:
  "Game::fireMultipleBullets(Bullet::BulletTrajectory, int)", referenced from:
      Game::fireBullet(Bullet::BulletTrajectory) in mainSDL.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
make: *** [bin/Debug/Plane] Error 1