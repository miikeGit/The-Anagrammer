#include "Game.h"

// Helper functions
namespace {
    std::string FormatWithLeadingZeros(int Number) {
        std::string NumberText = std::to_string(Number);
        unsigned int LeadingZeros = 4 - NumberText.length();
        return std::string(LeadingZeros, '0') + NumberText;
    }

    std::string xorEncryptDecrypt(const std::string& Input, const std::string& Key) {
        std::string Output = Input;
        size_t KeyLength = Key.length();
        for (size_t i = 0; i < Input.size(); ++i) {
            Output[i] = static_cast<char>(Input[i] ^ Key[i % KeyLength]);
        }
        return Output;
    }
}

// Constructor: Initializes the game, sets up the window and loads resources
Game::Game() {
    SetTargetFPS(60);       // Set the frame rate
    HideCursor();           // Hide the mouse cursor
    LoadWindowIcon();       // Load the window icon
    LoadAssets();         // Load all game assets

    ShouldPlayAudio = true; // Set default audio state
    ShouldExit = false;     // Set default exit state

    worker = Worker(LoadedFont, HintBoxColor);
    CMD = CommandPrompt(LoadedFont);

    PlayMusicStream(BackgroundMusic);
}

Game::~Game() {
    if (Score > Highscore)
        Highscore = Score;
    SaveHighscoreToFile();

    worker.~Worker();
    timer.~Timer();
    CMD.~CommandPrompt();

    UnloadTexture(Background);
    UnloadTexture(ExitButtonIdle);
    UnloadTexture(ExitButtonHover);
    UnloadTexture(AudioButtonIdle);
    UnloadTexture(AudioButtonHover);
    UnloadTexture(TextureCursor);
    UnloadTexture(TextureBoss);
    UnloadTexture(TextureHacker);
}

// Load the window icon from a file
void Game::LoadWindowIcon() {
    Image WindowIcon = LoadImage("VFX/logo.png");
    SetWindowIcon(WindowIcon);
    UnloadImage(WindowIcon);
}

// Load all necessary textures and fonts
void Game::LoadAssets() {
    Background = LoadTexture("VFX/background.png");
    ExitButtonIdle = LoadTexture("VFX/exitArrow.png");
    ExitButtonHover = LoadTexture("VFX/exitArrowHover.png");
    AudioButtonIdle = LoadTexture("VFX/speaker.png");
    AudioButtonHover = LoadTexture("VFX/speakerHover.png");
    TextureCursor = LoadTexture("VFX/cursor.png");
    TextureBoss = LoadTexture("VFX/boss.png");
    TextureHacker = LoadTexture("VFX/hacker.png");

    LoadedFont = LoadFont("Other/Daydream.ttf");

    talking1 = LoadSound("SFX/sans1.mp3");
    talking2 = LoadSound("SFX/sans2.mp3");

    BackgroundMusic = LoadMusicStream("SFX/bgMusic.mp3");
}

// Update and draw all game objects
void Game::UpdateObjects() {
    ClearBackground({16,16,16,255});
    DrawTexture(Background, 0, 0, WHITE);  // Draw background
    DrawScore();                           // Draw score
    UpdateExitButton();                    // Update exit button state
    UpdateAudioButton();                   // Update audio button state

    // Update worker, timer, cmd, and cursor
    worker.UpdateWorker(timer, ShouldPlayAudio);
    timer.UpdateTimer(LoadedFont);
    CMD.UpdateCMD(timer, &Score);
    DrawTexture(TextureCursor, static_cast<int>(GetMousePosition().x), static_cast<int>(GetMousePosition().y), WHITE);
}

// Draw the current score and highscore on the screen
void Game::DrawScore() {
    float FontSize = 70;
    float Spacing = 0;

    // Draw the score
    DrawTextEx(LoadedFont, "SCORE:", {10, 10}, FontSize, Spacing, WHITE);
    DrawTextEx(LoadedFont, FormatWithLeadingZeros(Score).c_str(),
               {30.0f + static_cast<float>(MeasureText("SCORE:", static_cast<int>(FontSize))), 10}, FontSize, Spacing, WHITE);

    // Draw the highscore
    DrawTextEx(LoadedFont, "HIGHSCORE:", {950, 10}, FontSize, Spacing, WHITE);
    DrawTextEx(LoadedFont, FormatWithLeadingZeros(Highscore).c_str(),
               {980.0f + static_cast<float>(MeasureText("HIGHSCORE:", static_cast<int>(FontSize))), 10}, FontSize, Spacing, WHITE);
}

// Update the state of the exit button based on user interaction
void Game::UpdateExitButton() {
    Vector2 ButtonPosition = {460, 870};
    Rectangle ButtonCollisionRectangle = {ButtonPosition.x - 10, ButtonPosition.y - 20, 100, 60};

    // Check if the cursor is over the button
    if (CheckCollisionPointRec(GetMousePosition(), ButtonCollisionRectangle)) {

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {  // Check if the button is clicked
            ShouldExit = true;
        } else {  // Hover state
            DrawTextureV(ExitButtonHover, ButtonPosition, WHITE);
            DrawRectangle(GetMouseX() + 25, GetMouseY() + 45, 60, 30, HintBoxColor);
            DrawTextEx(LoadedFont, "Exit", {static_cast<float>(GetMouseX()) + 30, static_cast<float>(GetMouseY()) + 50}, 20, 0, YELLOW);
        }

    } else {
        DrawTextureV(ExitButtonIdle, ButtonPosition, WHITE);  // Default state
    }
}

// Update the state of the audio button based on user interaction
void Game::UpdateAudioButton() {
    Vector2 ButtonPosition = {790, 300};
    Rectangle ButtonCollisionRectangle = {ButtonPosition.x - 10, ButtonPosition.y - 20, 110, 110};

    DrawTextureV(AudioButtonIdle, ButtonPosition, WHITE); // Default state

    // Check if the cursor is over the button
    if (CheckCollisionPointRec(GetMousePosition(), ButtonCollisionRectangle)) {

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {  // Check if the button is clicked
            ShouldPlayAudio = !ShouldPlayAudio;  // Toggle audio state
        } else {  // Hover state
            DrawTextureV(AudioButtonHover, ButtonPosition, WHITE);
            DrawRectangle(GetMouseX() + 25, GetMouseY() + 45, 230, 30, HintBoxColor);
            DrawTextEx(LoadedFont, "Turn on/off audio", {static_cast<float>(GetMouseX()) + 30, static_cast<float>(GetMouseY()) + 50}, 20, 0, YELLOW);
        }
    }
}

// Main game update loop
void Game::Update() {
    while (!WindowShouldClose()) {  // Continue until the window is closed

        BeginDrawing();

        if (ShouldPlayIntro) {
            PlayIntro();
        } else {
            if (ShouldExit) break;  // Exit the game loop if exit button has been preesed

            UpdateObjects();  // Update all objects

            if (worker.CheckIfWorking()) {
                timer.AddToTimer(-GetFrameTime());  // Update the timer

                if (timer.GetTimeLeft() <= 0) {  // Check if the timer has run out
                    timer.SetTimer(0);
                    worker.SetWorkingState(false);
                }

            } else {
                // Check if the current score is higher than the highscore and update if necessary
                if (Score > Highscore)
                    Highscore = Score;

                Score = 0;  // Reset the score
            }

            if (ShouldPlayAudio) {
                UpdateMusicStream(BackgroundMusic);
            }
        }
        EndDrawing();
    }
}

// Save the highscore to a file
void Game::SaveHighscoreToFile() const {
    std::ofstream HighscoreFile("highscore.txt");
    if (HighscoreFile.is_open()) {
        std::string ScoreStr = std::to_string(Highscore);
        std::string EncryptionKey = "HAKUNA_MATATA";
        std::string EncryptedScore = xorEncryptDecrypt(ScoreStr, EncryptionKey);
        HighscoreFile << EncryptedScore << std::endl;
        HighscoreFile.close();
    }
}

int Game::LoadHighscoreFromFile() {
    int LoadedHighscore = 0;
    std::ifstream HighscoreFile("highscore.txt");

    if (HighscoreFile.is_open()) {
        ShouldPlayIntro = false;
        std::string EncryptedScore;
        if (std::getline(HighscoreFile, EncryptedScore)) {
            try {
                std::string EncryptionKey = "HAKUNA_MATATA";
                std::string DecryptedScore = xorEncryptDecrypt(EncryptedScore, EncryptionKey);
                LoadedHighscore = std::stoi(DecryptedScore);
            } catch (const std::exception& e) {
                LoadedHighscore = 0;
            }
        }
        HighscoreFile.close();
    } else {
        LoadedHighscore = 0;
        ShouldPlayIntro = true;
    }

    return LoadedHighscore;
}

void Game::PlayIntro() {
    static int lastFramesCounter = 0; // Keeps track of the last frame counter when a character was displayed
    Rectangle DialogBox = {0, 600, 1600, 360};

    ClearBackground(BLACK);
    FramesCounter++;

    DrawRectangleRec(DialogBox, DARKBLUE); // Inner part
    DrawRectangleLinesEx(DialogBox, 5, LIGHTGRAY); // Outer part
    DrawTextEx(LoadedFont, TextSubtext(messages[CurrentMessage], 0, FramesCounter / 3), {600, DialogBox.y + 40}, 35, 5, WHITE);

    if (FramesCounter / GetRandomValue(20, 30) > lastFramesCounter / GetRandomValue(20, 30) && FramesCounter / 3 < std::string(messages[CurrentMessage]).length()) {
        if (CurrentMessage % 2) {
            PlaySound(talking1);
        } else {
            PlaySound(talking2);
        }
        lastFramesCounter = FramesCounter;
    }

    if (!(CurrentMessage % 2)) {
        DrawTextureV(TextureBoss, {-60, 360}, WHITE);
    } else {
        DrawTextureV(TextureHacker, {0, 360}, WHITE);
    }

    if (FramesCounter / 3 >= std::string(messages[CurrentMessage]).length()) {
        DrawTextEx(LoadedFont, "Press -SPACE- to continue...",
                   {static_cast<float>(GetScreenWidth() - 600), static_cast<float>(GetScreenHeight() - 50)},
                   30, 0, {255, 255, 255, 100});

        if (IsKeyPressed(KEY_SPACE)) {
            FramesCounter = 0;
            lastFramesCounter = 0; // Reset last frame counter when moving to the next message
            if (++CurrentMessage == sizeof(messages) / sizeof(char*)) {
                ShouldPlayIntro = false;
            }
        }
    }
}