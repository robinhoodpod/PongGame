#include "graphics.hpp"
#include "logic.hpp"
#include <SDL_mixer.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]){
    Graphics graphics;
    graphics.init();

    graphics.background = graphics.loadTexture("Pong.jpg");
    SDL_Texture* ballTexture = graphics.loadTexture("ball.png");


    Mix_Music *gMusic = Mix_LoadMUS("soundtrack.mp3");
    Mix_PlayMusic(gMusic, -1);

    Mix_Chunk* hitSound = Mix_LoadWAV("hitsound.mp3");



    Paddle left(50);
    Paddle right(SCREEN_WIDTH - 50 - PADDLE_WIDTH);
    Ball ball;
    Score score;

    bool quit = false;
    SDL_Event event;


    while (!quit){

        const Uint8* keystates = SDL_GetKeyboardState(NULL);
           left.speedY = 0;
           right.speedY = 0;

        if (keystates[SDL_SCANCODE_W]) left.speedY = -PADDLE_SPEED;
        if (keystates[SDL_SCANCODE_S]) left.speedY = PADDLE_SPEED;
        if (keystates[SDL_SCANCODE_UP]) right.speedY = -PADDLE_SPEED;
        if (keystates[SDL_SCANCODE_DOWN]) right.speedY = PADDLE_SPEED;

        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) quit = true;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    quit = true;
            }
        }

        left.move();
        right.move();
        ball.move(left, right, score, hitSound);


        SDL_RenderCopy(graphics.renderer, graphics.background, NULL, NULL);


        graphics.renderRect(left.getRect(), {0, 0, 0});
        graphics.renderRect(right.getRect(), {0, 0, 0});
        SDL_Rect dest = ball.getRect();
        SDL_RenderCopy(graphics.renderer, ballTexture, NULL, &dest);


        string leftScore = to_string(score.left);
        string rightScore= to_string(score.right);

        int lw, lh, rw, rh;
           TTF_SizeText(graphics.font, leftScore.c_str(), &lw, &lh);
           TTF_SizeText(graphics.font, rightScore.c_str(), &rw, &rh);

        int padding = 100;
        int leftX = padding;
        int rightX = SCREEN_WIDTH - padding - rw;
        int y = 50;

        graphics.renderText(leftScore.c_str(), leftX, y, {0, 0, 0});
        graphics.renderText(rightScore.c_str(), rightX, y, {0, 0, 0});
        graphics.presentScene();

        SDL_Delay(12);
    }

    SDL_DestroyTexture(ballTexture);
    graphics.quit();
    return 0;
}
