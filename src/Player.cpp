#include "Player.hpp"

Player::Player(BoardMetrics board_metrics, std::vector<Piece> *pieces, PieceColor color)
{
    this->board_metrics = board_metrics;
    this->pieces = pieces;
    this->color = color;

    king_piece = get_piece(color, King);
}

void Player::new_turn()
{
    turn_over = false;
    turn_count++;

    king_threatened = is_piece_threatened(*king_piece);

    bool no_movable_pieces = true;
    for (auto &piece : *pieces)
    {
        if (piece.destroyed || piece.color != color)
            continue;

        piece.movable = !get_legal_positions(piece).empty();

        if (piece.movable)
            no_movable_pieces = false;
    }

    if (no_movable_pieces)
    {
        lost = true;

        hovered_piece = nullptr;
        selected_piece = nullptr;
        highlighted_positions.clear();
    }
}

void Player::end_turn()
{
    turn_over = true;

    hovered_piece = nullptr;
    selected_piece = nullptr;
    highlighted_positions.clear();
}

bool Player::add_position(std::vector<BoardPosition> &positions, PieceColor color, const int32_t col, const int32_t row)
{
    bool empty = false;
    BoardPosition new_position = { col, row };
    BoardPositionState new_position_state = check_position(color, new_position);

    switch (new_position_state)
    {
        case Empty:
            positions.push_back(new_position);
            empty = true;
            break;
        case Enemy_Piece:
            positions.push_back(new_position);
            break;
        case Inaccessable:
            break;
        default:
            break;
    }

    return empty;
}

BoardPositionState Player::check_position(PieceColor color, BoardPosition position)
{
    // Bounds check
    if (position.col < 0 || position.col > 7 || 
        position.row < 0 || position.row > 7)
        return Inaccessable;

    Piece *_piece = get_piece(position);
    if (_piece != nullptr)
    {
        if (_piece->color == color)
            return Inaccessable;
        else
            return Enemy_Piece;
    }

    return Empty;
}

Piece* Player::get_piece(BoardPosition position)
{
    for (auto &piece : *pieces)
    {
        if (piece.destroyed)
            continue;

        if (piece.position == position)
            return &piece;
    }

    return nullptr;
}

Piece* Player::get_piece(PieceColor color, PieceType type)
{
    for (auto &piece : *pieces)
    {
        if (piece.destroyed)
            continue;

        if (piece.type == type && piece.color == color)
            return &piece;
    }

    return nullptr;
}

std::vector<BoardPosition> Player::get_legal_positions(Piece &piece, bool movable)
{
    std::vector<BoardPosition> results;

    int32_t pawn_forward_move_direction = 0;
    if (piece.color == White)
        pawn_forward_move_direction = -1;
    else
        pawn_forward_move_direction = 1;

    // Neighbors
    if (piece.type == King || 
        piece.type == Queen)
    {
        std::array<BoardPosition, 8> neighboring_positions = {{
            { piece.position.col - 1, piece.position.row - 1 },
            { piece.position.col - 1, piece.position.row     },
            { piece.position.col - 1, piece.position.row + 1 },
            { piece.position.col    , piece.position.row - 1 },
            { piece.position.col    , piece.position.row + 1 },
            { piece.position.col + 1, piece.position.row - 1 },
            { piece.position.col + 1, piece.position.row     },
            { piece.position.col + 1, piece.position.row + 1 }
        }};

        for (auto &neighboring_position : neighboring_positions)
            if (check_position(piece.color, neighboring_position) != Inaccessable)
                results.push_back(neighboring_position);
    }
    // Horizontal and vertical
    if (piece.type == Queen || 
        piece.type == Rook)
    {
        for (int32_t col = piece.position.col - 1; col >= 0; col--)
            if (!add_position(results, piece.color, col, piece.position.row))
                break;
        for (int32_t col = piece.position.col + 1; col <= 7; col++)
            if (!add_position(results, piece.color, col, piece.position.row))
                break;
        for (int32_t row = piece.position.row - 1; row >= 0; row--)
            if (!add_position(results, piece.color, piece.position.col, row))
                break;
        for (int32_t row = piece.position.row + 1; row <= 7; row++)
            if (!add_position(results, piece.color, piece.position.col, row))
                break;
    }
    // Diagonal
    if (piece.type == Queen || 
        piece.type == Bishop)
    {
        for (int32_t col = piece.position.col - 1; col >= 0; col--)
            if (!add_position(results, piece.color, col, piece.position.row - (piece.position.col - col)))
                break;
        for (int32_t col = piece.position.col - 1; col >= 0; col--)
            if (!add_position(results, piece.color, col, piece.position.row + (piece.position.col - col)))
                break;
        for (int32_t col = piece.position.col + 1; col <= 7; col++)
            if (!add_position(results, piece.color, col, piece.position.row + (piece.position.col - col)))
                break;
        for (int32_t col = piece.position.col + 1; col <= 7; col++)
            if (!add_position(results, piece.color, col, piece.position.row + (piece.position.col - col)))
                break;
    }
    // Knight and Pawn
    if (piece.type == Knight)
    {
        std::array<BoardPosition, 8> neighboring_positions = {{
            { piece.position.col - 2, piece.position.row - 1 },
            { piece.position.col - 1, piece.position.row - 2 },
            { piece.position.col + 1, piece.position.row - 2 },
            { piece.position.col + 2, piece.position.row - 1 },
            { piece.position.col - 2, piece.position.row + 1 },
            { piece.position.col - 1, piece.position.row + 2 },
            { piece.position.col + 1, piece.position.row + 2 },
            { piece.position.col + 2, piece.position.row + 1 }
        }};

        for (auto &neighboring_position : neighboring_positions)
            if (check_position(piece.color, neighboring_position) != Inaccessable)
                results.push_back(neighboring_position);
    }
    if (piece.type == Pawn)
    {
        std::array<BoardPosition, 4> pawn_positions = {{
            {   piece.position.col    , piece.position.row + pawn_forward_move_direction     },
            {   piece.position.col    , piece.position.row + pawn_forward_move_direction * 2 },
            {   piece.position.col - 1, piece.position.row + pawn_forward_move_direction     },
            {   piece.position.col + 1, piece.position.row + pawn_forward_move_direction     }
        }};

        if (check_position(piece.color, pawn_positions[0]) == Empty)
        {
            results.push_back(pawn_positions[0]);
            
            if (check_position(piece.color, pawn_positions[1]) == Empty && !piece.moved)
                results.push_back(pawn_positions[1]);
        }
        if (check_position(piece.color, pawn_positions[2]) == Enemy_Piece)
            results.push_back(pawn_positions[2]);
        if (check_position(piece.color, pawn_positions[3]) == Enemy_Piece)
            results.push_back(pawn_positions[3]);
    }

    if (movable)
    {
        Piece *_king_piece = get_piece(piece.color, King);

        // Temporarily move to position and check if somebody threatens the King
        BoardPosition prev_position = piece.position;
        for (std::size_t i = 0; i < results.size(); i++)
        {
            Piece *temp_destroyed_piece = get_piece(results[i]);

            if (temp_destroyed_piece != nullptr)
                temp_destroyed_piece->destroyed = true;
            piece.position = results[i];

            if (is_piece_threatened(*_king_piece))
            {
                results.erase(results.begin() + i);
                i--;
            }
            
            piece.position = prev_position;
            if (temp_destroyed_piece != nullptr)
                temp_destroyed_piece->destroyed = false;
        }
    }

    return results;
}

bool Player::is_piece_threatened(const Piece &piece)
{
    for (auto &_piece : *pieces)
    {
        if (_piece.destroyed || _piece.color == piece.color)
            continue;

        std::vector<BoardPosition> threatened_positions = get_legal_positions(_piece, false);

        for (auto &threatened_position : threatened_positions)
            if (threatened_position == piece.position)
                return true;
    }

    // No threats
    return false;
}

void Player::move_piece(PieceMove move)
{
    if (check_position(move.first->color, move.second) == Enemy_Piece)
        get_piece(move.second)->destroyed = true;

    // Players only get (well, most of the time only want) Queens - ADD dialogue for choosing a piece
    if (move.first->type == Pawn)
    {
        if (move.first->color == White && move.second.row == 0)
        {
            log("Pawn reached last tile!");
            move.first->type = Queen;
        }
        else if (move.first->color == Black && move.second.row == 7)
        {
            log("Pawn reached last tile!");
            move.first->type = Queen;
        }
    }

    move.first->position = move.second;
    move.first->moved = true;
}
