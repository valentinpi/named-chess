#include "Game.hpp"

int main(int argc, char *argv[])
{
    Game game(720, 720);

    if (!game.init())
        return EXIT_FAILURE;

    game.run();

    return EXIT_SUCCESS;
}
