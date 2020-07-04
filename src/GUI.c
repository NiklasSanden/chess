#include "GUI.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"

#include "data.h"
#include "rules.h"
#include "ai.h"

static int screenWidth = 800;
static int screenHeight = 600;
static int screenSpaceWidth = 1;
static int screenSpaceHeight = 1;

static Vector2 centreOfScreen = { 0.0f, 0.0f };
static Vector2 bottomLeft = { 0.0f, 0.0f };
static Vector2 topLeft = { 0.0f, 0.0f };
static int boardWidth = 1;
static float squareSize = 1;

static Vector2 backButtonTopLeft = { 0.0f, 0.0f };
static Vector2 forwardButtonTopLeft = {0.0f, 0.0f};
static int backForwardButtonWidth = 1;
static int backForwardButtonHeight = 1;


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

static int currentBoard = 0;
static int totalBoards = 0;
static Board board[500];

static int movingPieceIndex = -1;
static bool hasMouseButtonBeenPressed = false;
static bool hasSpacebarBeenPressed = false;

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

    board[0] = InitializeBoard();
}

void HandleMouseClickedOnBoard(const Vector2 position)
{
    Vector2 offset = Vector2Subtract(position, bottomLeft);
    int x = (int)(offset.x / squareSize);
    int y = -(int)(offset.y / squareSize);

    bool hasMoved = false;
    if (movingPieceIndex != -1)
    {
        Piece movingPiece = GetMovedPieceVersion(board[currentBoard].board[movingPieceIndex]);
        Piece targetPiece = GetPiece(&board[currentBoard], x, y);
        
        if (GetColour(targetPiece) != GetColour(movingPiece))
        {
            Board moves[MAX_LEGAL_MOVES];
            int count;
            GenerateAllLegalMoves(&board[currentBoard], moves, &count);

            for (int i = 0; i < count; ++i)
            {
                if (GetMovedPieceVersion(GetPiece(&moves[i], x, y)) == movingPiece && moves[i].board[movingPieceIndex] == EmptySquare)
                {
                    ++currentBoard;
                    totalBoards = currentBoard + 1;
                    board[currentBoard] = moves[i];
                    hasMoved = true;
                    break;
                }
            }
        }

        movingPieceIndex = -1;
    }

    if (!hasMoved)
    {
        Piece clickedPiece = GetPiece(&board[currentBoard], x, y);
        Colour teamColour = board[currentBoard].isWhiteTurn ? White : Black;
        if (clickedPiece > EmptySquare && GetColour(clickedPiece) == teamColour)
        {
            movingPieceIndex = PositionToIndex(x, y);
        }
    }
}

void CalculateNextMoves()
{
    const static int depth = 4;

    Board moveSequence[depth];
    int moveSequenceCount;

    if (!board[currentBoard].isGameOver)
    {
        printf("Starting to calculate with depth = %i\n", depth);
        CallMinimaxAndGetTheMoveSequence(&board[currentBoard], &moveSequence[0], &moveSequenceCount, depth, board[currentBoard].isWhiteTurn, true);

        if (moveSequence[0].isWhiteTurn == board[currentBoard].isWhiteTurn) // In case it just returned this board. That means it is game over
        {
            board[currentBoard] = moveSequence[0];
            printf("The game is already over\n");
        }
        else
        {
            for (int i = 0; i < moveSequenceCount; ++i)
            {
                board[currentBoard + i + 1] = moveSequence[i];
            }
            totalBoards = currentBoard + 1 + moveSequenceCount;
            board[currentBoard].value = moveSequence[0].value;

            printf("Found a move sequence of %i moves. New board value: %.0f\n", moveSequenceCount, board[currentBoard].value);
        }
    }
    else
    {
        printf("Game is already over!\n");
    }
}

void Update()
{
    if (hasMouseButtonBeenPressed)
    {
        Vector2 position = GetMousePosition();
        
        // If you clicked on the board
        if (position.x >= bottomLeft.x && position.x <= boardWidth + bottomLeft.x && position.y <= bottomLeft.y && position.y >= topLeft.y)
        {
            HandleMouseClickedOnBoard(position);
        }
        // If you clicked on the back button
        else if (position.x >= backButtonTopLeft.x && position.x <= backButtonTopLeft.x + backForwardButtonWidth && position.y >= backButtonTopLeft.y && position.y <= backButtonTopLeft.y + backForwardButtonHeight)
        {
            if (currentBoard > 0) --currentBoard;
        }
        // If you clicked on the forward button
        else if (position.x >= forwardButtonTopLeft.x && position.x <= forwardButtonTopLeft.x + backForwardButtonWidth && position.y >= forwardButtonTopLeft.y && position.y <= forwardButtonTopLeft.y + backForwardButtonHeight)
        {
            if (currentBoard < totalBoards - 1) ++currentBoard;
        }
    }

    if (hasSpacebarBeenPressed)
    {
        CalculateNextMoves();
    }
}

void DrawPiece(const Piece piece, const int x, const int y)
{
    switch (piece)
    {
    case WhitePawn:
    case WhiteUnmovedPawn:
        DrawTexturePro(piecesTexture, whitePawnRec, (Rectangle){ bottomLeft.x + squareSize * x, bottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case BlackPawn:
    case BlackUnmovedPawn:
        DrawTexturePro(piecesTexture, blackPawnRec, (Rectangle){ bottomLeft.x + squareSize * x, bottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case WhiteRook:
    case WhiteUnmovedRook:
        DrawTexturePro(piecesTexture, whiteRookRec, (Rectangle){ bottomLeft.x + squareSize * x, bottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case BlackRook:
    case BlackUnmovedRook:
        DrawTexturePro(piecesTexture, blackRookRec, (Rectangle){ bottomLeft.x + squareSize * x, bottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case WhiteKnight:
        DrawTexturePro(piecesTexture, whiteKnightRec, (Rectangle){ bottomLeft.x + squareSize * x, bottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case BlackKnight:
        DrawTexturePro(piecesTexture, blackKnightRec, (Rectangle){ bottomLeft.x + squareSize * x, bottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case WhiteBishop:
        DrawTexturePro(piecesTexture, whiteBishopRec, (Rectangle){ bottomLeft.x + squareSize * x, bottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case BlackBishop:
        DrawTexturePro(piecesTexture, blackBishopRec, (Rectangle){ bottomLeft.x + squareSize * x, bottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case WhiteQueen:
        DrawTexturePro(piecesTexture, whiteQueenRec, (Rectangle){ bottomLeft.x + squareSize * x, bottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case BlackQueen:
        DrawTexturePro(piecesTexture, blackQueenRec, (Rectangle){ bottomLeft.x + squareSize * x, bottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case WhiteKing:
    case WhiteUnmovedKing:
        DrawTexturePro(piecesTexture, whiteKingRec, (Rectangle){ bottomLeft.x + squareSize * x, bottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    case BlackKing:
    case BlackUnmovedKing:
        DrawTexturePro(piecesTexture, blackKingRec, (Rectangle){ bottomLeft.x + squareSize * x, bottomLeft.y - squareSize * (y + 1), squareSize, squareSize }, Vector2Zero(), 0.0f, WHITE);
        break;
    default: // Emtpy Square
        break;
    }
}

// The board will cover 3/4 of the width but the entire height. However, the board is a square. Therefore our aspect ratio will also be 4:3
const static float aspectRatio = 4.0f / 3.0f;
void Draw()
{
    screenSpaceHeight = screenHeight;
    screenSpaceWidth = screenSpaceHeight * aspectRatio;
    if (screenSpaceWidth > screenWidth)
    {
        screenSpaceWidth = screenWidth;
        screenSpaceHeight = screenSpaceWidth / aspectRatio;
    }
    boardWidth = (screenSpaceWidth * 3) / 4;

    centreOfScreen = (Vector2){ screenWidth / 2, screenHeight / 2 };
    bottomLeft = Vector2Add(centreOfScreen, (Vector2){ -screenSpaceWidth / 2, screenSpaceHeight / 2 });
    topLeft = Vector2Subtract(centreOfScreen, (Vector2){ screenSpaceWidth / 2, screenSpaceHeight / 2 });
    
    // Draw board
    Rectangle boardRectangle = { topLeft.x, topLeft.y, boardWidth, screenSpaceHeight };
    DrawTexturePro(boardTexture, (Rectangle){ 0.0f, 0.0f, boardTexture.width, boardTexture.height }, boardRectangle, Vector2Zero(), 0.0f, WHITE);

    // Draw pieces
    squareSize = boardWidth / 8.0f;
    for (int i = 0; i < 64; ++i)
    {
        int x, y;
        IndexToPosition(i, &x, &y);
        if (i == movingPieceIndex)
        {
            // Draw a blue square background for the selected piece
            DrawRectangle(bottomLeft.x + x * squareSize, bottomLeft.y - squareSize * (y + 1), squareSize + 1, squareSize + 2, BLUE);
        }
        DrawPiece(board[currentBoard].board[i], x, y);
    }

    // Draw UI
    DrawRectangle(topLeft.x + boardWidth, topLeft.y, screenSpaceWidth - boardWidth, screenSpaceHeight, LIGHTGRAY);

    float rightUIColumnWidth = (screenSpaceWidth - boardWidth) / 13.0f;
    backForwardButtonWidth = rightUIColumnWidth * 4;
    backForwardButtonHeight = screenSpaceHeight / 15.0f;

    backButtonTopLeft = (Vector2){ topLeft.x + boardWidth + rightUIColumnWidth * 2, screenHeight / 2.0f - backForwardButtonHeight / 2.0f };
    forwardButtonTopLeft = (Vector2){ topLeft.x + boardWidth + rightUIColumnWidth * (2 + 4 + 1), screenHeight / 2.0f - backForwardButtonHeight / 2.0f };
    
    Vector2 position = GetMousePosition();
    if (position.x >= backButtonTopLeft.x && position.x <= backButtonTopLeft.x + backForwardButtonWidth && position.y >= backButtonTopLeft.y && position.y <= backButtonTopLeft.y + backForwardButtonHeight)
    {
        DrawRectangle(backButtonTopLeft.x, backButtonTopLeft.y, backForwardButtonWidth, backForwardButtonHeight, DARKGRAY);
    }
    else
    {
        DrawRectangle(backButtonTopLeft.x, backButtonTopLeft.y, backForwardButtonWidth, backForwardButtonHeight, GRAY);
    }
    if (position.x >= forwardButtonTopLeft.x && position.x <= forwardButtonTopLeft.x + backForwardButtonWidth && position.y >= forwardButtonTopLeft.y && position.y <= forwardButtonTopLeft.y + backForwardButtonHeight)
    {
        DrawRectangle(forwardButtonTopLeft.x, forwardButtonTopLeft.y, backForwardButtonWidth, backForwardButtonHeight, DARKGRAY);
    }
    else
    {
        DrawRectangle(forwardButtonTopLeft.x, forwardButtonTopLeft.y, backForwardButtonWidth, backForwardButtonHeight, GRAY);
    }
}

void InputsAndEvents()
{
    screenWidth = GetScreenWidth();
    screenHeight  = GetScreenHeight();
    
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        movingPieceIndex = -1;
    }
    hasMouseButtonBeenPressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    hasSpacebarBeenPressed = IsKeyPressed(KEY_SPACE);
}

void Run()
{
    Setup();
    LoadTexturesFromDisk();

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
