#include "Utils.hpp"

void log(const char *text)
{
#if DEBUG
    std::cout << text << std::endl;
#endif
}

SDL_Texture *extract_from_texture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect area)
{
    if (renderer == nullptr || texture == nullptr)
        return nullptr;

    Uint32 format = 0;
    SDL_QueryTexture(texture, &format, nullptr, nullptr, nullptr);

    SDL_Texture *result = SDL_CreateTexture(
        renderer, format, SDL_TEXTUREACCESS_TARGET, area.w, area.h);
    SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND);

    SDL_Texture *previous_target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, result);

    SDL_RenderCopy(renderer, texture, &area, nullptr);
    SDL_RenderPresent(renderer);

    SDL_SetRenderTarget(renderer, previous_target);
    
    return result;
}

SDL_Texture *load_texture(SDL_Renderer *renderer, const char *file_path)
{
    if (renderer == nullptr)
        return nullptr;

    SDL_Texture *texture = IMG_LoadTexture(renderer, file_path);

    if (texture == nullptr)
    {
        std::stringstream message;
        message << "Failed to load " << file_path;
        log_sdl_error("SDL_image", message.str().c_str(), IMG_GetError());
        return nullptr;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    return texture;
}

void log_sdl_error(const char *library, const char *text, const char *error)
{
#if DEBUG
    std::cout 
        << library << ": " << text << "\n"
        << library << "-Error: " << error;
#endif
}

void render_rect(SDL_Renderer *renderer,SDL_Point rect_position, int width, int height, SDL_Color color, 
    bool fill)
{
    if (renderer == nullptr)
        return;

    Uint8 prev_r = 0, prev_g = 0, prev_b = 0, prev_a = 0;
    SDL_GetRenderDrawColor(renderer, &prev_r, &prev_g, &prev_b, &prev_a);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    SDL_Rect rect = 
    {
        rect_position.x, rect_position.y, width, height
    };

    if (fill)
        SDL_RenderFillRect(renderer, &rect);
    else
        SDL_RenderDrawRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, prev_r, prev_g, prev_b, prev_a);
}

void render_texture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Point position)
{
    if (renderer == nullptr || texture == nullptr)
        return;

    int width = 0, height = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

    SDL_Rect dstrect = { position.x, position.y, width, height };

    SDL_RenderCopy(renderer, texture, nullptr, &dstrect);
}

void render_texture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect area)
{
    if (renderer == nullptr || texture == nullptr)
        return;

    SDL_RenderCopy(renderer, texture, nullptr, &area);
}
