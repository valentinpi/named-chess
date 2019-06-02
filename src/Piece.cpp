#include "Piece.hpp"

Piece::Piece(PieceType type, PieceColor color, BoardPosition position)
{
	this->type = type;
	this->color = color;
	this->position = position;
}
