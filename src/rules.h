#ifndef CHESS_RULES
#define CHESS_RULES

#include <stdbool.h>
#include <stdlib.h>

#include "data.h"

// Helper
bool CheckForAttackingPieceInLine(const Board* const board, const int x, const int y, const int xIncrement, const int yIncrement, const Piece targetPiece, const Piece queenPiece)
{
    Piece piece;
    int i = 1;
    do
    {
        piece = GetPiece(board, x + xIncrement * i, y + yIncrement * i);
        if (piece == targetPiece || piece == queenPiece)
        {
            return true;
        }

        i++;
    } while (piece != OutsideSquare && GetColour(piece) == NoColour);

    return false;
}

bool IsPositionAttackedByColour(const Board* const board, const int x, const int y, const Colour attackingColour)
{
    int whiteToBlack = 0;
    if (attackingColour == Black)
    {
        whiteToBlack = BlackPawn - WhitePawn;
    }

    // Pawn
    if (GetPiece(board, x - 1, y - 1) == whiteToBlack + WhitePawn) return true;
    if (GetPiece(board, x + 1, y - 1) == whiteToBlack + WhitePawn) return true;

    // King
    if (GetPiece(board, x + 1, y) == whiteToBlack + WhiteKing) return true;
    if (GetPiece(board, x - 1, y) == whiteToBlack + WhiteKing) return true;
    if (GetPiece(board, x, y + 1) == whiteToBlack + WhiteKing) return true;
    if (GetPiece(board, x, y - 1) == whiteToBlack + WhiteKing) return true;
    if (GetPiece(board, x + 1, y + 1) == whiteToBlack + WhiteKing) return true;
    if (GetPiece(board, x + 1, y - 1) == whiteToBlack + WhiteKing) return true;
    if (GetPiece(board, x - 1, y + 1) == whiteToBlack + WhiteKing) return true;
    if (GetPiece(board, x - 1, y - 1) == whiteToBlack + WhiteKing) return true;

    // Knight
    if (GetPiece(board, x + 1, y + 2) == whiteToBlack + WhiteKnight) return true;
    if (GetPiece(board, x - 1, y + 2) == whiteToBlack + WhiteKnight) return true;
    if (GetPiece(board, x + 2, y + 1) == whiteToBlack + WhiteKnight) return true;
    if (GetPiece(board, x - 2, y + 1) == whiteToBlack + WhiteKnight) return true;
    if (GetPiece(board, x + 1, y - 2) == whiteToBlack + WhiteKnight) return true;
    if (GetPiece(board, x - 1, y - 2) == whiteToBlack + WhiteKnight) return true;
    if (GetPiece(board, x + 2, y - 1) == whiteToBlack + WhiteKnight) return true;
    if (GetPiece(board, x - 2, y - 1) == whiteToBlack + WhiteKnight) return true;

    // Rook and first part of Queen
    if (CheckForAttackingPieceInLine(board, x, y, 1, 0, whiteToBlack + WhiteRook, whiteToBlack + WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, -1, 0, whiteToBlack + WhiteRook, whiteToBlack + WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, 0, 1, whiteToBlack + WhiteRook, whiteToBlack + WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, 0, -1, whiteToBlack + WhiteRook, whiteToBlack + WhiteQueen)) return true;

    // Bishop and second part of Queen
    if (CheckForAttackingPieceInLine(board, x, y, 1, 1, whiteToBlack + WhiteBishop, whiteToBlack + WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, -1, 1, whiteToBlack + WhiteBishop, whiteToBlack + WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, 1, -1, whiteToBlack + WhiteBishop, whiteToBlack + WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, -1, -1, whiteToBlack + WhiteBishop, whiteToBlack + WhiteQueen)) return true;

    return false;
}

bool IsColourInCheck(const Board* const board, const Colour teamColour)
{
    int kingXPos, kingYPos;
    if (teamColour == White)
    {
        IndexToPosition(board->whiteKing, &kingXPos, &kingYPos);
        return IsPositionAttackedByColour(board, kingXPos, kingYPos, Black);
    }
    else
    {
        IndexToPosition(board->blackKing, &kingXPos, &kingYPos);
        return IsPositionAttackedByColour(board, kingXPos, kingYPos, White);
    }
}

bool IsSquareBlockedOrOutside(const Board* const board, const int x, const int y, const Colour teamColour)
{
    Piece squarePiece = GetPiece(board, x, y);
    return squarePiece == OutsideSquare || GetColour(squarePiece) == teamColour;
}

// Does not change unmoved enum
void MovePiece(Board* const board, const int xStart, const int yStart, const int xEnd, const int yEnd)
{
    board->isWhiteTurn = !board->isWhiteTurn;

    board->board[xEnd][yEnd] = board->board[xStart][yStart];
    board->board[xStart][yStart] = EmptySquare;
}

void MoveKing(Board* const board, const int xStart, const int yStart, const int xEnd, const int yEnd)
{
    if (board->isWhiteTurn)
    {
        MovePiece(board, xStart, yStart, xEnd, yEnd);
        board->whiteKing = PositionToIndex(xEnd, yEnd);
        if (board->board[xEnd][yEnd] == WhiteUnmovedKing) 
        {
            board->board[xEnd][yEnd] = WhiteKing;
        }
    }
    else
    {
        MovePiece(board, xStart, yStart, xEnd, yEnd);
        board->blackKing = PositionToIndex(xEnd, yEnd);
        if (board->board[xEnd][yEnd] == BlackUnmovedKing) 
        {
            board->board[xEnd][yEnd] = BlackUnmovedKing;
        }
    }
}

// TODO (remember special enums)
void MovePawn(Board* const board, const int xStart, const int yStart, const int xEnd, const int yEnd)
{

}

// TODO (remember special enums)
void MoveRook(Board* const board, const int xStart, const int yStart, const int xEnd, const int yEnd)
{

}

// TODO all other pieces
// Generate Moves
void AddKingMoveIfLegal(const Board* const board, int* const outAmountOfMoves, Board* const newBoard, const int xOld, const int yOld, const int x, const int y, const Colour colour, const Colour oppositeColour)
{
    if (!IsSquareBlockedOrOutside(board, x , y, colour) && !IsPositionAttackedByColour(board, x, y, oppositeColour))
    {
        *newBoard = *board;
        MoveKing(newBoard, xOld, yOld, x, y);
        ++(*outAmountOfMoves);
    }
}
Board* GenerateLegalMovesForKing(const Board* const board, int* const outAmountOfMoves, const int x, const int y)
{
    *outAmountOfMoves = 0;
    Board* moves = malloc(sizeof(*moves) * 10);

    Colour colour = board->isWhiteTurn ? White : Black;
    Colour oppositeColour = colour == White ? Black : White;
    // Normal moves
    AddKingMoveIfLegal(board, outAmountOfMoves, &moves[*outAmountOfMoves], x, y, x + 1, y, colour, oppositeColour);
    AddKingMoveIfLegal(board, outAmountOfMoves, &moves[*outAmountOfMoves], x, y, x - 1, y, colour, oppositeColour);
    AddKingMoveIfLegal(board, outAmountOfMoves, &moves[*outAmountOfMoves], x, y, x, y + 1, colour, oppositeColour);
    AddKingMoveIfLegal(board, outAmountOfMoves, &moves[*outAmountOfMoves], x, y, x, y - 1, colour, oppositeColour);
    AddKingMoveIfLegal(board, outAmountOfMoves, &moves[*outAmountOfMoves], x, y, x + 1, y + 1, colour, oppositeColour);
    AddKingMoveIfLegal(board, outAmountOfMoves, &moves[*outAmountOfMoves], x, y, x + 1, y - 1, colour, oppositeColour);
    AddKingMoveIfLegal(board, outAmountOfMoves, &moves[*outAmountOfMoves], x, y, x - 1, y + 1, colour, oppositeColour);
    AddKingMoveIfLegal(board, outAmountOfMoves, &moves[*outAmountOfMoves], x, y, x - 1, y - 1, colour, oppositeColour);

    // Castling king-side
    if (colour == White)
    {
        if (GetPiece(board, x, y) == WhiteUnmovedKing && GetPiece(board, 7, 0) == WhiteUnmovedRook)
        {
            if (GetPiece(board, x + 1, y) == EmptySquare && GetPiece(board, x + 2, y) == EmptySquare)
            {
                if (!IsPositionAttackedByColour(board, x, y, Black) && !IsPositionAttackedByColour(board, x + 1, y, Black) && !IsPositionAttackedByColour(board, x + 2, y, Black))
                {
                    moves[*outAmountOfMoves] = *board;
                    MoveKing(&moves[*outAmountOfMoves], x, y, x + 2, y);
                    MoveRook(&moves[*outAmountOfMoves], 7, 0, 5, 0);
                    ++(*outAmountOfMoves);
                }
            }
        }
    }
    else
    {
        if (GetPiece(board, x, y) == BlackUnmovedKing && GetPiece(board, 7, 7) == BlackUnmovedRook)
        {
            if (GetPiece(board, x + 1, y) == EmptySquare && GetPiece(board, x + 2, y) == EmptySquare)
            {
                if (!IsPositionAttackedByColour(board, x, y, White) && !IsPositionAttackedByColour(board, x + 1, y, White) && !IsPositionAttackedByColour(board, x + 2, y, White))
                {
                    moves[*outAmountOfMoves] = *board;
                    MoveKing(&moves[*outAmountOfMoves], x, y, x + 2, y);
                    MoveRook(&moves[*outAmountOfMoves], 7, 7, 5, 7);
                    ++(*outAmountOfMoves);
                }
            }
        }
    }
    
    // Castling queen-side
    if (colour == White)
    {
        if (GetPiece(board, x, y) == WhiteUnmovedKing && GetPiece(board, 0, 0) == WhiteUnmovedRook)
        {
            if (GetPiece(board, x - 1, y) == EmptySquare && GetPiece(board, x - 2, y) == EmptySquare && GetPiece(board, x - 3, y) == EmptySquare)
            {
                if (!IsPositionAttackedByColour(board, x, y, Black) && !IsPositionAttackedByColour(board, x - 1, y, Black) && !IsPositionAttackedByColour(board, x - 2, y, Black))
                {
                    moves[*outAmountOfMoves] = *board;
                    MoveKing(&moves[*outAmountOfMoves], x, y, x - 2, y);
                    MoveRook(&moves[*outAmountOfMoves], 0, 0, 3, 0);
                    ++(*outAmountOfMoves);
                }
            }
        }
    }
    else
    {
        if (GetPiece(board, x, y) == BlackUnmovedKing && GetPiece(board, 0, 7) == BlackUnmovedRook)
        {
            if (GetPiece(board, x - 1, y) == EmptySquare && GetPiece(board, x - 2, y) == EmptySquare && GetPiece(board, x - 3, y) == EmptySquare)
            {
                if (!IsPositionAttackedByColour(board, x, y, White) && !IsPositionAttackedByColour(board, x - 1, y, White) && !IsPositionAttackedByColour(board, x - 2, y, White))
                {
                    moves[*outAmountOfMoves] = *board;
                    MoveKing(&moves[*outAmountOfMoves], x, y, x - 2, y);
                    MoveRook(&moves[*outAmountOfMoves], 0, 7, 3, 7);
                    ++(*outAmountOfMoves);
                }
            }
        }
    }

    moves = realloc(moves, sizeof(*moves) * (*outAmountOfMoves));
    return moves;
}

#endif
