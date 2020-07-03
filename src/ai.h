#ifndef AI_H
#define AI_H

#include "data.h"

void CallMinimaxAndGetTheMoveSequence(const Board* const board, Board* const outMoveSequence, int* const outMoveSequenceCount, const int depth, const bool isMaximizing);
void TestGenerateLeavesAtDepthFromTestBoard();
void TestPlayingItself();

#endif
