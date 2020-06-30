#ifndef CHESS_AI
#define CHESS_AI

#include <stdbool.h>

#include "data.h"
#include "rules.h"

int GetMaterialValue(const Board* const board)
{
    int value = 0;
    for (int x = 0; x < 8; ++x)
    {
        for (int y = 0; y < 8; ++y)
        {
            switch (board->board[x][y])
            {
            // White
            case WhitePawn:
            case WhiteUnmovedPawn:
            case WhiteEnPassantPawn:
                value += 1;
                break;
            case WhiteRook:
            case WhiteUnmovedRook:
                value += 5;
                break;
            case WhiteKnight:
                value += 3;
                break;
            case WhiteBishop:
                value += 3;
                break;
            case WhiteQueen:
                value += 9;
                break;
            // Black
            case BlackPawn:
            case BlackUnmovedPawn:
            case BlackEnPassantPawn:
                value -= 1;
                break;
            case BlackRook:
            case BlackUnmovedRook:
                value -= 5;
                break;
            case BlackKnight:
                value -= 3;
                break;
            case BlackBishop:
                value -= 3;
                break;
            case BlackQueen:
                value -= 9;
                break;
            default:
                break;
            }
        }
    }

    return value;
}

#endif
