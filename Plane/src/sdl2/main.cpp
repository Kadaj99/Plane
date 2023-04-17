#include "mainSDL.h"

int main(int argc, char* argv[]) {
    const std::string title = "Your Game Title";
    const int windowWidth = 800;
    const int windowHeight = 600;

    Game game(title, windowWidth, windowHeight);

    game.run();

    return 0;
}
