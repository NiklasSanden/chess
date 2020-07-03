#ifndef RULES_H
#define RULES_H

#include "data.h"

#define MAX_LEGAL_MOVES 200

GameEndStatus GetResultOfGameIfNoMovesAreAvailable(const Board* const board);
void GenerateAllLegalMoves(const Board* const board, Board* const newMoves, int* const outAmountOfMoves);

#endif
