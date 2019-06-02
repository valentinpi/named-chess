#pragma once

#include <cstdint>

enum BoardPositionState
{
    Empty,
    Enemy_Piece,
    Inaccessable
};

struct BoardMetrics
{
    int32_t 
        x_offset = 0, 
        y_offset = 0, 
        tile_width = 0, 
        tile_height = 0;
};

struct BoardPosition
{
    int32_t col = 0, row = 0;

    bool operator!=(const BoardPosition& pos) const;
    bool operator==(const BoardPosition& pos) const;
};
