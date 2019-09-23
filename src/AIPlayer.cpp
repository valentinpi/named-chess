#include "AIPlayer.hpp"

AIPlayer::AIPlayer(BoardMetrics board_metrics, std::vector<Piece> *pieces, PieceColor color)
{
    this->board_metrics = board_metrics;
    this->color = color;
    this->pieces = pieces;

    king_piece = get_piece(color, King);

    ai = true;

    planned_move.first = nullptr;

    maximizer_color = color;

    if (maximizer_color == White)
        minimizer_color = Black;
    else
        minimizer_color = White;
}

AIPlayer::~AIPlayer()
{
    average_planning_time /= turn_count;
    std::cout << "Average planning time: " << average_planning_time << " seconds" << std::endl;
}

void AIPlayer::update(const std::vector<SDL_Event> events)
{
    (void) events;

    if (!delay_finished)
    {
        auto t2 = std::chrono::steady_clock::now();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() > 250)
            delay_finished = true;
        else
            return;
    }

    if (planning_phase)
    {
        bench_t1 = std::chrono::steady_clock::now();

        if (turn_count == 1)
            std::cout << "planning_depth: " << planning_depth << std::endl; 

        plan_moves_max(planning_depth);

        std::cout << bench_nodes_checked << " nodes checked" << "\n" 
            << bench_nodes_cut << " nodes cut" << std::endl;
        bench_nodes_checked = 0;
        bench_nodes_cut = 0;

        bench_t2 = std::chrono::steady_clock::now();
        float seconds_passed = 
            std::chrono::duration_cast<std::chrono::milliseconds>(bench_t2 - bench_t1).count() / 1000.0f;
        std::cout << seconds_passed << " seconds planning time" << std::endl;
        average_planning_time += seconds_passed;

        selected_piece = planned_move.first;
        highlighted_positions.push_back(planned_move.second);

        t1 = std::chrono::steady_clock::now();
        delay_finished = false;
        planning_phase = false;
    }
    else
    {
        move_piece(planned_move);
        planning_phase = true;
        end_turn();
    }
}

int32_t AIPlayer::plan_moves_max(
    uint32_t planning_depth,
    int32_t alpha,
    int32_t beta)
{
    bench_nodes_checked++;
    
    // Getting moves and presorting them
    std::vector<std::pair<PieceMove, int32_t>> moves;
    for (auto &piece : *pieces)
    {
        if (piece.destroyed || piece.color != maximizer_color)
            continue;

        std::vector<BoardPosition> possible_positions = get_legal_positions(piece);

        for (auto &possible_position : possible_positions)
        {
            std::pair<PieceMove, int32_t> move;
            move.first.first = &piece;
            move.first.second = possible_position;
            move.second = 0;

            Piece *destroyed_piece = get_piece(possible_position);
            if (destroyed_piece != nullptr)
                move.second = destroyed_piece->type;

            moves.push_back(move);
        }
    }

    if (moves.empty())
        return evaluate_board();

    std::sort(
        moves.begin(), moves.end(),
        [] (const std::pair<PieceMove, int32_t>& first, const std::pair<PieceMove, int32_t>& second) {
            return first.second > second.second;
        });

    // Evaluation
    int32_t highest_value = alpha;
    Piece *destroyed_piece = nullptr;
    for (auto &move : moves)
    {
        int32_t move_value = 0;

        Piece *piece = move.first.first;
        PieceType prev_type = piece->type;
        bool prev_moved = piece->moved;
        BoardPosition prev_position = piece->position;

        destroyed_piece = get_piece(move.first.second);
        if (destroyed_piece != nullptr)
            destroyed_piece->destroyed = true;
        if (piece->type == Pawn)
        {
            if (piece->color == White && move.first.second.row == 0)
                piece->type = Queen;
            else if (piece->color == Black && move.first.second.row == 7)
                piece->type = Queen;
        }
        piece->position = move.first.second;
        piece->moved = true;

        if (planning_depth > 0)
            move_value = plan_moves_min(planning_depth - 1, highest_value, beta);
        else
            move_value = evaluate_board();

        piece->moved = prev_moved;
        piece->position = prev_position;
        piece->type = prev_type;
        if (destroyed_piece != nullptr)
        {
            destroyed_piece->destroyed = false;
            destroyed_piece = nullptr;
        }
      
        if (move_value > highest_value)
        {
            highest_value = move_value;

            if (move_value >= beta)
            {
                bench_nodes_cut++;
                break;
            }

            if (planning_depth == this->planning_depth)
                planned_move = move.first;
        }
    }

    return highest_value;
}

int32_t AIPlayer::plan_moves_min(
    uint32_t planning_depth,
    int32_t alpha,
    int32_t beta)
{
    bench_nodes_checked++;

    // Getting moves and presorting them
    std::vector<std::pair<PieceMove, int32_t>> moves;
    for (auto &piece : *pieces)
    {
        if (piece.destroyed || piece.color != minimizer_color)
            continue;

        std::vector<BoardPosition> possible_positions = get_legal_positions(piece);

        for (auto &possible_position : possible_positions)
        {
            std::pair<PieceMove, int32_t> move;
            move.first.first = &piece;
            move.first.second = possible_position;
            move.second = 0;

            Piece *destroyed_piece = get_piece(possible_position);
            if (destroyed_piece != nullptr)
                move.second = -destroyed_piece->type;

            moves.push_back(move);
        }
    }

    if (moves.empty())
        return evaluate_board();

    std::sort(
        moves.begin(), moves.end(),
        [] (const std::pair<PieceMove, int32_t>& first, 
            const std::pair<PieceMove, int32_t>& second) {
            return first.second < second.second;
        });

    int32_t lowest_value = beta;
    Piece *destroyed_piece = nullptr;
    for (auto &move : moves)
    {
        int32_t move_value = 0;

        Piece *piece = move.first.first;
        PieceType prev_type = piece->type;
        bool prev_moved = piece->moved;
        BoardPosition prev_position = piece->position;

        destroyed_piece = get_piece(move.first.second);
        if (destroyed_piece != nullptr)
            destroyed_piece->destroyed = true;
        if (piece->type == Pawn)
        {
            if (piece->color == White && move.first.second.row == 0)
                piece->type = Queen;
            else if (piece->color == Black && move.first.second.row == 7)
                piece->type = Queen;
        }
        piece->position = move.first.second;
        piece->moved = true;

        if (planning_depth > 0)
            move_value = plan_moves_max(planning_depth - 1, alpha, lowest_value);
        else
            move_value = -evaluate_board();

        piece->moved = prev_moved;
        piece->position = prev_position;
        piece->type = prev_type;
        if (destroyed_piece != nullptr)
        {
            destroyed_piece->destroyed = false;
            destroyed_piece = nullptr;
        }

        if (move_value < lowest_value)
        {
            lowest_value = move_value;

            if (move_value <= alpha)
            {
                bench_nodes_cut++;
                break;
            }
        }
    }

    return lowest_value;
}

int32_t AIPlayer::evaluate_board()
{
    int32_t result = 0;

    bool lost = true, won = true;
    for (auto &piece : *pieces)
    {
        if (piece.destroyed)
            continue;

        if (!get_legal_positions(piece).empty())
        {
            if (piece.color == color)
                lost = false;
            else
                won = false;
        }

        if (!won && !lost)
            break;
    }

    if (lost)
        return std::numeric_limits<int32_t>::min() + 1;
    else if (won)
        return std::numeric_limits<int32_t>::max() - 1;
    
    for (auto &piece : *pieces)
    {
        if (piece.destroyed || piece.type == King)
            continue;

        bool threatened = is_piece_threatened(piece);

        if (piece.color == color)
        {
            if (threatened)
                result -= piece.type;
            else
                result += piece.type;
        }
        else if (threatened)
            result += piece.type;
    }

    return result;
}
