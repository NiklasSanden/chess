#include "ai.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "data.h"
#include "rules.h"
#include "utility.h"

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
    /*Board newTest = InitializeEmptyBoard();
    int count;
    Board* a = GenerateAllLegalMoves(&newTest, &count);
    printf("%i\n", count);
    int leaves = 0;
    for (int i = 0; i < count; ++i)
    {
        int count2;
        Board* b = GenerateAllLegalMoves(&a[i], &count2);
        for (int j = 0; j < count2; ++j)
        {
            int count3;
            Board* c = GenerateAllLegalMoves(&b[j], &count3);
            leaves += count3;
        }
    }
    printf("%i\n", leaves);*/

    /*clock_t start, end;
    double cpu_time_used;

    start = clock();

    Board startBoard = InitializeBoard();
    int amount1;
    Board* allMoves1 = GenerateAllLegalMoves(&startBoard, &amount1);
    int nodes = amount1;
    int leaves = 0;
    for (int i = 0; i < amount1; ++i)
    {
        int amount2;
        Board* allMoves2 = GenerateAllLegalMoves(&allMoves1[i], &amount2);
        nodes += amount2;
        for (int j = 0; j < amount2; ++j)
        {
            int amount3;
            Board* allMoves3 = GenerateAllLegalMoves(&allMoves2[j], &amount3);
            nodes += amount3;
            for (int k = 0; k < amount3; ++k)
            {
                int amount4;
                Board* allMoves4 = GenerateAllLegalMoves(&allMoves3[k], &amount4);
                nodes += amount4;
                for (int l = 0; l < amount4; ++l)
                {
                    int amount5;
                    Board* allMoves5 = GenerateAllLegalMoves(&allMoves4[l], &amount5);
                    nodes += amount5;
                    for (int m = 0; m < amount5; ++m)
                    {
                        int amount6;
                        Board* allMoves6 = GenerateAllLegalMoves(&allMoves5[m], &amount6);
                        nodes += amount6;
                        leaves += amount6;
                        free(allMoves6);
                    }
                    free(allMoves5);
                }
                free(allMoves4);
            }
            free(allMoves3);
        }
        free(allMoves2);
    }
    free(allMoves1);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Nodes: %i - Leaves: %i - Time: %f\n", nodes, leaves, cpu_time_used);
    fflush(stdout);*/
}
