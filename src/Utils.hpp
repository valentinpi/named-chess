#pragma once

#define DEBUG 1

#include <iostream>
#include <sstream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void log(const char *text);

// SDL helper functions
SDL_Texture *extract_from_texture(
    SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect area);
SDL_Texture *load_texture(SDL_Renderer *renderer, const char *file_path);
void log_sdl_error(const char *library, const char *text, const char *error);

void render_rect(SDL_Renderer *renderer, SDL_Point rect_position, int width, int height, SDL_Color color, 
    bool fill = true);
void render_texture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Point position);
void render_texture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect area);
