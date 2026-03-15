#include "GameExe.h"
#include "Save.h"
#include "Load.h"
#include "raylib.h"
#include <iostream>
#include <string>
#include <atomic>
#include <mutex>
#include <thread>

static bool IsButtonClicked(const Rectangle& button, const Vector2& mousePos) {
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, button);
}

int main() {
    const int screenWidth = 900;
    const int screenHeight = 600;
    const char* saveFile = "saves/menu_save.json";

    InitWindow(screenWidth, screenHeight, "Gomoku Menu");
    SetTargetFPS(60);

    Rectangle startButton = { 350.0f, 190.0f, 200.0f, 60.0f };
    Rectangle saveButton = { 350.0f, 280.0f, 200.0f, 60.0f };
    Rectangle loadButton = { 350.0f, 370.0f, 200.0f, 60.0f };

    GameState menuState;
    std::string statusText = "Ready";
    std::atomic<bool> runTestRunning(false);
    std::mutex menuStateMutex;

    while (!WindowShouldClose()) {
        const Vector2 mousePos = GetMousePosition();

        if (IsButtonClicked(startButton, mousePos)) {
            if (!runTestRunning.load()) {
                std::thread([&runTestRunning, &menuState, &menuStateMutex, saveFile]() {
                    runTestRunning.store(true);
                    GameState initialState;
                    readSharedState(&menuState, &menuStateMutex, initialState);
                    GameState finalState = runTest(initialState, saveFile, &menuState, &menuStateMutex);
                    writeSharedState(&menuState, &menuStateMutex, finalState);
                    runTestRunning.store(false);
                }).detach();
                statusText = "runTest started in terminal";
            }
            else {
                statusText = "runTest is already running";
            }
        }

        if (IsButtonClicked(saveButton, mousePos)) {
            GameState snapshot;
            readSharedState(&menuState, &menuStateMutex, snapshot);

            if (saveGameState(snapshot, saveFile)) {
                statusText = "Save success: saves/menu_save.json";
            }
            else {
                statusText = "Save failed";
            }
        }

        if (IsButtonClicked(loadButton, mousePos)) {
            GameState loaded;
            if (loadGameState(saveFile, loaded)) {
                writeSharedState(&menuState, &menuStateMutex, loaded);
                statusText = "Load success: saves/menu_save.json";
                std::cout << "Loaded board from " << saveFile << "\n";
                printBoard(loaded.board);
            }
            else {
                statusText = "Load failed (file missing or invalid)";
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("GOMOKU MENU", 320, 70, 44, BLACK);
        DrawText("Start: runTest() in terminal", 320, 130, 20, DARKGRAY);

        const bool overStart = CheckCollisionPointRec(mousePos, startButton);
        const bool overSave = CheckCollisionPointRec(mousePos, saveButton);
        const bool overLoad = CheckCollisionPointRec(mousePos, loadButton);

        DrawRectangleRec(startButton, overStart ? SKYBLUE : BLUE);
        DrawRectangleRec(saveButton, overSave ? LIME : GREEN);
        DrawRectangleRec(loadButton, overLoad ? ORANGE : GOLD);

        DrawText("START", 418, 208, 28, WHITE);
        DrawText("SAVE", 430, 298, 28, WHITE);
        DrawText("LOAD", 430, 388, 28, WHITE);

        DrawText("Status:", 70, 500, 24, DARKGRAY);
        DrawText(statusText.c_str(), 150, 503, 20, MAROON);

        DrawText("Tip: Save/Load uses saves/menu_save.json", 70, 540, 18, GRAY);

        EndDrawing();
    }

    CloseWindow();


    return 0;
}