#include "mainSDL.h"

int main(int argc, char* argv[]) {
    const std::string title = "Your Game Title";
    const int windowWidth = 800;
    const int windowHeight = 600;

    Game game(title, windowWidth, windowHeight);

    game.run();

    return 0;
}

/*
||=== Build: Debug in Plane (compiler: GNU GCC Compiler) ===|
/home/kadaj/Doc/Plane/src/sdl2/mainSDL.cpp||In member function ‘void Game::run()’:|
/home/kadaj/Doc/Plane/src/sdl2/mainSDL.cpp|231|warning: enumeration value ‘ControlMenu’ not handled in switch [-Wswitch]|
/home/kadaj/Doc/Plane/src/sdl2/mainSDL.cpp|231|warning: enumeration value ‘Paused’ not handled in switch [-Wswitch]|
/home/kadaj/Doc/Plane/src/sdl2/mainSDL.cpp||In member function ‘void Game::handleRunningEvents(SDL_Event&)’:|
/home/kadaj/Doc/Plane/src/sdl2/mainSDL.cpp|302|warning: comparison of integer expressions of different signedness: ‘Uint32’ {aka ‘unsigned int’} and ‘const int’ [-Wsign-compare]|
||=== Build finished: 0 error(s), 3 warning(s) (0 minute(s), 1 second(s)) ===|
||=== Run: Debug in Plane (compiler: GNU GCC Compiler) ===|
*/
