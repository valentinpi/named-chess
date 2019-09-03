#pragma once

#include <vector>

#include <SDL2/SDL.h>

#include "Piece.hpp"

class Board
{
public:
    Board();
    ~Board();
private:
    SDL_Texture *board_texture = nullptr;
    std::vector<Piece> pieces;
};
