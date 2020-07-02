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
  
    // Does not change unmoved enum. Changes whose turn it is
void MovePiece(Board* const board, const int xStart, const int yStart, const int xEnd, const int yEnd)
{
    Piece piece = GetPiece(board, xStart, yStart);
    board->isWhiteTurn = GetColour(piece) == Black;

    SetPiece(board, xEnd, yEnd, piece);
    SetPiece(board, xStart, yStart, EmptySquare);
}
void MoveKing(Board* const board, const int xStart, const int yStart, const int xEnd, const int yEnd, const Colour colour)
{
    if (colour == White)
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
    // Deals with En Passant. promotionOrNormalMovedPawn will be the piece you want to promote to if you are promoting, or alternatively just White/BlackPawn. Therefore unmoved is removed automatically
void MovePawn(Board* const board, const int xStart, const int yStart, const int xEnd, const int yEnd, const Piece promotionOrNormalMovedPawn)
{
    // Double first enables En Passant
    if (abs(yStart - yEnd) == 2)
    {
        board->enPassantIndex = PositionToIndex(xEnd, yEnd);
    }
    MovePiece(board, xStart, yStart, xEnd, yEnd);
    SetPiece(board, xEnd, yEnd, promotionOrNormalMovedPawn); // no need to handle colours separately because of this
}
    // normalMovedRook should have the correct colour and handles removing unmoved automatically
void MoveRook(Board* const board, const int xStart, const int yStart, const int xEnd, const int yEnd, const Piece normalMovedRook)
{
    MovePiece(board, xStart, yStart, xEnd, yEnd);
    SetPiece(board, xEnd, yEnd, normalMovedRook);
}


// Generate Moves Helper

void AddNormalMoveIfLegal(const Board* const board, int* const outAmountOfMoves, Board* const newBoard, const int xOld, const int yOld, const int x, const int y, const Colour colour)
{
    if (!IsSquareBlockedOrOutside(board, x , y, colour))
    {
        *newBoard = *board;
        MovePiece(newBoard, xOld, yOld, x, y);
        if (!IsColourInCheck(newBoard, colour))
        {
            ++(*outAmountOfMoves);
        }
    }
}

void AddKingMoveIfLegal(const Board* const board, int* const outAmountOfMoves, Board* const newBoard, const int xOld, const int yOld, const int x, const int y, const Colour colour)
{
    if (!IsSquareBlockedOrOutside(board, x , y, colour))
    {
        *newBoard = *board;
        MoveKing(newBoard, xOld, yOld, x, y, colour);
        if (!IsColourInCheck(newBoard, colour))
        {
            ++(*outAmountOfMoves);
        }
    }
}

void AddMovesAlongDirectionIfLegal(const Board* const board, int* const outAmountOfMoves, Board* const moves, const int x, const int y, const int xIncrement, const int yIncrement, const Colour colour, const Colour oppositeColour)
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

        ++i;
    } while (pieceColour != oppositeColour);
}

void AddRookMovesAlongDirectionIfLegal(const Board* const board, int* const outAmountOfMoves, Board* const moves, const int x, const int y, const int xIncrement, const int yIncrement, const Colour colour, const Colour oppositeColour, const Piece normalMovedRook)
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

        ++i;
    } while (pieceColour != oppositeColour);
}


// Generate Moves

void GenerateLegalMovesForKing(const Board* const board, Board* const newBoard, int* const outAmountOfMoves, const int x, const int y, const Colour colour, const Colour oppositeColour)
{
    // Normal moves
    AddKingMoveIfLegal(board, outAmountOfMoves, &newBoard[*outAmountOfMoves], x, y, x + 1, y, colour);
    AddKingMoveIfLegal(board, outAmountOfMoves, &newBoard[*outAmountOfMoves], x, y, x - 1, y, colour);
    AddKingMoveIfLegal(board, outAmountOfMoves, &newBoard[*outAmountOfMoves], x, y, x, y + 1, colour);
    AddKingMoveIfLegal(board, outAmountOfMoves, &newBoard[*outAmountOfMoves], x, y, x, y - 1, colour);
    AddKingMoveIfLegal(board, outAmountOfMoves, &newBoard[*outAmountOfMoves], x, y, x + 1, y + 1, colour);
    AddKingMoveIfLegal(board, outAmountOfMoves, &newBoard[*outAmountOfMoves], x, y, x + 1, y - 1, colour);
    AddKingMoveIfLegal(board, outAmountOfMoves, &newBoard[*outAmountOfMoves], x, y, x - 1, y + 1, colour);
    AddKingMoveIfLegal(board, outAmountOfMoves, &newBoard[*outAmountOfMoves], x, y, x - 1, y - 1, colour);

    if (!IsColourInCheck(board, colour))
    {
        // Castling king-side
        if (colour == White)
        {
            if (GetPiece(board, x, y) == WhiteUnmovedKing && GetPiece(board, 7, 0) == WhiteUnmovedRook)
            {
                if (GetPiece(board, x + 1, y) == EmptySquare && GetPiece(board, x + 2, y) == EmptySquare)
                {
                    if (!IsPositionAttackedByColour(board, x + 1, y, Black) && !IsPositionAttackedByColour(board, x + 2, y, Black))
                    {
                        newBoard[*outAmountOfMoves] = *board;
                        MoveKing(&newBoard[*outAmountOfMoves], x, y, x + 2, y, colour);
                        MoveRook(&newBoard[*outAmountOfMoves], 7, 0, 5, 0, WhiteRook);
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
                        newBoard[*outAmountOfMoves] = *board;
                        MoveKing(&newBoard[*outAmountOfMoves], x, y, x + 2, y, colour);
                        MoveRook(&newBoard[*outAmountOfMoves], 7, 7, 5, 7, BlackRook);
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
                    if (!IsPositionAttackedByColour(board, x - 1, y, Black) && !IsPositionAttackedByColour(board, x - 2, y, Black))
                    {
                        newBoard[*outAmountOfMoves] = *board;
                        MoveKing(&newBoard[*outAmountOfMoves], x, y, x - 2, y, colour);
                        MoveRook(&newBoard[*outAmountOfMoves], 0, 0, 3, 0, WhiteRook);
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
                        newBoard[*outAmountOfMoves] = *board;
                        MoveKing(&newBoard[*outAmountOfMoves], x, y, x - 2, y, colour);
                        MoveRook(&newBoard[*outAmountOfMoves], 0, 7, 3, 7, BlackRook);
                        ++(*outAmountOfMoves);
                    }
                }
            }
        }
    }
}
    
    // Currently always promotes to a queen
void GenerateLegalMovesForPawn(const Board* const board, Board* const newBoard, int* const outAmountOfMoves, const int x, const int y, const Colour colour, const Colour oppositeColour)
{
    // En Passant position
    int enPassantX, enPassantY;
    if (board->enPassantIndex != -1)
    {
        IndexToPosition(board->enPassantIndex, &enPassantX, &enPassantY);
    }

    int forwardDirection = colour == White ? 1 : -1;
    int promotionLine = colour == White ? 6 : 1;
    Piece pawnPiece = colour == White ? WhitePawn : BlackPawn;
    Piece unmovedPawnPiece = colour == White ? WhiteUnmovedPawn : BlackUnmovedPawn;
    Piece promotionPiece = colour == White ? WhiteQueen : BlackQueen;
    // Moves
    // Forward 1
    if (GetPiece(board, x, y + forwardDirection) == EmptySquare)
    {
        newBoard[*outAmountOfMoves] = *board;
        // promotion
        if (y == promotionLine)
        {
            MovePawn(&newBoard[*outAmountOfMoves], x, y, x, y + forwardDirection, promotionPiece);
        } 
        else
        {
            MovePawn(&newBoard[*outAmountOfMoves], x, y, x, y + forwardDirection, pawnPiece);
        }

        if (!IsColourInCheck(&newBoard[*outAmountOfMoves], colour))
        {
            ++(*outAmountOfMoves);
        }
    }

    // Forward 2
    if (GetPiece(board, x, y) == unmovedPawnPiece)
    {
        if (GetPiece(board, x, y + forwardDirection) == EmptySquare && GetPiece(board, x, y + 2 * forwardDirection) == EmptySquare)
        {
            newBoard[*outAmountOfMoves] = *board;
            MovePawn(&newBoard[*outAmountOfMoves], x, y, x, y + 2 * forwardDirection, pawnPiece);
            if (!IsColourInCheck(&newBoard[*outAmountOfMoves], colour))
            {
                ++(*outAmountOfMoves);
            }
        }
    }

    // Captures
    if (GetColour(GetPiece(board, x + 1, y + forwardDirection)) == oppositeColour)
    {
        newBoard[*outAmountOfMoves] = *board;
        MovePawn(&newBoard[*outAmountOfMoves], x, y, x + 1, y + forwardDirection, pawnPiece);
        if (!IsColourInCheck(&newBoard[*outAmountOfMoves], colour))
        {
            ++(*outAmountOfMoves);
        }
    }
    if (GetColour(GetPiece(board, x - 1, y + forwardDirection)) == oppositeColour)
    {
        newBoard[*outAmountOfMoves] = *board;
        MovePawn(&newBoard[*outAmountOfMoves], x, y, x - 1, y + forwardDirection, pawnPiece);
        if (!IsColourInCheck(&newBoard[*outAmountOfMoves], colour))
        {
            ++(*outAmountOfMoves);
        }
    }

    // En Passant capture
    if (board->enPassantIndex != -1)
    {
        if (enPassantY == y && abs(enPassantX - x) == 1)
        {
            newBoard[*outAmountOfMoves] = *board;
            MovePawn(&newBoard[*outAmountOfMoves], x, y, enPassantX, y + forwardDirection, pawnPiece);
            newBoard[*outAmountOfMoves].board[board->enPassantIndex] = EmptySquare; // This is not handled by MovePawn
            if (!IsColourInCheck(&newBoard[*outAmountOfMoves], colour))
            {
                ++(*outAmountOfMoves);
            }
        }
    }
}

void GenerateLegalMovesForKnight(const Board* const board, Board* const newBoard, int* const outAmountOfMoves, const int x, const int y, const Colour colour, const Colour oppositeColour)
{
    AddNormalMoveIfLegal(board, outAmountOfMoves, &newBoard[*outAmountOfMoves], x, y, x + 1, y + 2, colour);
    AddNormalMoveIfLegal(board, outAmountOfMoves, &newBoard[*outAmountOfMoves], x, y, x - 1, y + 2, colour);
    AddNormalMoveIfLegal(board, outAmountOfMoves, &newBoard[*outAmountOfMoves], x, y, x + 2, y + 1, colour);
    AddNormalMoveIfLegal(board, outAmountOfMoves, &newBoard[*outAmountOfMoves], x, y, x - 2, y + 1, colour);
    AddNormalMoveIfLegal(board, outAmountOfMoves, &newBoard[*outAmountOfMoves], x, y, x + 1, y - 2, colour);
    AddNormalMoveIfLegal(board, outAmountOfMoves, &newBoard[*outAmountOfMoves], x, y, x - 1, y - 2, colour);
    AddNormalMoveIfLegal(board, outAmountOfMoves, &newBoard[*outAmountOfMoves], x, y, x + 2, y - 1, colour);
    AddNormalMoveIfLegal(board, outAmountOfMoves, &newBoard[*outAmountOfMoves], x, y, x - 2, y - 1, colour);
}

void GenerateLegalMovesForRook(const Board* const board, Board* const newBoard, int* const outAmountOfMoves, const int x, const int y, const Colour colour, const Colour oppositeColour)
{
    Piece rookPiece = colour == White ? WhiteRook : BlackRook;
    AddRookMovesAlongDirectionIfLegal(board, outAmountOfMoves, newBoard, x, y, 1, 0, colour, oppositeColour, rookPiece);
    AddRookMovesAlongDirectionIfLegal(board, outAmountOfMoves, newBoard, x, y, -1, 0, colour, oppositeColour, rookPiece);
    AddRookMovesAlongDirectionIfLegal(board, outAmountOfMoves, newBoard, x, y, 0, 1, colour, oppositeColour, rookPiece);
    AddRookMovesAlongDirectionIfLegal(board, outAmountOfMoves, newBoard, x, y, 0, -1, colour, oppositeColour, rookPiece);
}

void GenerateLegalMovesForBishop(const Board* const board, Board* const newBoard, int* const outAmountOfMoves, const int x, const int y, const Colour colour, const Colour oppositeColour)
{
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, newBoard, x, y, 1, 1, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, newBoard, x, y, -1, -1, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, newBoard, x, y, -1, 1, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, newBoard, x, y, 1, -1, colour, oppositeColour);
}

void GenerateLegalMovesForQueen(const Board* const board, Board* const newBoard, int* const outAmountOfMoves, const int x, const int y, const Colour colour, const Colour oppositeColour)
{
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, newBoard, x, y, 1, 0, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, newBoard, x, y, -1, 0, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, newBoard, x, y, 0, 1, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, newBoard, x, y, 0, -1, colour, oppositeColour);

    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, newBoard, x, y, 1, 1, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, newBoard, x, y, -1, -1, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, newBoard, x, y, -1, 1, colour, oppositeColour);
    AddMovesAlongDirectionIfLegal(board, outAmountOfMoves, newBoard, x, y, 1, -1, colour, oppositeColour);
}

#define MAX_LEGAL_MOVES 200
Board* GenerateAllLegalMoves(Board* const board, int* const outAmountOfMoves)
{
    *outAmountOfMoves = 0;
    Colour colourOfTurn = board->isWhiteTurn ? White : Black;
    Colour oppositeColour = colourOfTurn == White ? Black : White;

    // Reset En Passant
    if (board->enPassantIndex != -1)
    {
        int enPassantX, enPassantY;
        IndexToPosition(board->enPassantIndex, &enPassantX, &enPassantY);
        if (GetColour(GetPiece(board, enPassantX, enPassantY)) == colourOfTurn)
        {
            board->enPassantIndex = -1;
        }
    }
    

    Board newMoves[MAX_LEGAL_MOVES];
    for (int i = 0; i < 64; ++i)
    {
        Piece piece = board->board[i];
        Colour colour = GetColour(piece);
        
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
                GenerateLegalMovesForPawn(board, newMoves, outAmountOfMoves, x, y, colourOfTurn, oppositeColour);
                break;
            case WhiteRook:
            case WhiteUnmovedRook:
            case BlackRook:
            case BlackUnmovedRook:
                GenerateLegalMovesForRook(board, newMoves, outAmountOfMoves, x, y, colourOfTurn, oppositeColour);
                break;
            case WhiteKnight:
            case BlackKnight:
                GenerateLegalMovesForKnight(board, newMoves, outAmountOfMoves, x, y, colourOfTurn, oppositeColour);
                break;
            case WhiteBishop:
            case BlackBishop:
                GenerateLegalMovesForBishop(board, newMoves, outAmountOfMoves, x, y, colourOfTurn, oppositeColour);
                break;
            case WhiteQueen:
            case BlackQueen:
                GenerateLegalMovesForQueen(board, newMoves, outAmountOfMoves, x, y, colourOfTurn, oppositeColour);
                break;
            case WhiteKing:
            case WhiteUnmovedKing:
            case BlackKing:
            case BlackUnmovedKing:
                GenerateLegalMovesForKing(board, newMoves, outAmountOfMoves, x, y, colourOfTurn, oppositeColour);
                break;
            default:
                assert(false && "In GenerateAllLegalMoves: All pieces aren't covered in the switch statement");
                break;
            }
        }
    }

    Board* moves = malloc(sizeof(*moves) * (*outAmountOfMoves));
    assert(moves && "In GenerateAllLegalMoves: Failed to allocate memory for moves");
    for (int i = 0; i < *outAmountOfMoves; ++i)
    {
        moves[i] = newMoves[i];
    }

    return moves;
}
