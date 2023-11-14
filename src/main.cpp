#include <iostream>

#include "game.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    std::cout << "Snake v.0.2\n";

    Game *g = new Game();
    g->Execute();
    return 0;
}
