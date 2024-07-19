#ifndef ANAGRAMMER_WORKER_H
#define ANAGRAMMER_WORKER_H

#include "raylib.h"
#include "Timer.h"

class Worker {
private:
    // Images and textures for the worker animation
    Image ImageWorker{};
    Texture2D GifWorker{};
    Texture2D TextureWorkerNoOutline{};
    Texture2D TextureWorkerOutline{};

    // Animation parameters
    int NextFrameDataOffset = 0;  // Current byte offset to next frame in image.data
    int CurrentAnimFrame = 0;     // Current animation frame to load and draw
    int FrameDelay = 8;           // Frame delay to switch between animation frames
    int FrameCounter = 0;         // General frames counter
    int AnimFrames = 0;           // Total number of frames in the animation

    bool IsWorking = false;       // Indicates if the worker is currently working

    Font LoadedFont{};            // Font used for rendering text
    Color HintBoxColor{};         // Color used for hint boxes
    Music TypingSounds{};         // Typing sound effect

    // Draw the worker animation frame
    void DrawGif(bool ShouldPlayAudio);

public:
    // Default constructor
    Worker() = default;

    // Constructor to load images, textures, and sounds
    Worker(Font LoadedFont, Color HintBoxColor);

    // Update and draw the worker
    void UpdateWorker(Timer& timer, bool ShouldPlayAudio);

    // Check if the worker is currently working
    bool CheckIfWorking() const;

    // Set the working state of the worker
    void SetWorkingState(bool working);
};

#endif // ANAGRAMMER_WORKER_H