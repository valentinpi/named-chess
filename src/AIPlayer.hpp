#pragma once

#include <algorithm>
#include <chrono>

#include "Player.hpp"

class AIPlayer : public Player
{
public:
    AIPlayer(BoardMetrics board_metrics, std::vector<Piece> *pieces, PieceColor color);
    ~AIPlayer() override;

    void update(const std::vector<SDL_Event> events) override;
private:
    std::chrono::steady_clock::time_point t1;
    bool delay_finished = true;
    bool planning_phase = true;
    PieceMove planned_move;
    // Minimax
    PieceColor maximizer_color, 
        minimizer_color;
    // Benchmarking
    int32_t bench_nodes_checked = 0, 
        bench_nodes_cut = 0;
    std::chrono::steady_clock::time_point bench_t1, 
        bench_t2;
    float average_planning_time = 0.0f;

    /* 
        Minimax algorithm
        The depth will determine the performance and 
        complexity
    */
    // NOTE: Reduced to one to debug the other components of the game faster
    uint32_t planning_depth = 2;
    /*
        2 - plan_moves_max
        1 - plan_moves_min
        0 - evaluate_board

        Even => Last function call to maximizer
        Uneven => Last function call to minimizer
    */
    // Recursion
    int32_t plan_moves_max(
        uint32_t planning_depth, 
        int32_t alpha = std::numeric_limits<int32_t>::min(), 
        int32_t beta = std::numeric_limits<int32_t>::max());
    int32_t plan_moves_min(
        uint32_t planning_depth, 
        int32_t alpha = std::numeric_limits<int32_t>::max(), 
        int32_t beta = std::numeric_limits<int32_t>::min());
    // Helper
    int32_t evaluate_board();
};
