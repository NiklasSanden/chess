#include "ai.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <float.h>

#include "data.h"
#include "rules.h"
#include "utility.h"

static int nodes = 1;
static int round = 1;

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
static const float KingPositionValues[] = { // Added castling incentive
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     20, 20,  0,  0,  0,  0, 20, 20,
     30, 40, 20,  0,  0, 20, 40, 30
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
            value += KingPositionValues[PositionToIndex(x, y)];
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

// TODO Implement
float GetDeepBoardValue(const Board* const board)
{
    return GetBoardValue(board);
}

int CompareAscendingOrder(const void* a, const void* b)
{
    float valueA = (*((Board*)a)).value;
    float valueB = (*((Board*)b)).value;

    return (valueA > valueB) - (valueA < valueB);
}
int CompareDescendingOrder(const void* a, const void* b)
{
    float valueA = (*((Board*)a)).value;
    float valueB = (*((Board*)b)).value;

    return (valueB > valueA) - (valueB < valueA);
}

// Sets the nextBoard pointer on the board parameter
void Minimax(Board* const board, float alpha, float beta, const int depthLeft, const bool isMaximizing, const bool deepLeafEvaluation)
{
    board->nextBoard = NULL;
    // Static evaluation has already been done when sorting
    if (depthLeft == 0)
    {
        if (deepLeafEvaluation)
        {
            board->value = GetDeepBoardValue(board);
        }
        return;
    }


    int allPossibleMovesCount;
    Board allPossibleMoves[MAX_LEGAL_MOVES];
    GenerateAllLegalMoves(board, allPossibleMoves, &allPossibleMovesCount);
    nodes += allPossibleMovesCount;

    // The game is over if you can't make a move (Checkmate or Stalemate)
    if (allPossibleMovesCount == 0)
    {
        board->isGameOver = true;
        if (GetResultOfGameIfNoMovesAreAvailable(board) == Stalemate)
        {
            board->value = 0.0f;
        }
        else
        {
            // They have to be smaller than FLT_MAX since that is reserved for alpha and beta
            board->value = isMaximizing ? -FLT_MAX / 10.0f : FLT_MAX / 10.0f;
        }

        return;
    }

    // Set their values
    for (int i = 0; i < allPossibleMovesCount; ++i)
    {
        allPossibleMoves[i].value = GetBoardValue(&allPossibleMoves[i]);
    }

    // The ordering changes depending on if you want to maximize or minimize
    if (isMaximizing)
    {
        qsort(allPossibleMoves, allPossibleMovesCount, sizeof(*allPossibleMoves), CompareDescendingOrder);
    }
    else
    {
        qsort(allPossibleMoves, allPossibleMovesCount, sizeof(*allPossibleMoves), CompareAscendingOrder);
    }

    // Value child nodes and pick the best one
    int bestIndex = 0;
    float nextBoardValue = isMaximizing ? alpha : beta;
    int lastMinimaxIndex = 0; // Since pruning can get us out of calling Minimax
    for (int i = 0; i < allPossibleMovesCount; ++i)
    {
        Minimax(&allPossibleMoves[i], alpha, beta, depthLeft - 1, !isMaximizing, deepLeafEvaluation);
        lastMinimaxIndex = i;

        // Alpha beta pruning
        if (isMaximizing)
        {
            if (allPossibleMoves[i].value >= beta)
            {
                nextBoardValue = beta;
                bestIndex = 0;
                break;
            }
            if (allPossibleMoves[i].value > alpha)
            {
                alpha = allPossibleMoves[i].value;
                nextBoardValue = alpha;
                bestIndex = i;
            }
        }
        else
        {
            if (allPossibleMoves[i].value <= alpha)
            {
                nextBoardValue = alpha;
                bestIndex = 0;
                break;
            }
            if (allPossibleMoves[i].value < beta)
            {
                beta = allPossibleMoves[i].value;
                nextBoardValue = beta;
                bestIndex = i;
            }
        }
    }

    // Cleanup
    for (int i = 0; i <= lastMinimaxIndex; ++i)
    {
        if (i == bestIndex) continue;
        FreeRecursiveNextBoard(&allPossibleMoves[i]);
    }

    // Save next board
    Board* nextBoard = malloc(sizeof(*nextBoard));
    *nextBoard = allPossibleMoves[bestIndex];
    nextBoard->value = nextBoardValue;

    board->value = nextBoardValue;
    board->nextBoard = nextBoard;
}

void GetMoveSequenceFromNextBoardPointerChain(const Board* const board, Board* const outMoveSequence, int* const outMoveSequenceCount, const int index)
{
    if (board->nextBoard)
    {
        GetMoveSequenceFromNextBoardPointerChain(board->nextBoard, outMoveSequence, outMoveSequenceCount, index + 1);
    }
    else
    {
        *outMoveSequenceCount = index + 1;
    }
    outMoveSequence[index] = *board;
    outMoveSequence[index].nextBoard = NULL; // The nextBoard will be freed anyway after this copies them to the array outMoveSequence
}

// Automatically cleans
void CallMinimaxAndGetTheMoveSequence(const Board* const board, Board* const outMoveSequence, int* const outMoveSequenceCount, const int depth, const bool isMaximizing, const bool deepLeafEvaluation)
{
    Board tempBoard = *board;
    Minimax(&tempBoard, -FLT_MAX, FLT_MAX, depth, isMaximizing, deepLeafEvaluation);

    if (tempBoard.isGameOver)
    {
        outMoveSequence[0] = tempBoard;
        *outMoveSequenceCount = 1;
    }
    else
    {
        GetMoveSequenceFromNextBoardPointerChain(tempBoard.nextBoard, outMoveSequence, outMoveSequenceCount, 0);
        FreeRecursiveNextBoard(&tempBoard);
    }
}

// Test functions
void TestGenerateLeavesAtDepthFromTestBoard()
{
    Board newTest = InitializeTestBoard();
    int count;
    Board a[MAX_LEGAL_MOVES];
    GenerateAllLegalMoves(&newTest, a, &count);
    int leaves = 0;
    for (int i = 0; i < count; ++i)
    {
        int count2;
        Board b[MAX_LEGAL_MOVES];
        GenerateAllLegalMoves(&a[i], b, &count2);
        for (int j = 0; j < count2; ++j)
        {
            int count3;
            Board c[MAX_LEGAL_MOVES];
            GenerateAllLegalMoves(&b[j], c, &count3);
            for (int k = 0; k < count3; ++k)
            {
                int count4;
                Board d[MAX_LEGAL_MOVES];
                GenerateAllLegalMoves(&c[k], d, &count4);
                for (int l = 0; l < count4; ++l)
                {
                    int count5;
                    Board e[MAX_LEGAL_MOVES];
                    GenerateAllLegalMoves(&d[l], e, &count5);
                    leaves += count5;
                }
            }
        }
    }
    printf("%i\n", leaves);
}

void TestPlayingItself()
{
    const int depth = 6;
    printf("TestPlayingItself with depth: %i\n\n", depth);

    Board board = InitializeBoard();
    Board moveSequence[depth];
    int moveSequenceCount;
    PrintBoard(&board);

    clock_t start, end;
    clock_t moveStart, moveEnd;
    double cpuTimeUsed;
    double moveCpuTimeUsed;
    start = clock();

    while (!board.isGameOver)
    {
        moveStart = clock();
        
        CallMinimaxAndGetTheMoveSequence(&board, &moveSequence[0], &moveSequenceCount, depth, board.isWhiteTurn, true);
        board = moveSequence[0];

        moveEnd = clock();
        moveCpuTimeUsed = ((double)(moveEnd - moveStart)) / CLOCKS_PER_SEC;

        printf("Round: %i - Nodes: %i - Value: %.1f - Static value: %.1f - Time: %.3f\n", round, nodes, board.value, GetBoardValue(&board), moveCpuTimeUsed);
        PrintBoard(&board);
        nodes = 1;
        ++round;
    }

    // Game over
    end = clock();
    cpuTimeUsed = ((double) (end - start)) / CLOCKS_PER_SEC;

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
    printf("TIME: %f\n", cpuTimeUsed);
}
