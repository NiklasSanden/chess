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
    board.board[1] = WhiteKnight;
    board.board[2] = WhiteBishop;
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

Board InitializeTestBoard()
{
    Board board;
    board.enPassantIndex = -1;
    board.isWhiteTurn = true;
    board.value = 0.0f;
    
    // Do not make this into a loop. You are suppose to change emptysquares to test
    board.board[0] = EmptySquare;
    board.board[1] = EmptySquare;
    board.board[2] = EmptySquare;
    board.board[3] = EmptySquare;
    board.board[4] = EmptySquare;
    board.board[5] = EmptySquare;
    board.board[6] = EmptySquare;
    board.board[7] = EmptySquare;

    board.board[8] = EmptySquare;
    board.board[9] = EmptySquare;
    board.board[10] = EmptySquare;
    board.board[11] = EmptySquare;
    board.board[12] = EmptySquare;
    board.board[13] = EmptySquare;
    board.board[14] = EmptySquare;
    board.board[15] = EmptySquare;

    board.board[16] = EmptySquare;
    board.board[17] = EmptySquare;
    board.board[18] = EmptySquare;
    board.board[19] = EmptySquare;
    board.board[20] = EmptySquare;
    board.board[21] = EmptySquare;
    board.board[22] = EmptySquare;
    board.board[23] = EmptySquare;

    board.board[24] = EmptySquare;
    board.board[25] = EmptySquare;
    board.board[26] = EmptySquare;
    board.board[27] = EmptySquare;
    board.board[28] = EmptySquare;
    board.board[29] = EmptySquare;
    board.board[30] = EmptySquare;
    board.board[31] = EmptySquare;

    board.board[32] = EmptySquare;
    board.board[33] = EmptySquare;
    board.board[34] = EmptySquare;
    board.board[35] = EmptySquare;
    board.board[36] = EmptySquare;
    board.board[37] = EmptySquare;
    board.board[38] = EmptySquare;
    board.board[39] = EmptySquare;

    board.board[40] = EmptySquare;
    board.board[41] = EmptySquare;
    board.board[42] = EmptySquare;
    board.board[43] = EmptySquare;
    board.board[44] = EmptySquare;
    board.board[45] = EmptySquare;
    board.board[46] = EmptySquare;
    board.board[47] = EmptySquare;

    board.board[48] = EmptySquare;
    board.board[49] = EmptySquare;
    board.board[50] = EmptySquare;
    board.board[51] = EmptySquare;
    board.board[52] = EmptySquare;
    board.board[53] = EmptySquare;
    board.board[54] = EmptySquare;
    board.board[55] = EmptySquare;

    board.board[56] = EmptySquare;
    board.board[57] = EmptySquare;
    board.board[58] = EmptySquare;
    board.board[59] = EmptySquare;
    board.board[60] = EmptySquare;
    board.board[61] = EmptySquare;
    board.board[62] = EmptySquare;
    board.board[63] = EmptySquare;

    board.whiteKingIndex = -1;
    board.blackKingIndex = -1;

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
