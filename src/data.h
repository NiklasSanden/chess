#ifndef DATA_H
#define DATA_H

#include <stdbool.h>

// NoColour = 0
typedef enum EColour
{
    NoColour,
    White,
    Black,
} Colour;

// OutsideSquare = 0
typedef enum EPiece
{
    OutsideSquare,
    EmptySquare,
    WhitePawn, WhiteUnmovedPawn,
    WhiteRook, WhiteUnmovedRook,
    WhiteKnight,
    WhiteBishop,
    WhiteQueen,
    WhiteKing, WhiteUnmovedKing,
    BlackPawn, BlackUnmovedPawn,
    BlackRook, BlackUnmovedRook,
    BlackKnight,
    BlackBishop,
    BlackQueen,
    BlackKing, BlackUnmovedKing,
} Piece;

// Stalemate = 0
typedef enum EGameEndStatus
{
    Stalemate,
    Checkmate
} GameEndStatus;

typedef struct
{
    char board[64];
    void* nextBoard;
    float value;
    int whiteKingIndex; // Init 4
    int blackKingIndex; // Init 60
    int enPassantIndex; // Init -1. There can only be one pawn that can be attacked with en passant at a time. Needs to be reset to -1 when there isn't one
    bool isWhiteTurn;   // Init true
    bool isGameOver;    // Init false
} Board;
Board InitializeBoard();
Board InitializeTestBoard();
void FreeRecursiveNextBoard(Board* const board);

int PositionToIndex(const int x, const int y);
void IndexToPosition(const int index, int* const outX, int* const outY);
Piece GetPiece(const Board* const board, const int x, const int y);
void SetPiece(Board* const board, const int x, const int y, const Piece piece);
Colour GetColour(const Piece piece);

#endif
