#include "data.h"

Board InitializeBoard()
{
    Board board;
    board.enPassantIndex = -1;
    board.whiteKingIndex = 4;
    board.blackKingIndex = 60;
    board.isWhiteTurn = true;
    board.value = 0.0f;
    
    board.board[0] = WhiteUnmovedRook;
    board.board[2] = WhiteKnight;
    board.board[1] = WhiteBishop;
    board.board[3] = WhiteQueen;
    board.board[4] = WhiteUnmovedKing;
    board.board[5] = WhiteBishop;
    board.board[6] = WhiteKnight;
    board.board[7] = WhiteUnmovedRook;
    for (int i = 0; i < 8; ++i)
    {
        board.board[i + 8] = WhiteUnmovedPawn;
    }
    for (int i = 0; i < 8 * 4; ++i)
    {
        board.board[i + 16] = EmptySquare;
    }
    for (int i = 0; i < 8; ++i)
    {
        board.board[i + 48] = BlackUnmovedPawn;
    }
    board.board[56] = BlackUnmovedRook;
    board.board[57] = BlackKnight;
    board.board[58] = BlackBishop;
    board.board[59] = BlackQueen;
    board.board[60] = BlackUnmovedKing;
    board.board[61] = BlackBishop;
    board.board[62] = BlackKnight;
    board.board[63] = BlackUnmovedRook;

    return board;
}

int PositionToIndex(const int x, const int y)
{
    return y * 8 + x;
}

void IndexToPosition(const int index, int* const outX, int* const outY)
{
    (*outX) = index % 8;
    (*outY) = index / 8;
}

Piece GetPiece(const Board* const board, const int x, const int y)
{
    if (x < 0 || x > 7 || y < 0 || y > 7) return OutsideSquare;
    return board->board[PositionToIndex(x, y)];
}

void SetPiece(Board* const board, const int x, const int y, const Piece piece)
{
    board->board[PositionToIndex(x, y)] = piece;
}

Colour GetColour(const Piece piece)
{
    if (piece <= EmptySquare) return NoColour;
    else if (piece < BlackPawn) return White;
    else return Black;
}
