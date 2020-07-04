#include "GUI.h"

#include <stdio.h>

#include "raylib.h"

static int screenWidth = 800;
static int screenHeight = 600;

static Texture2D boardTexture;
static Texture2D whiteKingTexture;
static Texture2D whiteQueenTexture;
static Texture2D whiteBishopTexture;
static Texture2D whiteKnightTexture;
static Texture2D whiteRookTexture;
static Texture2D whitePawnTexture;
static Texture2D blackKingTexture;
static Texture2D blackQueenTexture;
static Texture2D blackBishopTexture;
static Texture2D blackKnightTexture;
static Texture2D blackRookTexture;
static Texture2D blackPawnTexture;

void LoadTexturesFromDisk()
{
    Image boardImage = LoadImage("res/Board.png");
    printf("Hey\n"); fflush(stdout);
    boardTexture = LoadTextureFromImage(boardImage);
    UnloadImage(boardImage);

}
void UnloadTexturesFromDisk()
{
    UnloadTexture(boardTexture);
    UnloadTexture(whiteKingTexture);
    UnloadTexture(whiteQueenTexture);
    UnloadTexture(whiteBishopTexture);
    UnloadTexture(whiteKnightTexture);
    UnloadTexture(whiteRookTexture);
    UnloadTexture(whitePawnTexture);
    UnloadTexture(blackKingTexture);
    UnloadTexture(blackQueenTexture);
    UnloadTexture(blackBishopTexture);
    UnloadTexture(blackKnightTexture);
    UnloadTexture(blackRookTexture);
    UnloadTexture(blackPawnTexture);
}

void Setup()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Farcevol chess engine");

    SetTargetFPS(30);
}

void Update()
{

}

void Draw()
{
    Vector2 position = { screenWidth / 2 - boardTexture.width / 2, screenHeight / 2 - boardTexture.height / 2 };
    Color color = { 255, 255, 255, 255 };
    DrawTextureEx(boardTexture, position, 0.0f, 1.0f, color);
}

void Run()
{
    LoadTexturesFromDisk();
    Setup();

    while (!WindowShouldClose())
    {
        Update();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        Draw();
        EndDrawing();
    }

    CloseWindow();
}
