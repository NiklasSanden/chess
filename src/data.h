#ifndef CHESS_DATA
#define CHESS_DATA

// NoColour = 0
typedef enum EColour
{
    NoColour,
    White,
    Black,
} Colour;

// OutsidePiece = 0
typedef enum EPiece
{
    OutsidePiece,
    EmptyPiece,
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
    int whiteKing;
    int blackKing;
} Board;

Piece GetPiece(const Board* const board, const int x, const int y)
{
    if (x < 0 || x > 7 || y < 0 || y > 7) return OutsidePiece;
    return (Piece)board->board[x][y];
}

Colour GetColour(const Piece piece)
{
    if (piece <= EmptyPiece) return NoColour;
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
