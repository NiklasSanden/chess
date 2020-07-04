#include "GUI.h"

#include <stdio.h>
#include <assert.h>

#include "raylib.h"
#include "raymath.h"

#include "data.h"

static int screenWidth = 800;
static int screenHeight = 600;

static Texture2D boardTexture;
static Texture2D piecesTexture;
static Rectangle whiteKingRec;
static Rectangle whiteQueenRec;
static Rectangle whiteBishopRec;
static Rectangle whiteKnightRec;
static Rectangle whiteRookRec;
static Rectangle whitePawnRec;
static Rectangle blackKingRec;
static Rectangle blackQueenRec;
static Rectangle blackBishopRec;
static Rectangle blackKnightRec;
static Rectangle blackRookRec;
static Rectangle blackPawnRec;

void LoadTexturesFromDisk()
{
    boardTexture = LoadTexture("res/Board.png");
    piecesTexture = LoadTexture("res/Pieces.png");

    whiteKingRec =      (Rectangle){ 0.0f                       , 0.0f, piecesTexture.width / 6, piecesTexture.height / 2 };
    whiteQueenRec =     (Rectangle){ piecesTexture.width / 6    , 0.0f, piecesTexture.width / 6, piecesTexture.height / 2 };
    whiteBishopRec =    (Rectangle){ piecesTexture.width / 6 * 2, 0.0f, piecesTexture.width / 6, piecesTexture.height / 2 };
    whiteKnightRec =    (Rectangle){ piecesTexture.width / 6 * 3, 0.0f, piecesTexture.width / 6, piecesTexture.height / 2 };
    whiteRookRec =      (Rectangle){ piecesTexture.width / 6 * 4, 0.0f, piecesTexture.width / 6, piecesTexture.height / 2 };
    whitePawnRec =      (Rectangle){ piecesTexture.width / 6 * 5, 0.0f, piecesTexture.width / 6, piecesTexture.height / 2 };

    blackKingRec =      (Rectangle){ 0.0f                       , piecesTexture.height / 2, piecesTexture.width / 6, piecesTexture.height / 2 };
    blackQueenRec =     (Rectangle){ piecesTexture.width / 6    , piecesTexture.height / 2, piecesTexture.width / 6, piecesTexture.height / 2 };
    blackBishopRec =    (Rectangle){ piecesTexture.width / 6 * 2, piecesTexture.height / 2, piecesTexture.width / 6, piecesTexture.height / 2 };
    blackKnightRec =    (Rectangle){ piecesTexture.width / 6 * 3, piecesTexture.height / 2, piecesTexture.width / 6, piecesTexture.height / 2 };
    blackRookRec =      (Rectangle){ piecesTexture.width / 6 * 4, piecesTexture.height / 2, piecesTexture.width / 6, piecesTexture.height / 2 };
    blackPawnRec =      (Rectangle){ piecesTexture.width / 6 * 5, piecesTexture.height / 2, piecesTexture.width / 6, piecesTexture.height / 2 };
}
void UnloadTexturesFromDisk()
{
    UnloadTexture(boardTexture);
    UnloadTexture(piecesTexture);
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

void DrawPiece(const Piece piece, const int x, const int y, const Vector2 boardBottomLeft, const float squareSize)
{
    switch (piece)
    {
    case WhitePawn:
    case WhiteUnmovedPawn:
        DrawTexturePro(piecesTexture, whitePawnRec, (Rectangle){ boardBottomLeft.x + squareSize * x, boardBottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case BlackPawn:
    case BlackUnmovedPawn:
        DrawTexturePro(piecesTexture, blackPawnRec, (Rectangle){ boardBottomLeft.x + squareSize * x, boardBottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case WhiteRook:
    case WhiteUnmovedRook:
        DrawTexturePro(piecesTexture, whiteRookRec, (Rectangle){ boardBottomLeft.x + squareSize * x, boardBottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case BlackRook:
    case BlackUnmovedRook:
        DrawTexturePro(piecesTexture, blackRookRec, (Rectangle){ boardBottomLeft.x + squareSize * x, boardBottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case WhiteKnight:
        DrawTexturePro(piecesTexture, whiteKnightRec, (Rectangle){ boardBottomLeft.x + squareSize * x, boardBottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case BlackKnight:
        DrawTexturePro(piecesTexture, blackKnightRec, (Rectangle){ boardBottomLeft.x + squareSize * x, boardBottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case WhiteBishop:
        DrawTexturePro(piecesTexture, whiteBishopRec, (Rectangle){ boardBottomLeft.x + squareSize * x, boardBottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case BlackBishop:
        DrawTexturePro(piecesTexture, blackBishopRec, (Rectangle){ boardBottomLeft.x + squareSize * x, boardBottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case WhiteQueen:
        DrawTexturePro(piecesTexture, whiteQueenRec, (Rectangle){ boardBottomLeft.x + squareSize * x, boardBottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case BlackQueen:
        DrawTexturePro(piecesTexture, blackQueenRec, (Rectangle){ boardBottomLeft.x + squareSize * x, boardBottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case WhiteKing:
    case WhiteUnmovedKing:
        DrawTexturePro(piecesTexture, whiteKingRec, (Rectangle){ boardBottomLeft.x + squareSize * x, boardBottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case BlackKing:
    case BlackUnmovedKing:
        DrawTexturePro(piecesTexture, blackKingRec, (Rectangle){ boardBottomLeft.x + squareSize * x, boardBottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    default: // Emtpy Square
        break;
    }
}

// The board will cover 3/4 of the width but the entire height. However, the board is a square. Therefore our aspect ratio will also be 4:3
const static float aspectRatio = 4.0f / 3.0f;
static Board board;
void Draw()
{
    int screenSpaceHeight = screenHeight;
    int screenSpaceWidth = screenSpaceHeight * aspectRatio;
    if (screenSpaceWidth > screenWidth)
    {
        screenSpaceWidth = screenWidth;
        screenSpaceHeight = screenSpaceWidth / aspectRatio;
    }
    int boardWidth = (screenSpaceWidth * 3) / 4;

    Vector2 centreOfScreen = { screenWidth / 2, screenHeight / 2 };
    Vector2 bottomLeft = Vector2Add(centreOfScreen, (Vector2){ -screenSpaceWidth / 2, screenSpaceHeight / 2 });
    Vector2 topLeft = Vector2Subtract(centreOfScreen, (Vector2){ screenSpaceWidth / 2, screenSpaceHeight / 2 });
    
    Rectangle boardRectangle = { topLeft.x, topLeft.y, boardWidth, screenSpaceHeight };
    DrawTexturePro(boardTexture, (Rectangle){ 0.0f, 0.0f, boardTexture.width, boardTexture.height }, boardRectangle, Vector2Zero(), 0.0f, WHITE);

    float squareSize = boardWidth / 8;
    for (int i = 0; i < 64; ++i)
    {
        int x, y;
        IndexToPosition(i, &x, &y);
        DrawPiece(board.board[i], x, y, bottomLeft, squareSize);
    }
}

void InputsAndEvents()
{
    screenWidth = GetScreenWidth();
    screenHeight  = GetScreenHeight();
}

void Run()
{
    Setup();
    LoadTexturesFromDisk();

    board = InitializeBoard();

    while (!WindowShouldClose())
    {
        InputsAndEvents();
        Update();

        BeginDrawing();
        ClearBackground(DARKGRAY);
        Draw();
        EndDrawing();
    }

    CloseWindow();
}
