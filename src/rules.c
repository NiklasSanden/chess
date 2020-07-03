#include "rules.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

// Helper

bool CheckForAttackingPieceInLine(const Board* const board, const int x, const int y, const int xIncrement, const int yIncrement, const Piece targetPiece, const Piece targetPieceVariant, const Piece queenPiece)
{
    Piece piece;
    int i = 1;
    do
    {
        piece = GetPiece(board, x + xIncrement * i, y + yIncrement * i);
        if (piece == targetPiece || piece == targetPieceVariant || piece == queenPiece)
        {
            return true;
        }

        ++i;
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

    Piece piece;
    // Pawn
    if (attackingColour == White)
    {
        piece = GetPiece(board, x - 1, y - 1);
        if (piece == WhitePawn || piece == WhiteUnmovedPawn) return true;
        piece = GetPiece(board, x + 1, y - 1);
        if (piece == WhitePawn || piece == WhiteUnmovedPawn) return true;
    }
    else
    {
        piece = GetPiece(board, x - 1, y + 1);
        if (piece == BlackPawn || piece == BlackUnmovedPawn) return true;
        piece = GetPiece(board, x + 1, y + 1);
        if (piece == BlackPawn || piece == BlackUnmovedPawn) return true;
    }

    // King
    piece = GetPiece(board, x + 1, y);
    if (piece == whiteToBlack + WhiteKing || piece == whiteToBlack + WhiteUnmovedKing) return true;
    piece = GetPiece(board, x - 1, y);
    if (piece == whiteToBlack + WhiteKing || piece == whiteToBlack + WhiteUnmovedKing) return true;
    piece = GetPiece(board, x, y + 1);
    if (piece == whiteToBlack + WhiteKing || piece == whiteToBlack + WhiteUnmovedKing) return true;
    piece = GetPiece(board, x, y - 1);
    if (piece == whiteToBlack + WhiteKing || piece == whiteToBlack + WhiteUnmovedKing) return true;
    piece = GetPiece(board, x + 1, y + 1);
    if (piece == whiteToBlack + WhiteKing || piece == whiteToBlack + WhiteUnmovedKing) return true;
    piece = GetPiece(board, x + 1, y - 1);
    if (piece == whiteToBlack + WhiteKing || piece == whiteToBlack + WhiteUnmovedKing) return true;
    piece = GetPiece(board, x - 1, y + 1);
    if (piece == whiteToBlack + WhiteKing || piece == whiteToBlack + WhiteUnmovedKing) return true;
    piece = GetPiece(board, x - 1, y - 1);
    if (piece == whiteToBlack + WhiteKing || piece == whiteToBlack + WhiteUnmovedKing) return true;

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
    if (CheckForAttackingPieceInLine(board, x, y, 1, 0, whiteToBlack + WhiteRook, whiteToBlack + WhiteUnmovedRook, whiteToBlack + WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, -1, 0, whiteToBlack + WhiteRook, whiteToBlack + WhiteUnmovedRook, whiteToBlack + WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, 0, 1, whiteToBlack + WhiteRook, whiteToBlack + WhiteUnmovedRook, whiteToBlack + WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, 0, -1, whiteToBlack + WhiteRook, whiteToBlack + WhiteUnmovedRook, whiteToBlack + WhiteQueen)) return true;

    // Bishop and second part of Queen
    if (CheckForAttackingPieceInLine(board, x, y, 1, 1, whiteToBlack + WhiteBishop, whiteToBlack + WhiteBishop, whiteToBlack + WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, -1, 1, whiteToBlack + WhiteBishop, whiteToBlack + WhiteBishop, whiteToBlack + WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, 1, -1, whiteToBlack + WhiteBishop, whiteToBlack + WhiteBishop, whiteToBlack + WhiteQueen)) return true;
    if (CheckForAttackingPieceInLine(board, x, y, -1, -1, whiteToBlack + WhiteBishop, whiteToBlack + WhiteBishop, whiteToBlack + WhiteQueen)) return true;

    return false;
}

bool IsColourInCheck(const Board* const board, const Colour teamColour)
{
    int kingXPos, kingYPos;
    if (teamColour == White)
    {
        IndexToPosition(board->whiteKingIndex, &kingXPos, &kingYPos);
        return IsPositionAttackedByColour(board, kingXPos, kingYPos, Black);
    }
    else
    {
        IndexToPosition(board->blackKingIndex, &kingXPos, &kingYPos);
        return IsPositionAttackedByColour(board, kingXPos, kingYPos, White);
    }
}

bool IsSquareBlockedOrOutside(const Board* const board, const int x, const int y, const Colour teamColour)
{
    Piece squarePiece = GetPiece(board, x, y);
    return squarePiece == OutsideSquare || GetColour(squarePiece) == teamColour;
}

    // Only call if no moves are available
GameEndStatus GetResultOfGameIfNoMovesAreAvailable(const Board* const board)
{
    Colour colourOfTurn = board->isWhiteTurn ? White : Black;
    if (IsColourInCheck(board, colourOfTurn))
    {
        return Checkmate;
    }
    else
    {
        return Stalemate;
    }
}
  
    // Does not change unmoved enum. Changes whose turn it is. Resets En Passant
void MovePiece(Board* const board, const int xStart, const int yStart, const int xEnd, const int yEnd)
{
    Piece piece = GetPiece(board, xStart, yStart);
    board->isWhiteTurn = GetColour(piece) == Black;

    SetPiece(board, xEnd, yEnd, piece);
    SetPiece(board, xStart, yStart, EmptySquare);

    board->enPassantIndex = -1;
}
void MoveKing(Board* const board, const int xStart, const int yStart, const int xEnd, const int yEnd, const Colour teamColour)
{
    if (teamColour == White)
    {
        MovePiece(board, xStart, yStart, xEnd, yEnd);
        SetPiece(board, xEnd, yEnd, WhiteKing);
        board->whiteKingIndex = PositionToIndex(xEnd, yEnd);
    }
    else
    {
        MovePiece(board, xStart, yStart, xEnd, yEnd);
        SetPiece(board, xEnd, yEnd, BlackKing);
        board->blackKingIndex = PositionToIndex(xEnd, yEnd);
    }
}
    // Deals with En Passant. This will promote if you land on that rank. Unmoved gets removed by the parameters
void MovePawn(Board* const board, const int xStart, const int yStart, const int xEnd, const int yEnd, const Piece normalMovedPawn, const Piece promotionPiece)
{
    MovePiece(board, xStart, yStart, xEnd, yEnd);
    // promotion
    if (yEnd == 7 || yEnd == 0)
    {
        SetPiece(board, xEnd, yEnd, promotionPiece); // no need to handle colours separately because of this
    }
    else
    {
        SetPiece(board, xEnd, yEnd, normalMovedPawn);
    }
    // Double first enables En Passant. Must be after MovePiece since it resets it.
    if (abs(yStart - yEnd) == 2)
    {
        board->enPassantIndex = PositionToIndex(xEnd, yEnd);
    }
}
    // normalMovedRook should have the correct colour and handles removing unmoved automatically
void MoveRook(Board* const board, const int xStart, const int yStart, const int xEnd, const int yEnd, const Piece normalMovedRook)
{
    MovePiece(board, xStart, yStart, xEnd, yEnd);
    SetPiece(board, xEnd, yEnd, normalMovedRook);
}


// Generate Moves Helper

void AddNormalMoveIfLegal(const Board* const board, Board* const newMoves, int* const outAmountOfMoves, const int xStart, const int yStart, const int xEnd, const int yEnd, const Colour teamColour)
{
    if (!IsSquareBlockedOrOutside(board, xEnd , yEnd, teamColour))
    {
        newMoves[*outAmountOfMoves] = *board;
        MovePiece(&newMoves[*outAmountOfMoves], xStart, yStart, xEnd, yEnd);
        if (!IsColourInCheck(&newMoves[*outAmountOfMoves], teamColour))
        {
            ++(*outAmountOfMoves);
        }
    }
}

void AddKingMoveIfLegal(const Board* const board, Board* const newMoves, int* const outAmountOfMoves, const int xStart, const int yStart, const int xEnd, const int yEnd, const Colour teamColour)
{
    if (!IsSquareBlockedOrOutside(board, xEnd , yEnd, teamColour))
    {
        newMoves[*outAmountOfMoves] = *board;
        MoveKing(&newMoves[*outAmountOfMoves], xStart, yStart, xEnd, yEnd, teamColour);
        if (!IsColourInCheck(&newMoves[*outAmountOfMoves], teamColour))
        {
            ++(*outAmountOfMoves);
        }
    }
}

void AddMovesAlongDirectionIfLegal(const Board* const board, Board* const newMoves, int* const outAmountOfMoves, const int x, const int y, const int xIncrement, const int yIncrement, const Colour teamColour, const Colour oppositeColour)
{
    Piece piece;
    Colour pieceColour;
    int i = 1;
    do
    {
        piece = GetPiece(board, x + xIncrement * i, y + yIncrement * i);
        pieceColour = GetColour(piece);
        if (piece == EmptySquare || pieceColour == oppositeColour)
        {
            newMoves[*outAmountOfMoves] = *board;
            MovePiece(&newMoves[*outAmountOfMoves], x, y, x + xIncrement * i, y + yIncrement * i);
            if (!IsColourInCheck(&newMoves[*outAmountOfMoves], teamColour))
            {
                ++(*outAmountOfMoves);
            }
        }
        else
        {
            break;
        }

        ++i;
    } while (pieceColour != oppositeColour);
}

void AddRookMovesAlongDirectionIfLegal(const Board* const board, Board* const newMoves, int* const outAmountOfMoves, const int x, const int y, const int xIncrement, const int yIncrement, const Colour teamColour, const Colour oppositeColour, const Piece normalMovedRook)
{
    Piece piece;
    Colour pieceColour;
    int i = 1;
    do
    {
        piece = GetPiece(board, x + xIncrement * i, y + yIncrement * i);
        pieceColour = GetColour(piece);
        if (piece == EmptySquare || pieceColour == oppositeColour)
        {
            newMoves[*outAmountOfMoves] = *board;
            MoveRook(&newMoves[*outAmountOfMoves], x, y, x + xIncrement * i, y + yIncrement * i, normalMovedRook);
            if (!IsColourInCheck(&newMoves[*outAmountOfMoves], teamColour))
            {
                ++(*outAmountOfMoves);
            }
        }
        else
        {
            break;
        }

        ++i;
    } while (pieceColour != oppositeColour);
}


// Generate Moves

void GenerateLegalMovesForKing(const Board* const board, Board* const newMoves, int* const outAmountOfMoves, const int x, const int y, const Colour teamColour, const Colour oppositeColour)
{
    // Normal moves
    AddKingMoveIfLegal(board, newMoves, outAmountOfMoves, x, y, x + 1, y, teamColour);
    AddKingMoveIfLegal(board, newMoves, outAmountOfMoves, x, y, x - 1, y, teamColour);
    AddKingMoveIfLegal(board, newMoves, outAmountOfMoves, x, y, x, y + 1, teamColour);
    AddKingMoveIfLegal(board, newMoves, outAmountOfMoves, x, y, x, y - 1, teamColour);
    AddKingMoveIfLegal(board, newMoves, outAmountOfMoves, x, y, x + 1, y + 1, teamColour);
    AddKingMoveIfLegal(board, newMoves, outAmountOfMoves, x, y, x + 1, y - 1, teamColour);
    AddKingMoveIfLegal(board, newMoves, outAmountOfMoves, x, y, x - 1, y + 1, teamColour);
    AddKingMoveIfLegal(board, newMoves, outAmountOfMoves, x, y, x - 1, y - 1, teamColour);

    if (!IsColourInCheck(board, teamColour))
    {
        // Castling king-side
        if (teamColour == White)
        {
            if (GetPiece(board, x, y) == WhiteUnmovedKing && GetPiece(board, 7, 0) == WhiteUnmovedRook)
            {
                if (GetPiece(board, x + 1, y) == EmptySquare && GetPiece(board, x + 2, y) == EmptySquare)
                {
                    if (!IsPositionAttackedByColour(board, x + 1, y, Black) && !IsPositionAttackedByColour(board, x + 2, y, Black))
                    {
                        newMoves[*outAmountOfMoves] = *board;
                        MoveKing(&newMoves[*outAmountOfMoves], x, y, x + 2, y, teamColour);
                        MoveRook(&newMoves[*outAmountOfMoves], 7, 0, 5, 0, WhiteRook);
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
                    if (!IsPositionAttackedByColour(board, x + 1, y, White) && !IsPositionAttackedByColour(board, x + 2, y, White))
                    {
                        newMoves[*outAmountOfMoves] = *board;
                        MoveKing(&newMoves[*outAmountOfMoves], x, y, x + 2, y, teamColour);
                        MoveRook(&newMoves[*outAmountOfMoves], 7, 7, 5, 7, BlackRook);
                        ++(*outAmountOfMoves);
                    }
                }
            }
        }
        
        // Castling queen-side
        if (teamColour == White)
        {
            if (GetPiece(board, x, y) == WhiteUnmovedKing && GetPiece(board, 0, 0) == WhiteUnmovedRook)
            {
                if (GetPiece(board, x - 1, y) == EmptySquare && GetPiece(board, x - 2, y) == EmptySquare && GetPiece(board, x - 3, y) == EmptySquare)
                {
                    if (!IsPositionAttackedByColour(board, x - 1, y, Black) && !IsPositionAttackedByColour(board, x - 2, y, Black))
                    {
                        newMoves[*outAmountOfMoves] = *board;
                        MoveKing(&newMoves[*outAmountOfMoves], x, y, x - 2, y, teamColour);
                        MoveRook(&newMoves[*outAmountOfMoves], 0, 0, 3, 0, WhiteRook);
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
                    if (!IsPositionAttackedByColour(board, x - 1, y, White) && !IsPositionAttackedByColour(board, x - 2, y, White))
                    {
                        newMoves[*outAmountOfMoves] = *board;
                        MoveKing(&newMoves[*outAmountOfMoves], x, y, x - 2, y, teamColour);
                        MoveRook(&newMoves[*outAmountOfMoves], 0, 7, 3, 7, BlackRook);
                        ++(*outAmountOfMoves);
                    }
                }
            }
        }
    }
}
    
    // Currently always promotes to a queen
void GenerateLegalMovesForPawn(const Board* const board, Board* const newMoves, int* const outAmountOfMoves, const int x, const int y, const Colour teamColour, const Colour oppositeColour)
{
    // En Passant position
    int enPassantX, enPassantY;
    if (board->enPassantIndex != -1)
    {
        IndexToPosition(board->enPassantIndex, &enPassantX, &enPassantY);
    }

    int forwardDirection = teamColour == White ? 1 : -1;
    Piece pawnPiece = teamColour == White ? WhitePawn : BlackPawn;
    Piece unmovedPawnPiece = teamColour == White ? WhiteUnmovedPawn : BlackUnmovedPawn;
    Piece promotionPiece = teamColour == White ? WhiteQueen : BlackQueen;
    // Moves
    // Forward 1
    if (GetPiece(board, x, y + forwardDirection) == EmptySquare)
    {
        newMoves[*outAmountOfMoves] = *board;
        MovePawn(&newMoves[*outAmountOfMoves], x, y, x, y + forwardDirection, pawnPiece, promotionPiece);
        if (!IsColourInCheck(&newMoves[*outAmountOfMoves], teamColour))
        {
            ++(*outAmountOfMoves);
        }
    }

    // Forward 2
    if (GetPiece(board, x, y) == unmovedPawnPiece)
    {
        if (GetPiece(board, x, y + forwardDirection) == EmptySquare && GetPiece(board, x, y + 2 * forwardDirection) == EmptySquare)
        {
            newMoves[*outAmountOfMoves] = *board;
            MovePawn(&newMoves[*outAmountOfMoves], x, y, x, y + 2 * forwardDirection, pawnPiece, promotionPiece);
            if (!IsColourInCheck(&newMoves[*outAmountOfMoves], teamColour))
            {
                ++(*outAmountOfMoves);
            }
        }
    }

    // Captures
    if (GetColour(GetPiece(board, x + 1, y + forwardDirection)) == oppositeColour)
    {
        newMoves[*outAmountOfMoves] = *board;
        MovePawn(&newMoves[*outAmountOfMoves], x, y, x + 1, y + forwardDirection, pawnPiece, promotionPiece);
        if (!IsColourInCheck(&newMoves[*outAmountOfMoves], teamColour))
        {
            ++(*outAmountOfMoves);
        }
    }
    if (GetColour(GetPiece(board, x - 1, y + forwardDirection)) == oppositeColour)
    {
        newMoves[*outAmountOfMoves] = *board;
        MovePawn(&newMoves[*outAmountOfMoves], x, y, x - 1, y + forwardDirection, pawnPiece, promotionPiece);
        if (!IsColourInCheck(&newMoves[*outAmountOfMoves], teamColour))
        {
            ++(*outAmountOfMoves);
        }
    }

    // En Passant capture
    if (board->enPassantIndex != -1)
    {
        if (enPassantY == y && abs(enPassantX - x) == 1)
        {
            newMoves[*outAmountOfMoves] = *board;
            MovePawn(&newMoves[*outAmountOfMoves], x, y, enPassantX, y + forwardDirection, pawnPiece, promotionPiece);
            newMoves[*outAmountOfMoves].board[board->enPassantIndex] = EmptySquare; // This is not handled by MovePawn
            if (!IsColourInCheck(&newMoves[*outAmountOfMoves], teamColour))
            {
                ++(*outAmountOfMoves);
            }
        }
    }
}

void GenerateLegalMovesForKnight(const Board* const board, Board* const newMoves, int* const outAmountOfMoves, const int x, const int y, const Colour teamColour, const Colour oppositeColour)
{
    AddNormalMoveIfLegal(board, newMoves, outAmountOfMoves, x, y, x + 1, y + 2, teamColour);
    AddNormalMoveIfLegal(board, newMoves, outAmountOfMoves, x, y, x - 1, y + 2, teamColour);
    AddNormalMoveIfLegal(board, newMoves, outAmountOfMoves, x, y, x + 2, y + 1, teamColour);
    AddNormalMoveIfLegal(board, newMoves, outAmountOfMoves, x, y, x - 2, y + 1, teamColour);
    AddNormalMoveIfLegal(board, newMoves, outAmountOfMoves, x, y, x + 1, y - 2, teamColour);
    AddNormalMoveIfLegal(board, newMoves, outAmountOfMoves, x, y, x - 1, y - 2, teamColour);
    AddNormalMoveIfLegal(board, newMoves, outAmountOfMoves, x, y, x + 2, y - 1, teamColour);
    AddNormalMoveIfLegal(board, newMoves, outAmountOfMoves, x, y, x - 2, y - 1, teamColour);
}

void GenerateLegalMovesForRook(const Board* const board, Board* const newMoves, int* const outAmountOfMoves, const int x, const int y, const Colour teamColour, const Colour oppositeColour)
{
    Piece rookPiece = teamColour == White ? WhiteRook : BlackRook;
    AddRookMovesAlongDirectionIfLegal(board, newMoves, outAmountOfMoves, x, y, 1, 0, teamColour, oppositeColour, rookPiece);
    AddRookMovesAlongDirectionIfLegal(board, newMoves, outAmountOfMoves, x, y, -1, 0, teamColour, oppositeColour, rookPiece);
    AddRookMovesAlongDirectionIfLegal(board, newMoves, outAmountOfMoves, x, y, 0, 1, teamColour, oppositeColour, rookPiece);
    AddRookMovesAlongDirectionIfLegal(board, newMoves, outAmountOfMoves, x, y, 0, -1, teamColour, oppositeColour, rookPiece);
}

void GenerateLegalMovesForBishop(const Board* const board, Board* const newMoves, int* const outAmountOfMoves, const int x, const int y, const Colour teamColour, const Colour oppositeColour)
{
    AddMovesAlongDirectionIfLegal(board, newMoves, outAmountOfMoves, x, y, 1, 1, teamColour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, newMoves, outAmountOfMoves, x, y, -1, -1, teamColour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, newMoves, outAmountOfMoves, x, y, -1, 1, teamColour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, newMoves, outAmountOfMoves, x, y, 1, -1, teamColour, oppositeColour);
}

void GenerateLegalMovesForQueen(const Board* const board, Board* const newMoves, int* const outAmountOfMoves, const int x, const int y, const Colour teamColour, const Colour oppositeColour)
{
    AddMovesAlongDirectionIfLegal(board, newMoves, outAmountOfMoves, x, y, 1, 0, teamColour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, newMoves, outAmountOfMoves, x, y, -1, 0, teamColour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, newMoves, outAmountOfMoves, x, y, 0, 1, teamColour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, newMoves, outAmountOfMoves, x, y, 0, -1, teamColour, oppositeColour);

    AddMovesAlongDirectionIfLegal(board, newMoves, outAmountOfMoves, x, y, 1, 1, teamColour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, newMoves, outAmountOfMoves, x, y, -1, -1, teamColour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, newMoves, outAmountOfMoves, x, y, -1, 1, teamColour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, newMoves, outAmountOfMoves, x, y, 1, -1, teamColour, oppositeColour);
}

void GenerateAllLegalMoves(const Board* const board, Board* const newMoves, int* const outAmountOfMoves)
{
    *outAmountOfMoves = 0;
    Colour teamColour = board->isWhiteTurn ? White : Black;
    Colour oppositeColour = teamColour == White ? Black : White;

    for (int i = 0; i < 64; ++i)
    {
        Piece piece = board->board[i];
        Colour pieceColour = GetColour(piece);
        
        if (pieceColour == teamColour)
        {
            int x, y;
            IndexToPosition(i, &x, &y);

            switch (piece)
            {
            case WhitePawn:
            case WhiteUnmovedPawn:
            case BlackPawn:
            case BlackUnmovedPawn:
                GenerateLegalMovesForPawn(board, newMoves, outAmountOfMoves, x, y, teamColour, oppositeColour);
                break;
            case WhiteRook:
            case WhiteUnmovedRook:
            case BlackRook:
            case BlackUnmovedRook:
                GenerateLegalMovesForRook(board, newMoves, outAmountOfMoves, x, y, teamColour, oppositeColour);
                break;
            case WhiteKnight:
            case BlackKnight:
                GenerateLegalMovesForKnight(board, newMoves, outAmountOfMoves, x, y, teamColour, oppositeColour);
                break;
            case WhiteBishop:
            case BlackBishop:
                GenerateLegalMovesForBishop(board, newMoves, outAmountOfMoves, x, y, teamColour, oppositeColour);
                break;
            case WhiteQueen:
            case BlackQueen:
                GenerateLegalMovesForQueen(board, newMoves, outAmountOfMoves, x, y, teamColour, oppositeColour);
                break;
            case WhiteKing:
            case WhiteUnmovedKing:
            case BlackKing:
            case BlackUnmovedKing:
                GenerateLegalMovesForKing(board, newMoves, outAmountOfMoves, x, y, teamColour, oppositeColour);
                break;
            default:
                assert(false && "In GenerateAllLegalMoves: All pieces aren't covered in the switch statement");
                break;
            }
        }
    }
}
