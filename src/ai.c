#include "ai.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "data.h"
#include "rules.h"

int GetMaterialValue(const Board* const board)
{
    int value = 0;
    for (int i = 0; i < 64; ++i)
    {
        switch (board->board[i])
        {
        // White
        case WhitePawn:
        case WhiteUnmovedPawn:
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
        default: // King and empty square
            break;
        }
    }

    return value;
}

void Test()
{
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    Board b = InitializeBoard();
    int count = 0;
    while (count < 1000000)
    {
        int amount;
        Board* arr = GenerateAllLegalMoves(&b, &amount);
        free(arr);
        ++count;
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%f\n", cpu_time_used);
    fflush(stdout);
}
