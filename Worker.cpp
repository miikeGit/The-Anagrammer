#include "Worker.h"

// Constructor: Load images, textures, and sounds for the worker
Worker::Worker(Font LoadedFontParam, Color HintBoxColorParam) {
    // Load the animated worker image and its frames
    ImageWorker = LoadImageAnim("VFX/worker.gif", &AnimFrames);
    GifWorker = LoadTextureFromImage(ImageWorker);
    TextureWorkerNoOutline = LoadTextureFromImage(ImageWorker);
    TextureWorkerOutline = LoadTexture("VFX/workerOutline.png");

    // Load the typing sound effect
    TypingSounds = LoadMusicStream("SFX/typing.mp3");

    // Set the loaded font and hint box color
    LoadedFont = LoadedFontParam;
    HintBoxColor = HintBoxColorParam;
}

void Worker::UpdateWorker(Timer& timer, bool ShouldPlayAudio) {
    // Define the button position and collision rectangle
    Vector2 ButtonPosition = {950, 270};
    Rectangle ButtonCollisionRectangle = {ButtonPosition.x + 90, ButtonPosition.y + 145, 280, 260};

    if (IsWorking) {
        DrawGif(ShouldPlayAudio);
    } else if (CheckCollisionPointRec(GetMousePosition(), ButtonCollisionRectangle)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && timer.GetTimeLeft() <= 0) {
            // If the button is pressed and the timer has expired, start working
            DrawGif(ShouldPlayAudio);
            IsWorking = true;
            timer.SetTimer(timer.GetInitialTimer());
            PlayMusicStream(TypingSounds);
        } else {
            // If the cursor is over the button, draw the outline and hint text
            DrawTextureV(TextureWorkerOutline, ButtonPosition, WHITE);
            DrawRectangle(GetMouseX() + 25, GetMouseY() + 45, 80, 30, HintBoxColor);
            DrawTextEx(LoadedFont, "Start", {static_cast<float>(GetMouseX() + 30), static_cast<float>(GetMouseY() + 50)}, 20, 0, YELLOW);
        }
    } else {
        // If idle, draw the worker without the outline
        DrawTextureV(TextureWorkerNoOutline, ButtonPosition, WHITE);
    }
}

void Worker::DrawGif(bool ShouldPlayAudio) {
    // Draw the current frame of the worker animation
    DrawTexture(GifWorker, 950, 270, WHITE);
    FrameCounter++;

    if (FrameCounter >= FrameDelay) {
        // Move to the next frame
        CurrentAnimFrame++;

        if (CurrentAnimFrame < 12 && ShouldPlayAudio) {
            // Update the music stream if the worker is typing
            UpdateMusicStream(TypingSounds);
        }

        // Loop back to the first frame if the end is reached
        if (CurrentAnimFrame >= AnimFrames) {
            CurrentAnimFrame = 0;
        }

        // Calculate memory offset for the next frame data
        NextFrameDataOffset = ImageWorker.width * ImageWorker.height * 4 * CurrentAnimFrame;

        // Update the GPU texture data with the next frame image data
        UpdateTexture(GifWorker, ((unsigned char*)ImageWorker.data) + NextFrameDataOffset);

        FrameCounter = 0;
    }
}

// Check if the worker is currently working
bool Worker::CheckIfWorking() const {
    return IsWorking;
}

// Set the working state of the worker
void Worker::SetWorkingState(bool working) {
    IsWorking = working;
}