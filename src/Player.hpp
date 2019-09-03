#pragma once

#include <array>

#include "BoardTypes.hpp"
#include "Piece.hpp"
#include "Utils.hpp"

class Player
{
public:
    Player() = default;
    Player(BoardMetrics board_metrics, std::vector<Piece> *pieces, PieceColor color);
    virtual ~Player() = default;

    Piece *king_piece = nullptr, 
        *hovered_piece = nullptr, 
        *selected_piece = nullptr; 
    std::vector<BoardPosition> highlighted_positions;

    void new_turn();
    virtual void update(const std::vector<SDL_Event> events) = 0;

    inline PieceColor get_color()
    {
        return color;
    }
    inline int32_t get_turn_count()
    {
        return turn_count;
    }
    inline bool is_ai()
    {
        return ai;
    }
    inline bool is_turn_over()
    {
        return turn_over;
    }
    inline bool is_king_threatened()
    {
        return king_threatened;
    }
    inline bool has_lost()
    {   
        return lost;
    }
protected:
    BoardMetrics board_metrics;
    std::vector<Piece> *pieces = nullptr;
    PieceColor color;

    bool ai = false, 
        turn_over = true, 
        king_threatened = false, 
        lost = false;
    uint32_t turn_count = 0;

    void end_turn();

    // Movement
    bool add_position(std::vector<BoardPosition> &positions, PieceColor color, const int32_t col, const int32_t row);
    BoardPositionState check_position(PieceColor color, BoardPosition position);
    Piece* get_piece(BoardPosition position);
    Piece* get_piece(PieceColor color, PieceType type);
    std::vector<BoardPosition> get_legal_positions(Piece &piece, bool movable = true);
    bool is_piece_threatened(const Piece &piece);
    void move_piece(PieceMove move);
};
