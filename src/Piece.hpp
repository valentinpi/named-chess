#pragma once

#include <utility>

#include "BoardTypes.hpp"

enum PieceColor
{
    Black,
    White
};

// Assigned values to types for AI
enum PieceType
{
    // Maximum possible number of moves * 25
 // Type    = value, // Type in German
    King    = 10000, // König
    Queen   = 775,   // Dame
    Rook    = 350,   // Turm
    Bishop  = 325,   // Läufer
    Knight  = 200,   // Springer
    Pawn    = 50     // Bauer
};

struct Piece
{
    PieceColor color;
    PieceType type;
    BoardPosition position;
    bool destroyed = false,
         movable = false,
         moved = false;
    
    Piece(PieceType type, PieceColor color, BoardPosition position);
};

using PieceMove = std::pair<Piece*, BoardPosition>;
