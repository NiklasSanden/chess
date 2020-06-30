#ifndef CHESS_RULES
#define CHESS_RULES

#include <stdbool.h>

#include "data.h"

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
    } while (piece != OutsidePiece && GetColour(piece) == NoColour);

    return false;
}

bool IsPositionAttackedByWhite(const Board* const board, const int x, const int y)
{
    // Pawn
    if (GetPiece(board, x - 1, y - 1) == WhitePawn) return true;
    if (GetPiece(board, x + 1, y - 1) == WhitePawn) return true;

    // King
    if (GetPiece(board, x + 1, y) == WhiteKing) return true;
    if (GetPiece(board, x - 1, y) == WhiteKing) return true;
    if (GetPiece(board, x, y + 1) == WhiteKing) return true;
    if (GetPiece(board, x, y - 1) == WhiteKing) return true;
    if (GetPiece(board, x + 1, y + 1) == WhiteKing) return true;
    if (GetPiece(board, x + 1, y - 1) == WhiteKing) return true;
    if (GetPiece(board, x - 1, y + 1) == WhiteKing) return true;
    if (GetPiece(board, x - 1, y - 1) == WhiteKing) return true;

    // Knight
    if (GetPiece(board, x + 1, y + 2) == WhiteKnight) return true;
    if (GetPiece(board, x - 1, y + 2) == WhiteKnight) return true;
    if (GetPiece(board, x + 2, y + 1) == WhiteKnight) return true;
    if (GetPiece(board, x - 2, y + 1) == WhiteKnight) return true;
    if (GetPiece(board, x + 1, y - 2) == WhiteKnight) return true;
    if (GetPiece(board, x - 1, y - 2) == WhiteKnight) return true;
    if (GetPiece(board, x + 2, y - 1) == WhiteKnight) return true;
    if (GetPiece(board, x - 2, y - 1) == WhiteKnight) return true;

    // Rook and first part of Queen
    if (CheckForAttackingPieceInLine(board, x, y, 1, 0, WhiteRook, WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, -1, 0, WhiteRook, WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, 0, 1, WhiteRook, WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, 0, -1, WhiteRook, WhiteQueen)) return true;

    // Bishop and second part of Queen
    if (CheckForAttackingPieceInLine(board, x, y, 1, 1, WhiteBishop, WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, -1, 1, WhiteBishop, WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, 1, -1, WhiteBishop, WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, -1, -1, WhiteBishop, WhiteQueen)) return true;

    return false;
}
bool IsPositionAttackedByBlack(const Board* const board, const int x, const int y)
{
    // Pawn
    if (GetPiece(board, x - 1, y - 1) == BlackPawn) return true;
    if (GetPiece(board, x + 1, y - 1) == BlackPawn) return true;

    // King
    if (GetPiece(board, x + 1, y) == BlackKing) return true;
    if (GetPiece(board, x - 1, y) == BlackKing) return true;
    if (GetPiece(board, x, y + 1) == BlackKing) return true;
    if (GetPiece(board, x, y - 1) == BlackKing) return true;
    if (GetPiece(board, x + 1, y + 1) == BlackKing) return true;
    if (GetPiece(board, x + 1, y - 1) == BlackKing) return true;
    if (GetPiece(board, x - 1, y + 1) == BlackKing) return true;
    if (GetPiece(board, x - 1, y - 1) == BlackKing) return true;

    // Knight
    if (GetPiece(board, x + 1, y + 2) == BlackKnight) return true;
    if (GetPiece(board, x - 1, y + 2) == BlackKnight) return true;
    if (GetPiece(board, x + 2, y + 1) == BlackKnight) return true;
    if (GetPiece(board, x - 2, y + 1) == BlackKnight) return true;
    if (GetPiece(board, x + 1, y - 2) == BlackKnight) return true;
    if (GetPiece(board, x - 1, y - 2) == BlackKnight) return true;
    if (GetPiece(board, x + 2, y - 1) == BlackKnight) return true;
    if (GetPiece(board, x - 2, y - 1) == BlackKnight) return true;

    // Rook and first part of Queen
    if (CheckForAttackingPieceInLine(board, x, y, 1, 0, BlackRook, BlackQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, -1, 0, BlackRook, BlackQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, 0, 1, BlackRook, BlackQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, 0, -1, BlackRook, BlackQueen)) return true;

    // Bishop and second part of Queen
    if (CheckForAttackingPieceInLine(board, x, y, 1, 1, BlackBishop, BlackQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, -1, 1, BlackBishop, BlackQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, 1, -1, BlackBishop, BlackQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, -1, -1, BlackBishop, BlackQueen)) return true;

    return false;
}

bool IsWhiteInCheck(const Board* const board)
{
    int kingXPos, kingYPos;
    IndexToPosition(board->whiteKing, &kingXPos, &kingYPos);
    return IsPositionAttackedByBlack(board, kingXPos, kingYPos);
}
bool IsBlackInCheck(const Board* const board)
{
    int kingXPos, kingYPos;
    IndexToPosition(board->blackKing, &kingXPos, &kingYPos);
    return IsPositionAttackedByWhite(board, kingXPos, kingYPos);
}

bool IsSquareBlockedOrOutside(const Board* const board, const int x, const int y, const Colour teamColour)
{
    Piece squarePiece = GetPiece(board, x, y);
    return squarePiece == OutsidePiece || GetColour(squarePiece) == teamColour;
}

#endif
