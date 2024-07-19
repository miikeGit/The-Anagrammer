#ifndef ANAGRAMMER_COMMANDPROMPT_H
#define ANAGRAMMER_COMMANDPROMPT_H

#include "raylib.h"
#include "Timer.h"
#include <algorithm>
#include <iterator>
#include <random>
#include <fstream>

class CommandPrompt {
private:
    // Images and textures for the command prompt and reroll button
    Image ImageMatrix{};
    Texture2D TextureCMD{};
    Texture2D TextureRerrollButtonIdle{};
    Texture2D TextureRerrollButtonHover{};
    Texture2D TextureRerrollButtonPressed{};
    Texture2D GifMatrix{};

    // Alpha value for fade effect
    float alpha = 0.01;

    // Colors with alpha values for fade effect
    Color GreenAlpha{};
    Color WhiteAlpha{};

    // Animation parameters
    int NextFrameDataOffset = 0;  // Current byte offset to next frame in image.data
    int CurrentAnimFrame = 0;     // Current animation frame to load and draw
    int FrameDelay = 8;           // Frame delay to switch between animation frames
    int FrameCounter = 0;         // General frames counter
    int AnimFrames = 0;           // Total number of frames in the animation
    int underscoreFrames = 0;     // Frame counter for underscore blinking effect

    // Position for the shuffled word
    float ShuffledWordX{};
    float ShuffledWordY{};

    // Font for rendering text
    Font LoadedFont{};

    // Words for the game
    std::string CorrectWord;
    std::string ShuffledWord;
    std::string PlayerInput;

    // Draw the command prompt interface
    void DrawCMD();

    // Check and update the state of the reroll button
    void CheckRerrollBtnState(Timer &timer);

    // Generate a new word and shuffle it
    void GenerateWord();

    // Handle player input
    void UpdatePlayerInput(Timer &timer, int *score);

    // Fade in or out the command prompt
    void FadeInOrOut(Timer& timer);

    // Get a new word for the game
    void GetNewWord();

public:
    // Default constructor
    CommandPrompt() = default;

    // Constructor to load textures, sounds, and fonts
    explicit CommandPrompt(Font LoadedFont);

    // Update the command prompt interface
    void UpdateCMD(Timer &timer, int *score);
};

#endif // ANAGRAMMER_COMMANDPROMPT_H