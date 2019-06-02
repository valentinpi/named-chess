#include "BoardTypes.hpp"

bool BoardPosition::operator!=(const BoardPosition& pos) const
{
    return (col != pos.col || row != pos.row);
}

bool BoardPosition::operator==(const BoardPosition& pos) const
{
    return (col == pos.col && row == pos.row);
}
