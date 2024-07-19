#include "CommandPrompt.h"

// Constructor: Load textures, sounds, and fonts for the command prompt
CommandPrompt::CommandPrompt(Font LoadedFontParam) {
    // Load textures for the command prompt and reroll button states
    TextureCMD = LoadTexture("VFX/cmd.png");
    TextureRerrollButtonIdle = LoadTexture("VFX/rerroll.png");
    TextureRerrollButtonPressed = LoadTexture("VFX/rerrollPressed.png");
    TextureRerrollButtonHover = LoadTexture("VFX/rerrollHover.png");
    ImageMatrix = LoadImageAnim("VFX/matrix.gif", &AnimFrames);
    GifMatrix = LoadTextureFromImage(ImageMatrix);

    // Set the loaded font
    LoadedFont = LoadedFontParam;
}

// Check and update the state of the reroll button
void CommandPrompt::CheckRerrollBtnState(Timer &timer) {
    Vector2 ButtonPosition = {990, 740};
    Rectangle ButtonCollisionRectangle = {ButtonPosition.x - 10, ButtonPosition.y - 20, 70, 70};

    DrawRectangle(static_cast<int>(ButtonPosition.x) + 10, static_cast<int>(ButtonPosition.y) + 70, 50, 20, {40, 44, 52, static_cast<unsigned char>(alpha * 255)});
    DrawText("Tab", static_cast<int>(ButtonPosition.x) + 10, static_cast<int>(ButtonPosition.y) + 70, 20, WhiteAlpha);

    if (CheckCollisionPointRec(GetMousePosition(), ButtonCollisionRectangle)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            // Button is pressed
            DrawTextureV(TextureRerrollButtonPressed, ButtonPosition, WhiteAlpha);
        } else {
            // Button is hovered
            DrawTextureV(TextureRerrollButtonHover, ButtonPosition, WhiteAlpha);
            DrawRectangle(GetMouseX() + 25, GetMouseY() + 45, 95, 30, {40, 44, 52, static_cast<unsigned char>(alpha * 255)});
            DrawTextEx(LoadedFont, "Reroll", {static_cast<float>(GetMouseX() + 30), static_cast<float>(GetMouseY() + 50)}, 20, 0, {255, 0, 0, static_cast<unsigned char>(alpha * 255)});
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            // Generate a new word and reduce the timer
            GenerateWord();
            timer.AddToTimer(-5);
            PlayerInput.clear();
        }
    } else {
        // Button is idle
        DrawTextureV(TextureRerrollButtonIdle, ButtonPosition, WhiteAlpha);
    }

    if (IsKeyDown(KEY_TAB)) {
        // Tab key is pressed
        DrawTextureV(TextureRerrollButtonPressed, ButtonPosition, WhiteAlpha);
    }
    if (IsKeyReleased(KEY_TAB)) {
        // Generate a new word and reduce the timer
        GenerateWord();
        timer.AddToTimer(-5);
        PlayerInput.clear();
    }
}

// Draw the command prompt interface
void CommandPrompt::DrawCMD() {
    Vector2 CMDPosition = {270, 250};

    // Draw the command prompt background and animated matrix effect
    DrawTextureV(TextureCMD, CMDPosition, WhiteAlpha);
    DrawTexture(GifMatrix, static_cast<int>(CMDPosition.x) + 10, static_cast<int>(CMDPosition.y) + 50, {255, 255, 255, static_cast<unsigned char>(alpha * 70)});
    DrawTextEx(LoadedFont, ShuffledWord.c_str(), {ShuffledWordX, ShuffledWordY}, 75, 15, GreenAlpha);
    DrawLineEx({CMDPosition.x + 10, CMDPosition.y + 480}, {CMDPosition.x + 805, CMDPosition.y + 480}, 5, GreenAlpha);

    // Blinking underscore effect
    if (!((underscoreFrames / 20) % 2) && PlayerInput.length() != CorrectWord.length()) {
        DrawText("_", 710 + MeasureText(PlayerInput.c_str(), 45), 780, 40, GreenAlpha);
    }

    underscoreFrames++;
    FrameCounter++;

    if (FrameCounter >= FrameDelay) {
        // Move to the next frame of the animation
        CurrentAnimFrame++;
        if (CurrentAnimFrame >= AnimFrames) {
            CurrentAnimFrame = 0; // Loop back to the first frame
        }

        // Get memory offset for the next frame data
        NextFrameDataOffset = ImageMatrix.width * ImageMatrix.height * 4 * CurrentAnimFrame;

        // Update the GPU texture data with the next frame image data
        UpdateTexture(GifMatrix, (static_cast<unsigned char *>(ImageMatrix.data)) + NextFrameDataOffset);

        FrameCounter = 0;
    }
}

// Generate a new word and shuffle it
void CommandPrompt::GenerateWord() {
    std::ifstream Wordlist("Other/wordlist.txt");

    size_t lines = std::count(std::istreambuf_iterator<char>(Wordlist), std::istreambuf_iterator<char>(), '\n');

    int RandomLine = GetRandomValue(0, static_cast<int>(lines) - 1);

    Wordlist.clear(); // Clear EOF and fail bits
    Wordlist.seekg(0); // Rewind to the beginning of the file

    for (int CurrentLine = 0; CurrentLine != RandomLine && std::getline(Wordlist, ShuffledWord); CurrentLine++) {}

    Wordlist.close();

    CorrectWord = ShuffledWord;

    do {
        std::shuffle(ShuffledWord.begin(), std::prev(ShuffledWord.end()), std::mt19937 {std::random_device{}()});
    } while (ShuffledWord == CorrectWord);

    ShuffledWordX = static_cast<float>(GetRandomValue(300, 830 - MeasureText(ShuffledWord.c_str(), 75)));
    ShuffledWordY = static_cast<float>(GetRandomValue(300, 600));
}

// Handle player input
void CommandPrompt::UpdatePlayerInput(Timer &timer, int *score) {
    int key = GetCharPressed();

    // Only allow keys in range [97..122]
    if ((key > 96) && (key < 123) && PlayerInput.length() < ShuffledWord.length()) {
        PlayerInput.push_back(static_cast<char>(key));
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !PlayerInput.empty()) {
        PlayerInput.pop_back();
    }

    DrawTextEx(LoadedFont, "C:\\Users\\Default>", {300, 770}, 35, 0, GreenAlpha);
    DrawTextEx(LoadedFont, PlayerInput.c_str(), {700, 765}, 40, 0, GreenAlpha);

    if (PlayerInput == CorrectWord) {
        GetNewWord();
        timer.AddToTimer(static_cast<float>(CorrectWord.length()));
        *score += static_cast<int>(CorrectWord.length());
    }
}

// Fade in or out the command prompt based on the timer
void CommandPrompt::FadeInOrOut(Timer& timer) {
    float fadeSpeed = 0.03;

    if (timer.GetTimeLeft() > 0) {
        if (alpha < 0.99) {
            alpha += fadeSpeed;
        }
    } else {
        if (alpha > 0.01) {
            alpha -= fadeSpeed;
        }
    }
}

// Get a new word for the game
void CommandPrompt::GetNewWord() {
    GenerateWord();
    underscoreFrames = 0;
    PlayerInput.clear();
}

// Update the command prompt interface
void CommandPrompt::UpdateCMD(Timer &timer, int *score) {
    GreenAlpha = {0, 255, 0, static_cast<unsigned char>(alpha * 255)};
    WhiteAlpha = {255, 255, 255, static_cast<unsigned char>(alpha * 255)};

    if (timer.GetTimeLeft() <= 0) {
        GetNewWord();
    }

    FadeInOrOut(timer);
    DrawCMD();
    CheckRerrollBtnState(timer);
    UpdatePlayerInput(timer, score);
}