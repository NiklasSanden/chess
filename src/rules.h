#ifndef RULES_H
#define RULES_H

#include "data.h"

GameEndStatus GetResultOfGameIfNoMovesAreAvailable(const Board* const board);
Board* GenerateAllLegalMoves(const Board* const board, int* const outAmountOfMoves);

#endif
