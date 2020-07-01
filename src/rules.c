#include "rules.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

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

    // Does not change unmoved enum. Changes whose turn it is
void MovePiece(Board* const board, const int xStart, const int yStart, const int xEnd, const int yEnd)
{
    board->isWhiteTurn = !board->isWhiteTurn;

    SetPiece(board, xEnd, yEnd, GetPiece(board, xStart, yStart));
    SetPiece(board, xStart, yStart, EmptySquare);
}
void MoveKing(Board* const board, const int xStart, const int yStart, const int xEnd, const int yEnd)
{
    if (board->isWhiteTurn)
    {
        MovePiece(board, xStart, yStart, xEnd, yEnd);
        board->whiteKingIndex = PositionToIndex(xEnd, yEnd);
        if (GetPiece(board, xEnd, yEnd) == WhiteUnmovedKing)
        {
            SetPiece(board, xEnd, yEnd, WhiteKing);
        }
    }
    else
    {
        MovePiece(board, xStart, yStart, xEnd, yEnd);
        board->blackKingIndex = PositionToIndex(xEnd, yEnd);
        if (GetPiece(board, xEnd, yEnd) == BlackUnmovedKing)
        {
            SetPiece(board, xEnd, yEnd, BlackUnmovedKing);
        }
    }
}
    // Deals with En Passant. promotionOrNormalMovedPawn will be the piece you want to promote to if you are promoting, or alternatively just White/BlackPawn. Therefore unmoved is removed automatically
void MovePawn(Board* const board, const int xStart, const int yStart, const int xEnd, const int yEnd, const Piece promotionOrNormalMovedPawn)
{
    // Double first enables En Passant
    if (abs(yStart - yEnd) == 2)
    {
        board->enPassantIndex = PositionToIndex(xEnd, yEnd);
    }
    SetPiece(board, xStart, yStart, promotionOrNormalMovedPawn); // no need to handle colours separately because of this
    MovePiece(board, xStart, yStart, xEnd, yEnd);
}
    // normalMovedRook should have the correct colour and handles removing unmoved automatically
void MoveRook(Board* const board, const int xStart, const int yStart, const int xEnd, const int yEnd, const Piece normalMovedRook)
{
    MovePiece(board, xStart, yStart, xEnd, yEnd);
    SetPiece(board, xEnd, yEnd, normalMovedRook);
}


// Generate Moves Helper

void AddNormalMoveIfLegal(const Board* const board, int* const outAmountOfMoves, Board* const newBoard, const int xOld, const int yOld, const int x, const int y, const Colour colour, const Colour oppositeColour)
{
    if (!IsSquareBlockedOrOutside(board, x , y, colour))
    {
        *newBoard = *board;
        MovePiece(newBoard, xOld, yOld, x, y);
        if (!IsColourInCheck(newBoard, oppositeColour))
        {
            ++(*outAmountOfMoves);
        }
    }
}

void AddKingMoveIfLegal(const Board* const board, int* const outAmountOfMoves, Board* const newBoard, const int xOld, const int yOld, const int x, const int y, const Colour colour, const Colour oppositeColour)
{
    if (!IsSquareBlockedOrOutside(board, x , y, colour))
    {
        *newBoard = *board;
        MoveKing(newBoard, xOld, yOld, x, y);
        if (!IsColourInCheck(newBoard, oppositeColour))
        {
            ++(*outAmountOfMoves);
        }
    }
}

void AddMovesAlongDirectionIfLegal(const Board* const board, int* const outAmountOfMoves, Board* const moves, const int x, const int y, const int xIncrement, const int yIncrement, const Colour colour, const Colour oppositeColour)
{
    Piece piece;
    int i = 1;
    do
    {
        piece = GetPiece(board, x + xIncrement * i, y + yIncrement * i);
        if (piece == EmptySquare || GetColour(piece) == oppositeColour)
        {
            moves[*outAmountOfMoves] = *board;
            MovePiece(&moves[*outAmountOfMoves], x, y, x + xIncrement * i, y + yIncrement * i);
            if (!IsColourInCheck(&moves[*outAmountOfMoves], colour))
            {
                ++(*outAmountOfMoves);
            }
        }
        else
        {
            break;
        }

        i++;
    } while (piece != oppositeColour);
}

void AddRookMovesAlongDirectionIfLegal(const Board* const board, int* const outAmountOfMoves, Board* const moves, const int x, const int y, const int xIncrement, const int yIncrement, const Colour colour, const Colour oppositeColour, const Piece normalMovedRook)
{
    Piece piece;
    int i = 1;
    do
    {
        piece = GetPiece(board, x + xIncrement * i, y + yIncrement * i);
        if (piece == EmptySquare || GetColour(piece) == oppositeColour)
        {
            moves[*outAmountOfMoves] = *board;
            MoveRook(&moves[*outAmountOfMoves], x, y, x + xIncrement * i, y + yIncrement * i, normalMovedRook);
            if (!IsColourInCheck(&moves[*outAmountOfMoves], colour))
            {
                ++(*outAmountOfMoves);
            }
        }
        else
        {
            break;
        }

        i++;
    } while (piece != oppositeColour);
}


// Generate Moves

Board* GenerateLegalMovesForKing(const Board* const board, int* const outAmountOfMoves, const int x, const int y)
{
    *outAmountOfMoves = 0;
    Board* moves = malloc(sizeof(*moves) * 10);
    assert(moves && "In GenerateLegalMovesForKing: Failed to allocate memory for moves");

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

    if (!IsColourInCheck(board, colour))
    {
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
                        MoveRook(&moves[*outAmountOfMoves], 7, 0, 5, 0, WhiteRook);
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
                        MoveRook(&moves[*outAmountOfMoves], 7, 7, 5, 7, BlackRook);
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
                        MoveRook(&moves[*outAmountOfMoves], 0, 0, 3, 0, WhiteRook);
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
                        MoveRook(&moves[*outAmountOfMoves], 0, 7, 3, 7, BlackRook);
                        ++(*outAmountOfMoves);
                    }
                }
            }
        }
    }

    //moves = realloc(moves, sizeof(*moves) * (*outAmountOfMoves));
    //assert(moves && "In GenerateLegalMovesForKing: Failed to reallocate memory for moves");
    return moves;
}

    // Resets En Passant to -1 if it is over. Currently always promotes to a queen
Board* GenerateLegalMovesForPawn(Board* const board, int* const outAmountOfMoves, const int x, const int y)
{
    *outAmountOfMoves = 0;
    Board* moves = malloc(sizeof(*moves) * 5);
    assert(moves && "In GenerateLegalMovesForPawn: Failed to allocate memory for moves");

    Colour colour = board->isWhiteTurn ? White : Black;
    Colour oppositeColour = colour == White ? Black : White;

    // En Passant index reset
    int enPassantX, enPassantY;
    if (board->enPassantIndex != -1)
    {
        IndexToPosition(board->enPassantIndex, &enPassantX, &enPassantY);
        if (GetColour(GetPiece(board, enPassantX, enPassantY)) == colour)
        {
            board->enPassantIndex = -1;
        }
    }

    int forwardDirection = colour == White ? 1 : -1;
    int promotionLine = colour == White ? 6 : 1;
    Piece pawnPiece = colour == White ? WhitePawn : BlackPawn;
    Piece unmovedPawnPiece = colour == White ? WhiteUnmovedPawn : BlackUnmovedPawn;
    Piece promotionPiece = colour == White ? WhiteQueen : BlackQueen;
    // Moves
    // Forward 1
    if (!IsSquareBlockedOrOutside(board, x , y + forwardDirection, colour))
    {
        moves[*outAmountOfMoves] = *board;
        // promotion
        if (y == promotionLine)
        {
            MovePawn(&moves[*outAmountOfMoves], x, y, x, y + forwardDirection, promotionPiece);
        } 
        else
        {
            MovePawn(&moves[*outAmountOfMoves], x, y, x, y + forwardDirection, pawnPiece);
        }

        if (!IsColourInCheck(&moves[*outAmountOfMoves], colour))
        {
            ++(*outAmountOfMoves);
        }
    }

    // Forward 2
    if (GetPiece(board, x, y) == unmovedPawnPiece)
    {
        if (!IsSquareBlockedOrOutside(board, x, y + forwardDirection, colour) && !IsSquareBlockedOrOutside(board, x, y + 2 * forwardDirection, colour))
        {
            moves[*outAmountOfMoves] = *board;
            MovePawn(&moves[*outAmountOfMoves], x, y, x, y + 2 * forwardDirection, pawnPiece);
            if (!IsColourInCheck(&moves[*outAmountOfMoves], colour))
            {
                ++(*outAmountOfMoves);
            }
        }
    }

    // Captures
    if (GetColour(GetPiece(board, x + 1, y + forwardDirection)) == oppositeColour)
    {
        moves[*outAmountOfMoves] = *board;
        MovePawn(&moves[*outAmountOfMoves], x, y, x + 1, y + forwardDirection, pawnPiece);
        if (!IsColourInCheck(&moves[*outAmountOfMoves], colour))
        {
            ++(*outAmountOfMoves);
        }
    }
    if (GetColour(GetPiece(board, x - 1, y + forwardDirection)) == oppositeColour)
    {
        moves[*outAmountOfMoves] = *board;
        MovePawn(&moves[*outAmountOfMoves], x, y, x - 1, y + forwardDirection, pawnPiece);
        if (!IsColourInCheck(&moves[*outAmountOfMoves], colour))
        {
            ++(*outAmountOfMoves);
        }
    }

    // En Passant capture
    if (board->enPassantIndex != -1)
    {
        if (enPassantY == y && abs(enPassantX - x) == 1)
        {
            moves[*outAmountOfMoves] = *board;
            MovePawn(&moves[*outAmountOfMoves], x, y, enPassantX, y + forwardDirection, pawnPiece);
            moves[*outAmountOfMoves].board[board->enPassantIndex] = EmptySquare; // This is not handled by MovePawn
            if (!IsColourInCheck(&moves[*outAmountOfMoves], colour))
            {
                ++(*outAmountOfMoves);
            }
        }
    }

    //moves = realloc(moves, sizeof(*moves) * (*outAmountOfMoves));
    //assert(moves && "In GenerateLegalMovesForPawn: Failed to reallocate memory for moves");
    return moves;
}

Board* GenerateLegalMovesForKnight(const Board* const board, int* const outAmountOfMoves, const int x, const int y)
{
    *outAmountOfMoves = 0;
    Board* moves = malloc(sizeof(*moves) * 8);
    assert(moves && "In GenerateLegalMovesForKnight: Failed to allocate memory for moves");

    Colour colour = board->isWhiteTurn ? White : Black;
    Colour oppositeColour = colour == White ? Black : White;

    AddNormalMoveIfLegal(board, outAmountOfMoves, &moves[*outAmountOfMoves], x, y, x + 1, y + 2, colour, oppositeColour);
    AddNormalMoveIfLegal(board, outAmountOfMoves, &moves[*outAmountOfMoves], x, y, x - 1, y + 2, colour, oppositeColour);
    AddNormalMoveIfLegal(board, outAmountOfMoves, &moves[*outAmountOfMoves], x, y, x + 2, y + 1, colour, oppositeColour);
    AddNormalMoveIfLegal(board, outAmountOfMoves, &moves[*outAmountOfMoves], x, y, x - 2, y + 1, colour, oppositeColour);
    AddNormalMoveIfLegal(board, outAmountOfMoves, &moves[*outAmountOfMoves], x, y, x + 1, y - 2, colour, oppositeColour);
    AddNormalMoveIfLegal(board, outAmountOfMoves, &moves[*outAmountOfMoves], x, y, x - 1, y - 2, colour, oppositeColour);
    AddNormalMoveIfLegal(board, outAmountOfMoves, &moves[*outAmountOfMoves], x, y, x + 2, y - 1, colour, oppositeColour);
    AddNormalMoveIfLegal(board, outAmountOfMoves, &moves[*outAmountOfMoves], x, y, x - 2, y - 1, colour, oppositeColour);

    //moves = realloc(moves, sizeof(*moves) * (*outAmountOfMoves));
    //assert(moves && "In GenerateLegalMovesForKnight: Failed to reallocate memory for moves");
    return moves;
}

Board* GenerateLegalMovesForRook(const Board* const board, int* const outAmountOfMoves, const int x, const int y)
{
    *outAmountOfMoves = 0;
    Board* moves = malloc(sizeof(*moves) * 14);
    assert(moves && "In GenerateLegalMovesForRook: Failed to allocate memory for moves");

    Colour colour = board->isWhiteTurn ? White : Black;
    Colour oppositeColour = colour == White ? Black : White;

    Piece rookPiece = colour == White ? WhiteRook : BlackRook;
    AddRookMovesAlongDirectionIfLegal(board, outAmountOfMoves, moves, x, y, 1, 0, colour, oppositeColour, rookPiece);
    AddRookMovesAlongDirectionIfLegal(board, outAmountOfMoves, moves, x, y, -1, 0, colour, oppositeColour, rookPiece);
    AddRookMovesAlongDirectionIfLegal(board, outAmountOfMoves, moves, x, y, 0, 1, colour, oppositeColour, rookPiece);
    AddRookMovesAlongDirectionIfLegal(board, outAmountOfMoves, moves, x, y, 0, -1, colour, oppositeColour, rookPiece);

    //moves = realloc(moves, sizeof(*moves) * (*outAmountOfMoves));
    //assert(moves && "In GenerateLegalMovesForRook: Failed to reallocate memory for moves");
    return moves;
}

Board* GenerateLegalMovesForBishop(const Board* const board, int* const outAmountOfMoves, const int x, const int y)
{
    *outAmountOfMoves = 0;
    Board* moves = malloc(sizeof(*moves) * 13);
    assert(moves && "In GenerateLegalMovesForBishop: Failed to allocate memory for moves");

    Colour colour = board->isWhiteTurn ? White : Black;
    Colour oppositeColour = colour == White ? Black : White;

    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, moves, x, y, 1, 1, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, moves, x, y, -1, -1, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, moves, x, y, -1, 1, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, moves, x, y, 1, -1, colour, oppositeColour);

    //moves = realloc(moves, sizeof(*moves) * (*outAmountOfMoves));
    //assert(moves && "In GenerateLegalMovesForBishop: Failed to reallocate memory for moves");
    return moves;
}

Board* GenerateLegalMovesForQueen(const Board* const board, int* const outAmountOfMoves, const int x, const int y)
{
    *outAmountOfMoves = 0;
    Board* moves = malloc(sizeof(*moves) * 27);
    assert(moves && "In GenerateLegalMovesForQueen: Failed to allocate memory for moves");

    Colour colour = board->isWhiteTurn ? White : Black;
    Colour oppositeColour = colour == White ? Black : White;

    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, moves, x, y, 1, 0, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, moves, x, y, -1, 0, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, moves, x, y, 0, 1, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, moves, x, y, 0, -1, colour, oppositeColour);

    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, moves, x, y, 1, 1, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, moves, x, y, -1, -1, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, moves, x, y, -1, 1, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, moves, x, y, 1, -1, colour, oppositeColour);

    //moves = realloc(moves, sizeof(*moves) * (*outAmountOfMoves));
    //assert(moves && "In GenerateLegalMovesForQueen: Failed to reallocate memory for moves");
    return moves;
}

Board* GenerateAllLegalMoves(Board* const board, int* const outAmountOfMoves)
{
    *outAmountOfMoves = 0;
    Colour colourOfTurn = board->isWhiteTurn ? White : Black;

    Board* newMoves[64] = { NULL };
    int newMovesCount[64] = { 0 };
    for (int i = 0; i < 64; ++i)
    {
        Piece piece = board->board[i];
        Colour colour = GetColour(piece);
        
        newMovesCount[i] = 0;
        if (colour == colourOfTurn)
        {
            int x, y;
            IndexToPosition(i, &x, &y);

            switch (piece)
            {
            case WhitePawn:
            case WhiteUnmovedPawn:
            case BlackPawn:
            case BlackUnmovedPawn:
                newMoves[i] = GenerateLegalMovesForPawn(board, &newMovesCount[i], x, y);
                break;
            case WhiteRook:
            case WhiteUnmovedRook:
            case BlackRook:
            case BlackUnmovedRook:
                newMoves[i] = GenerateLegalMovesForRook(board, &newMovesCount[i], x, y);
                break;
            case WhiteKnight:
            case BlackKnight:
                newMoves[i] = GenerateLegalMovesForKnight(board, &newMovesCount[i], x, y);
                break;
            case WhiteBishop:
            case BlackBishop:
                newMoves[i] = GenerateLegalMovesForBishop(board, &newMovesCount[i], x, y);
                break;
            case WhiteQueen:
            case BlackQueen:
                newMoves[i] = GenerateLegalMovesForQueen(board, &newMovesCount[i], x, y);
                break;
            case WhiteKing:
            case WhiteUnmovedKing:
            case BlackKing:
            case BlackUnmovedKing:
                newMoves[i] = GenerateLegalMovesForKing(board, &newMovesCount[i], x, y);
                break;
            default:
                assert(false && "In GenerateAllLegalMoves: All pieces aren't covered in the switch statement");
                break;
            }

            *outAmountOfMoves += newMovesCount[i];
        }
    }

    Board* moves = malloc(sizeof(*moves) * (*outAmountOfMoves));
    assert(moves && "In GenerateAllLegalMoves: Failed to allocate memory for moves");
    int index = 0;
    for (int i = 0; i < 64; ++i)
    {
        for (int j = 0; j < newMovesCount[i]; ++j)
        {
            moves[index] = newMoves[i][j];
            ++index;
        }
        free(newMoves[i]);
    }

    return moves;
}
