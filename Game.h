#ifndef ANAGRAMMER_GAME_H
#define ANAGRAMMER_GAME_H

#include "raylib.h"
#include "Worker.h"
#include "Timer.h"
#include "CommandPrompt.h"


class Game {
private:
    // Textures used in the game
    Texture2D Background{};
    Texture2D ExitButtonIdle{};
    Texture2D ExitButtonHover{};
    Texture2D AudioButtonIdle{};
    Texture2D AudioButtonHover{};
    Texture2D TextureCursor{};
    Texture2D TextureBoss{};
    Texture2D TextureHacker{};

    // Game objects
    Worker worker;
    Timer timer = Timer();
    CommandPrompt CMD;

    // Font used for text rendering
    Font LoadedFont{};

    // Flags to track game state
    bool ShouldExit{};
    bool ShouldPlayAudio{};
    bool ShouldPlayIntro{};

    // Color for hint boxes
    Color HintBoxColor = {40, 44, 52, 255};

    // Game scores
    int Score = 0;
    int Highscore = LoadHighscoreFromFile();

    Sound talking1{};
    Sound talking2{};

    Music BackgroundMusic{};

    int FramesCounter = 0;
    int CurrentMessage = 0;

    const char *messages [7] = {
            "What have you done?!\nOur entire system is in chaos!",
            "Oh, don`t be so dramatic!\nYour precious data is just...\nrearranged.\nThink of it as a puzzle\nwaiting to be solved.",
            "A puzzle?! This isn`t some game!\nWe`re losing clients,\nand if this continues,\nwe`ll go under!",
            "Exactly!\nThat`s the beauty of it.\nYour team has to decrypt\nthe mess I`ve made.\nEvery scrambled word is a clue.",
            "He will.\nI won`t let you win.\nWe`ll decode your tricks\nand take back control!",
            "Let`s see if he`s up to the task.\nTime is ticking, and I`ve shuffled\nmore than just words.\nGood luck!",
            "We need to act fast.\nEvery second counts...\nLet`s get to work."
    };

    // Save the highscore to a file
    void SaveHighscoreToFile() const;

    // Load the highscore from a file
    int LoadHighscoreFromFile();

    // Load the window icon
    static void LoadWindowIcon();

    // Load all textures
    void LoadAssets();

    // Draw the score on the screen
    void DrawScore();

    // Update and draw all game objects
    void UpdateObjects();

    // Update buttons
    void UpdateExitButton();
    void UpdateAudioButton();

    void PlayIntro();

public:
    // Constructor: Initializes the game, sets up the window and loads resources
    Game();

    // Destructor: Unload resources
    ~Game();

    // Main game update loop
    void Update();
};

#endif //ANAGRAMMER_GAME_H
