#ifndef ANAGRAMMER_TIMER_H
#define ANAGRAMMER_TIMER_H

#include "raylib.h"

class Timer {
private:
    // Images and textures for timer visuals
    Image ImageClock{};
    Texture2D TextureClock{};
    Texture2D TextureTimerBar{};

    // Animation parameters
    int AnimFrames = 0;
    int NextFrameDataOffset = 0;  // Current byte offset to next frame in image.data
    int CurrentAnimFrame = 0;     // Current animation frame to load and draw
    float FrameDelay = 12;        // Frame delay to switch between animation frames
    float FrameCounter = 0;       // General frames counter

    // Timer settings
    const float InitialTimer = 60;
    float TimeLeft{};

public:
    // Constructor to load images and textures
    Timer();

    // Update and draw the timer
    void UpdateTimer(Font LoadedFont);

    // Get the remaining time
    float GetTimeLeft() const;

    // Set the timer to a specific time
    void SetTimer(float time);

    // Get the initial timer value
    float GetInitialTimer() const;

    // Add time to the timer
    void AddToTimer(float seconds);
};

#endif //ANAGRAMMER_TIMER_H