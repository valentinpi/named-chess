#include "HumanPlayer.hpp"

HumanPlayer::HumanPlayer(BoardMetrics board_metrics, std::vector<Piece> *pieces, PieceColor color)
{
    this->board_metrics = board_metrics;
    this->pieces = pieces;
    this->color = color;
    
    king_piece = get_piece(color, King);

    ai = false;
}

void HumanPlayer::update(const std::vector<SDL_Event> events)
{
    // Progress events
    bool mouse_clicked = false;
    for (auto event : events)
    {
        switch (event.type)
        {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    mouse_clicked = true;
                break;
            default:
                break;
        }
    }
    
    // Get mouse position and its position in the field
    int mouse_x = 0, mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    BoardPosition mouse_position = {
        (mouse_x - board_metrics.x_offset) / board_metrics.tile_width,
        (mouse_y - board_metrics.y_offset) / board_metrics.tile_height
    };
    hovered_piece = get_piece(mouse_position);
    if (hovered_piece != nullptr)
    {
        if (hovered_piece->color != color ||
            !hovered_piece->movable)
            hovered_piece = nullptr;
    }

    if (mouse_clicked)
    {
        if (hovered_piece != nullptr)
        {
            selected_piece = hovered_piece;
            highlighted_positions = get_legal_positions(*selected_piece);
        }
        else
        {
            for (auto &position : highlighted_positions)
            {
                if (mouse_position == position)
                {
                    move_piece(PieceMove(selected_piece, mouse_position));
                    turn_over = true;
                    end_turn();
                    break;
                }
            }

            if (!turn_over)
            {
                selected_piece = nullptr;
                highlighted_positions.clear();
            }
        }
    }
}
