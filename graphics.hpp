#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "defs.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

struct Graphics{

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* background = nullptr;
    TTF_Font* font;

    void logErrorAndExit(const char* msg, const char* error){
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,"%s: %s", msg, error);
        SDL_Quit();
    }


    void init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
            logErrorAndExit("SDL_Init", SDL_GetError());

        if (TTF_Init() != 0)
            logErrorAndExit("TTF_Init", TTF_GetError());

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);

        if (!window){
            logErrorAndExit("SDL_CreateWindow", SDL_GetError());
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (!renderer){
            logErrorAndExit("SDL_CreateRenderer", SDL_GetError());
        }

        font = TTF_OpenFont("BungeeTint-Regular.ttf", 150);

        if (!font){
            SDL_Log("Failed to load font: %s", TTF_GetError());
            exit(1);
        }

        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
           logErrorAndExit( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        }

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

        SDL_Texture* loadTexture(const char* filename){
        SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
        if (!texture){
            SDL_Log("Load texture %s error: %s", filename, IMG_GetError());
            exit(1);
        }
        return texture;
    }

    void renderTexture(SDL_Texture* texture, int x, int y){
        SDL_Rect dest;
        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }

    void renderText(const char* text, int x, int y, SDL_Color color){
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
        if (!surface){
            SDL_Log("Failed to create text surface: %s", TTF_GetError());
            return;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture){
            SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
            SDL_FreeSurface(surface);
            return;
        }

        SDL_Rect dst = { x, y, surface->w, surface->h };
        SDL_FreeSurface(surface);
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        SDL_DestroyTexture(texture);
    }



    void presentScene(){
        SDL_RenderPresent(renderer);
    }

    void renderRect(SDL_Rect rect, SDL_Color color){
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    }

    void quit(){
        if (background) SDL_DestroyTexture(background);
        TTF_Quit();
        Mix_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

    }
};

#endif

