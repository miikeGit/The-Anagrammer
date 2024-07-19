#include "Timer.h"

// Constructor: Load images and textures for the timer
Timer::Timer() {
    ImageClock = LoadImageAnim("VFX/clock.gif", &AnimFrames);
    TextureClock = LoadTextureFromImage(ImageClock);
    TextureTimerBar = LoadTexture("VFX/timerBar.png");
}

// Update and draw the timer
void Timer::UpdateTimer(Font LoadedFont) {
    // Calculate the height of the bar based on remaining time
    int barHeight = static_cast<int>(400 * (TimeLeft / InitialTimer));

    // Draw the decreasing bar
    DrawRectangleGradientV(140, (GetScreenHeight()) - barHeight - 275, 80, barHeight, GREEN, RED);
    DrawTexture(TextureTimerBar, 90, 245, WHITE);
    DrawTexture(TextureClock, 40, 630, WHITE);

    // Handle frame animation for the clock
    FrameCounter++;

    if (FrameCounter >= FrameDelay && TimeLeft > 0) {
        // Move to the next frame
        CurrentAnimFrame++;

        // Loop back to the first frame if needed
        if (CurrentAnimFrame >= AnimFrames) {
            CurrentAnimFrame = 0;
        }

        // Calculate memory offset for next frame data
        NextFrameDataOffset = ImageClock.width * ImageClock.height * 4 * CurrentAnimFrame;

        // Update texture with next frame data
        UpdateTexture(TextureClock, ((unsigned char *)ImageClock.data) + NextFrameDataOffset);

        FrameCounter = 0;
    }

    // Draw the remaining time
    DrawTextEx(LoadedFont, TextFormat("%.1f", TimeLeft), {20, 580}, 40, 0, WHITE);
}

// Get the remaining time
float Timer::GetTimeLeft() const {
    return TimeLeft;
}

// Set the timer to a specific time
void Timer::SetTimer(float time) {
    TimeLeft = time;
}

// Get the initial timer value
float Timer::GetInitialTimer() const {
    return InitialTimer;
}

// Add time to the timer
void Timer::AddToTimer(float seconds) {
    TimeLeft += seconds;
    if (TimeLeft > InitialTimer) {
        TimeLeft = InitialTimer;
    }
    if (TimeLeft <= 0) {
        TimeLeft = 0;
    }
}