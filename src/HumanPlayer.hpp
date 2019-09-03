#pragma once

#include "Player.hpp"

class HumanPlayer : public Player
{
public:
    HumanPlayer(BoardMetrics board_metrics, std::vector<Piece> *pieces, PieceColor color);

    void update(const std::vector<SDL_Event> events) override;
};
