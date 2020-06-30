#ifndef CHESS_DATA
#define CHESS_DATA

// NoColour = 0
typedef enum EColour
{
    NoColour,
    White,
    Black,
} Colour;

// OutsideSquare = 0
typedef enum EPiece
{
    OutsideSquare,
    EmptySquare,
    WhitePawn, WhiteUnmovedPawn, WhiteEnPassantPawn,
    WhiteRook, WhiteUnmovedRook,
    WhiteKnight,
    WhiteBishop,
    WhiteQueen,
    WhiteKing, WhiteUnmovedKing,
    BlackPawn, BlackUnmovedPawn, BlackEnPassantPawn,
    BlackRook, BlackUnmovedRook,
    BlackKnight,
    BlackBishop,
    BlackQueen,
    BlackKing, BlackUnmovedKing,
} Piece;

typedef struct
{
    char board[8][8];
    float value;
    int whiteKing;
    int blackKing;
    bool isWhiteTurn;
} Board;

Piece GetPiece(const Board* const board, const int x, const int y)
{
    if (x < 0 || x > 7 || y < 0 || y > 7) return OutsideSquare;
    return (Piece)board->board[x][y];
}

Colour GetColour(const Piece piece)
{
    if (piece <= EmptySquare) return NoColour;
    else if (piece < BlackPawn) return White;
    else return Black;
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

#endif
