#include "ai.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <float.h>

#include "data.h"
#include "rules.h"
#include "utility.h"

// TODO: Add position based points (dictionaries)
float GetBoardValue(const Board* const board)
{
    float value = 0.0f;
    for (int i = 0; i < 64; ++i)
    {
        switch (board->board[i])
        {
        // White
        case WhitePawn:
        case WhiteUnmovedPawn:
            value += 10.0f;
            break;
        case WhiteRook:
        case WhiteUnmovedRook:
            value += 50.0f;
            break;
        case WhiteKnight:
            value += 30.0f;
            break;
        case WhiteBishop:
            value += 30.0f;
            break;
        case WhiteQueen:
            value += 90.0f;
            break;
        // Black
        case BlackPawn:
        case BlackUnmovedPawn:
            value -= 10.0f;
            break;
        case BlackRook:
        case BlackUnmovedRook:
            value -= 50.0f;
            break;
        case BlackKnight:
            value -= 30.0f;
            break;
        case BlackBishop:
            value -= 30.0f;
            break;
        case BlackQueen:
            value -= 90.0f;
            break;
        default: // King and empty square
            break;
        }
    }

    return value;
}

Board Minimax(Board* const board, const int maxDepth, const int depth, const bool isMaximizing)
{
    Board returnBoard;
    // Static evaluation for leaves
    if (depth == maxDepth)
    {
        returnBoard = *board;
        returnBoard.value = GetBoardValue(&returnBoard);
        return returnBoard;
    }


    int allPossibleMovesCount;
    Board* allPossibleMoves = GenerateAllLegalMoves(board, &allPossibleMovesCount);

    // If the game is over (no need to free allPossibleMoves since it points to NULL if allPossibleMovesCount = 0)
    if (allPossibleMovesCount == 0)
    {
        returnBoard = *board;
        if (GetResultOfGameIfNoMovesAreAvailable(board) == Stalemate)
        {
            returnBoard.value = 0.0f;
        }
        else
        {
            returnBoard.value = isMaximizing ? FLT_MIN : FLT_MAX;
        }
        return returnBoard;
    }

    // Value child nodes
    Board followingMinimaxBoards[allPossibleMovesCount];
    for (int i = 0; i < allPossibleMovesCount; ++i)
    {
        followingMinimaxBoards[i] = Minimax(&allPossibleMoves[i], maxDepth, depth + 1, !isMaximizing);
    }

    // Order them and pick the best one
    int bestIndex = 0;
    float bestValue = isMaximizing ? -FLT_MAX : FLT_MAX;
    for (int i = 0; i < allPossibleMovesCount; ++i)
    {
        if ((isMaximizing && followingMinimaxBoards[i].value > bestValue) || (!isMaximizing && followingMinimaxBoards[i].value < bestValue))
        {
            bestValue = followingMinimaxBoards[i].value;
            bestIndex = i;
        }
    }

    returnBoard = allPossibleMoves[bestIndex];
    returnBoard.value = bestValue;
    free(allPossibleMoves);
    return returnBoard;
}

void Test()
{
    Board board = InitializeBoard();
    PrintBoard(&board);
    while (board.value != -FLT_MAX && board.value != FLT_MAX)
    {
        board = Minimax(&board, 4, 0, board.isWhiteTurn);
        PrintBoard(&board);
        fflush(stdout);
    }
    printf("Board value: %f - Result: ", GetBoardValue(&board));
    if (GetResultOfGameIfNoMovesAreAvailable(&board) == Stalemate)
    {
        printf("Stalemate!\n");
    }
    else
    {
        if (board.isWhiteTurn) printf("Black won!\n");
        else                   printf("White won!\n");
    }

    // Test board
    /*Board newTest = InitializeTestBoard();
    int count;
    Board* a = GenerateAllLegalMoves(&newTest, &count);
    int leaves = 0;
    for (int i = 0; i < count; ++i)
    {
        int count2;
        Board* b = GenerateAllLegalMoves(&a[i], &count2);
        for (int j = 0; j < count2; ++j)
        {
            int count3;
            Board* c = GenerateAllLegalMoves(&b[j], &count3);
            for (int k = 0; k < count3; ++k)
            {
                int count4;
                Board* d = GenerateAllLegalMoves(&c[k], &count4);
                for (int l = 0; l < count4; ++l)
                {
                    int count5;
                    Board* e = GenerateAllLegalMoves(&d[l], &count5);
                    leaves += count5;
                }
            }
        }
    }
    printf("%i\n", leaves);*/

    // First 6 moves
    /*clock_t start, end;
    double cpu_time_used;

    start = clock();

    Board startBoard = InitializeBoard();
    int amount1;
    Board* allMoves1 = GenerateAllLegalMoves(&startBoard, &amount1);
    long long nodes = amount1;
    long long leaves = 0;
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
                        for (int n = 0; n < amount6; ++n)
                        {
                            int amount7;
                            Board* allMoves7 = GenerateAllLegalMoves(&allMoves6[n], &amount7);
                            nodes += amount7;
                            leaves += amount7;
                            free(allMoves7);
                        }
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
    printf("Nodes: %lli - Leaves: %lli - Time: %f\n", nodes, leaves, cpu_time_used);
    fflush(stdout);*/
}
