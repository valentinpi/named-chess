#include "Game.hpp"

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    Game game(720, 720);

    if (!game.is_initialized())
        return EXIT_FAILURE;

    game.run();

    return EXIT_SUCCESS;
}
