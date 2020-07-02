#include "ai.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <float.h>

#include "data.h"
#include "rules.h"
#include "utility.h"

// Position Dictionaries
static const float PawnPositionValues[] = {
     0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
     5,  5, 10, 25, 25, 10,  5,  5,
     0,  0,  0, 20, 20,  0,  0,  0,
     5, -5,-10,  0,  0,-10, -5,  5,
     5, 10, 10,-20,-20, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0
};
static const float KnightPositionValues[] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50,
};
static const float BishopPositionValues[] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20,
};
static const float RookPositionValues[] = {
      0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10, 10, 10, 10, 10,  5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
      0,  0,  0,  5,  5,  0,  0,  0
};
static const float QueenPositionValues[] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
     -5,  0,  5,  5,  5,  5,  0, -5,
      0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};
static const float KingPositionValues[] = {
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     20, 20,  0,  0,  0,  0, 20, 20,
     20, 30, 10,  0,  0, 10, 30, 20
};


float GetBoardValue(const Board* const board)
{
    float value = 0.0f;
    for (int i = 0; i < 64; ++i)
    {
        int x, y;
        IndexToPosition(i, &x, &y);
        y = 7 - y;
        switch (board->board[i])
        {
        // White
        case WhitePawn:
        case WhiteUnmovedPawn:
            value += 100.0f;
            value += PawnPositionValues[PositionToIndex(x, y)];
            break;
        case WhiteRook:
        case WhiteUnmovedRook:
            value += 500.0f;
            value += RookPositionValues[PositionToIndex(x, y)];
            break;
        case WhiteKnight:
            value += 320.0f;
            value += KnightPositionValues[PositionToIndex(x, y)];
            break;
        case WhiteBishop:
            value += 330.0f;
            value += BishopPositionValues[PositionToIndex(x, y)];
            break;
        case WhiteQueen:
            value += 900.0f;
            value += QueenPositionValues[PositionToIndex(x, y)];
            break;
        case WhiteKing:
        case WhiteUnmovedKing:
            value += KingPositionValues[i];
            break;
        // Black
        case BlackPawn:
        case BlackUnmovedPawn:
            value -= 100.0f;
            value -= PawnPositionValues[i];
            break;
        case BlackRook:
        case BlackUnmovedRook:
            value -= 500.0f;
            value -= RookPositionValues[i];
            break;
        case BlackKnight:
            value -= 320.0f;
            value -= KnightPositionValues[i];
            break;
        case BlackBishop:
            value -= 330.0f;
            value -= BishopPositionValues[i];
            break;
        case BlackQueen:
            value -= 900.0f;
            value -= QueenPositionValues[i];
            break;
        case BlackKing:
        case BlackUnmovedKing:
            value -= KingPositionValues[i];
            break;
        default: // Empty Square
            break;
        }
    }

    return value;
}

Board Minimax(const Board* const board, const int maxDepth, const int depth, const bool isMaximizing)
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

    // TODO Order child nodes

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
    printf("Nodes: %lli - Leaves: %lli - Time: %f\n", nodes, leaves, cpu_time_used);
    fflush(stdout);*/
}
