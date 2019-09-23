#pragma once

#include <array>
#include <fstream>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "AIPlayer.hpp"
#include "Board.hpp"
#include "BoardTypes.hpp"
#include "HumanPlayer.hpp"
#include "Piece.hpp"
#include "Utils.hpp"

class Game
{
public:
    // The resolution should be a multiple of 18 to avoid graphic bugs
    Game(int window_width, int window_height);
    ~Game();

    inline bool is_initialized()
    {
        return initialized;
    }

    void run();
private:
    // NOTE: Raw pointers are used because SDL is written in C, so the library handles the memory
    SDL_Window *window = nullptr;
    int window_width = 0, 
        window_height = 0;
    SDL_Surface *window_icon = nullptr;
    SDL_Renderer *renderer = nullptr;
    bool initialized = false, 
        running = false;

    // Game logic
    BoardMetrics board_metrics;
    uint32_t cur_player = 0;
    bool game_over = false;
    std::vector<Piece> pieces;
    std::array<Player*, 2> players;

    SDL_Texture *board_texture = nullptr;
    std::vector<SDL_Texture *> piece_textures;
    
    const SDL_Color hovering_color = { 125, 75, 0, 122 };
    const SDL_Color highlighting_color = { 0, 255, 0, 255 };
    const SDL_Color endangering_color = { 255, 0, 0, 255 };

    void update();
    void render();
};
