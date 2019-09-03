#include "Game.hpp"

Game::Game(int window_width, int window_height)
{
    this->window_width = window_width;
    this->window_height = window_height;

    board_metrics.x_offset = window_width / 18;
    board_metrics.y_offset = window_height / 18;
    board_metrics.tile_width = window_width / 9;
    board_metrics.tile_height = window_height / 9;

    // Initialization of libraries
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0)
    {
        log_sdl_error("SDL", "Initialization failed!", SDL_GetError());
        return;
    }
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0)
    {
        log_sdl_error("SDL_image", "Initialization failed!", IMG_GetError());
        return;
    }
    if (TTF_Init() != 0)
    {
        log_sdl_error("SDL_ttf", "Initialization failed!", TTF_GetError());
        return;
    }
    if (SDLNet_Init() != 0)
    {
        log_sdl_error("SDL_net", "Initialization failed!", SDLNet_GetError());
        return;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    window = SDL_CreateWindow("Named Chess",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height,
        0);
    if (window == nullptr)
    {
        log_sdl_error("SDL", "Window creation failed!", SDL_GetError());
        return;
    }

    window_icon = IMG_Load("res/game.ico");
    if (window_icon == nullptr)
        log_sdl_error("SDL_image", "Failed to load icon!", IMG_GetError());
    SDL_SetWindowIcon(window, window_icon);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        log_sdl_error("SDL", "Renderer creation failed!", IMG_GetError());
        return;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Background texture
    board_texture = load_texture(renderer, "res/board.jpg");

    // Parse piece textures from sheet
    {
        SDL_Texture *sheet_texture = load_texture(renderer, "res/pieces.png");
        int sheet_texture_width = 0, sheet_texture_height = 0, sheet_cols = 6, sheet_rows = 2;
        SDL_QueryTexture(sheet_texture, nullptr, nullptr, &sheet_texture_width, &sheet_texture_height);
    
        for (int y = 0; y < sheet_texture_height; 
            y += sheet_texture_height / sheet_rows)
        {
            for (int x = 0; x < sheet_texture_width; 
                x += sheet_texture_width / sheet_cols)
            {
                SDL_Texture *piece_texture = extract_from_texture(renderer, sheet_texture, 
                    { x, y, sheet_texture_width / sheet_cols, sheet_texture_height / sheet_rows });
    
                piece_textures.push_back(piece_texture);
            }
        }
    
        SDL_DestroyTexture(sheet_texture);
    }

    // Create pieces
    std::string map_str = "\
BRNLQKLNR\n\
BPPPPPPPP\n\
 00000000\n\
 00000000\n\
 00000000\n\
 00000000\n\
WPPPPPPPP\n\
WRNLQKLNR";
    int32_t cur_col = 0,  
        cur_row = 0;
    PieceColor color;
    PieceType type;

    for (std::size_t i = 0; i < map_str.size(); i++)
    {
        char cur_char = map_str[i];

        switch (cur_char)
        {
            case 'W': color = White; continue;
            case 'B': color = Black; continue;
            case 'K': type = King;   break;
            case 'Q': type = Queen;  break;
            case 'R': type = Rook;   break;
            case 'L': type = Bishop; break;
            case 'N': type = Knight; break;
            case 'P': type = Pawn;   break;
            case '\n': 
                cur_col = 0; 
                cur_row++;
                continue;
            default:
                cur_col++;
                continue;
        }

        BoardPosition position = { cur_col, cur_row };
        pieces.emplace_back(type, color, position);
        cur_col++;
    }

    players[0] = new HumanPlayer(board_metrics, &pieces, White);
    players[1] = new AIPlayer(board_metrics, &pieces, Black);

    players[0]->new_turn();
    std::cout
        << "Player " << cur_player + 1 << ": Turn " 
        << players[cur_player]->get_turn_count() << std::endl;

    initialized = true;
}

Game::~Game()
{
    for (auto &player : players)
        delete player;

    SDL_DestroyTexture(board_texture);

    for (auto &i : piece_textures)
        SDL_DestroyTexture(i);

    SDL_FreeSurface(window_icon);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDLNet_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::run()
{
    running = true;

    while(running)
    {
        update();
        render();
    }
}

void Game::update()
{
    SDL_Event event;
    std::vector<SDL_Event> events;
    while(SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            running = false;
        else
            events.push_back(event);
    }
    if (game_over)
        return;

    static bool draw = true;

    // If only Kings are left, it's a Draw
    for (auto &piece : pieces)
    {
        if (piece.destroyed)
            continue;

        if (piece.type != King)
        {
            draw = false;
            break;
        }
    }

    if (draw)
    {
        log("Draw!");

        std::ofstream statistics_file(
            "stats", std::ios::app | std::ios::binary);
        
        // statistics_file << "Unentschieden";
        statistics_file << "556e656e74736368696564656e";
        statistics_file.close();

        game_over = true;
        return;
    }

    for (auto &player : players)
    {
        // Check for a checkmate
        if (player->has_lost())
        {
            std::string final_text = "";

            /* Confuse people by saying the Human has lost, but actually the 
               AI lost */
            if (player->is_ai())
                // final_text = "Human";
                final_text += "48756d616e";
            else
                // final_text = "AI";
                final_text += "4149";

            final_text += char(std::rand() % 0xFF);
            if (player->is_king_threatened())
            {
                log("Checkmated!");
                //  final_text += ": Schachmatt gesetzt\n";
                final_text += "3a205363686163686d6174742067657365747a74";
            }
            else
            {
                log("Patt!");
                //  final_text += ": Patt gesetzt\n";
                final_text += "3a20506174742067657365747a74";
            }

            // Output information
            std::ofstream statistics_file(
                "stats", std::ios::app | std::ios::binary);
            statistics_file << final_text;
            statistics_file.close();

            game_over = true;
            return;
        }
    }

    // Turn mechanism
    if (players[cur_player]->is_turn_over())
    {
        cur_player++;
        if (cur_player > (uint32_t) players.size() - 1)
            cur_player = 0;

        players[cur_player]->new_turn();
        std::cout
            << "Player " << cur_player + 1 << ": Turn " 
            << players[cur_player]->get_turn_count() << std::endl;
    }

    players[cur_player]->update(events);
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_texture(
        renderer, board_texture, 
        { 0, 0, window_width, window_height });

    //
    Piece *hovered_piece = players[cur_player]->hovered_piece;
    if (hovered_piece != nullptr)
    {
        SDL_Point position = {
            board_metrics.x_offset + 
                hovered_piece->position.col * board_metrics.tile_width,
            board_metrics.y_offset + 
                hovered_piece->position.row * board_metrics.tile_height
        };
        render_rect(
            renderer, position,
            board_metrics.tile_width, board_metrics.tile_height, 
            hovering_color);
    }

    // Highlight King if threatened
    if (players[cur_player]->is_king_threatened())
    {
        Piece *king_piece = players[cur_player]->king_piece;

        SDL_Point position = {
            board_metrics.x_offset + 
                king_piece->position.col * board_metrics.tile_width,
            board_metrics.y_offset + 
                king_piece->position.row * board_metrics.tile_height
        };
        render_rect(
            renderer, position, 
            board_metrics.tile_width, board_metrics.tile_height, 
            endangering_color);
    }

    // Render pieces
    for (auto &piece : pieces)
    {
        if (piece.destroyed)
            continue;

        SDL_Texture *texture = nullptr;
    
        if (piece.color == Black)
        {
            switch(piece.type)
            {
                case King:
                    texture = piece_textures[5];
                    break;
                case Queen:
                    texture = piece_textures[4];
                    break;
                case Rook:
                    texture = piece_textures[3];
                    break;
                case Bishop:
                    texture = piece_textures[2];
                    break;
                case Knight:
                    texture = piece_textures[1];
                    break;
                case Pawn:
                    texture = piece_textures[0];
                    break;
                default:
                    break;
            }
        }
        else if (piece.color == White)
        {
            switch(piece.type)
            {
                case King:
                    texture = piece_textures[11];
                    break;
                case Queen:
                    texture = piece_textures[10];
                    break;
                case Rook:
                    texture = piece_textures[9];
                    break;
                case Bishop:
                    texture = piece_textures[8];
                    break;
                case Knight:
                    texture = piece_textures[7];
                    break;
                case Pawn:
                    texture = piece_textures[6];
                    break;
                default:
                    break;
            }
        }
    
        render_texture(
            renderer, texture, 
            { board_metrics.x_offset + 
                piece.position.col * board_metrics.tile_width,
              board_metrics.y_offset + 
                piece.position.row * board_metrics.tile_height,
              board_metrics.tile_width, board_metrics.tile_height });
    }

    // Highlight available positions
    std::vector<BoardPosition> *highlighted_positions = 
        &players[cur_player]->highlighted_positions;
    for (auto &highlighted_position : *highlighted_positions)
    {
        SDL_Point position = {
            board_metrics.x_offset + 
                highlighted_position.col * board_metrics.tile_width
            + board_metrics.tile_width * 3 / 8,
            board_metrics.y_offset + 
                highlighted_position.row * board_metrics.tile_height
            + board_metrics.tile_height * 3 / 8
        };
        render_rect(renderer, position, 
            board_metrics.tile_width / 4, board_metrics.tile_height / 4,
            highlighting_color);
    }

    SDL_RenderPresent(renderer);
}
