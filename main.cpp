#include "Game.h"

int main(int argc, char* argv[]) {
    Game game;

    if (InitGame(game)) {
        RunGame(game);
    }

    CleanupGame(game);
    return 0;
}
