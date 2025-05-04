#include "defs.hpp"

struct Paddle{
    int x, y;
    int speedY = 0;

    Paddle(int xPos){
        x = xPos;
        y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    }

    void move(){
        y += speedY;
        if (y < 0) y = 0;
        if (y + PADDLE_HEIGHT > SCREEN_HEIGHT)
            y = SCREEN_HEIGHT - PADDLE_HEIGHT;
    }

    SDL_Rect getRect() const{
        return SDL_Rect{ x, y, PADDLE_WIDTH, PADDLE_HEIGHT };
    }
};


struct Score{
    int left = 0;
    int right = 0;
};


struct Ball{
    int x, y;
    int velX = BALL_SPEED;
    int velY = BALL_SPEED;

    Ball() {
        reset();
    }

    void reset(){
        x = SCREEN_WIDTH / 2 - BALL_SIZE / 2;
        y = SCREEN_HEIGHT / 2 - BALL_SIZE / 2;
    }


    void move(const Paddle& left, const Paddle& right,  Score& score, Mix_Chunk* hitSound){
    x += velX;
    y += velY;

    if(y <= 0 || y + BALL_SIZE >= SCREEN_HEIGHT)
        velY = -velY;

    SDL_Rect ballRect = getRect();
    SDL_Rect leftRect = left.getRect();
    SDL_Rect rightRect = right.getRect();

    if (SDL_HasIntersection(&ballRect, &leftRect)){
        velX = BALL_SPEED;
        Mix_PlayChannel(-1, hitSound, 0);
    }

    if (SDL_HasIntersection(&ballRect, &rightRect)){
        velX = -BALL_SPEED;
         Mix_PlayChannel(-1, hitSound, 0);
    }


    if (x < 0){
        score.right++;
        reset();
    }
    if (x + BALL_SIZE > SCREEN_WIDTH){
        score.left++;
        reset();
    }
}

    SDL_Rect getRect() const{
        return SDL_Rect{ x, y, BALL_SIZE, BALL_SIZE };
    }
};


