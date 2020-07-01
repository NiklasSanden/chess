#include "utility.h"

#include <stdio.h>

char GetPieceChar(const Piece piece)
{
    switch (piece)
    {
    case WhitePawn:
    case WhiteUnmovedPawn:
    case BlackPawn:
    case BlackUnmovedPawn:
        return 'P';
    case WhiteRook:
    case WhiteUnmovedRook:
    case BlackRook:
    case BlackUnmovedRook:
        return 'R';
    case WhiteKnight:
    case BlackKnight:
        return 'N';
    case WhiteBishop:
    case BlackBishop:
        return 'B';
    case WhiteQueen:
    case BlackQueen:
        return 'Q';
    case WhiteKing:
    case WhiteUnmovedKing:
    case BlackKing:
    case BlackUnmovedKing:
        return 'K';
    default:
        return '.';
    }
}

void PrintBoard(const Board* const board)
{
    for (int y = 7; y >= 0; --y)
    {
        for (int x = 0; x < 8; ++x)
        {
            printf("%c", GetPieceChar(GetPiece(board, x, y)));
        }
        printf("%c", '\n');
    }
    printf("%c", '\n');
}
