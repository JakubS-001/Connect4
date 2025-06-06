#include <allegro5/allegro.h>
#include "gui.h"

int main() {
    if (!init_gui()) {
        return -1;
    }
    game_loop();
    shutdown_gui();
    return 0;
}
