#include "raylib.h"
#include "Game.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitAudioDevice();

    InitWindow(1600, 960, "The anagrammer");

    Game game = Game();
    game.Update();
    game.~Game();

    CloseWindow();
    return 0;
}