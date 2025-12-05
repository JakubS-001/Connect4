#include "../include/Game.hpp"
#include "../include/UI.hpp"

int main() {
    Game game;
    UI ui(game);
    ui.start();
    return 0;
}
