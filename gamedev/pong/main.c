#include <stdio.h>
#include <raylib.h>

const int screenWidth = 800;
const int screenHeight = 525;
const int hudOffset = 25;

typedef struct {
    Rectangle shape;
    int isActive;
    float xSpeed;
    float ySpeed;
    float baseAcceleration;
} Ball;

void moveX(Ball *ball, Rectangle racket1, Rectangle racket2);
void moveY(Ball *ball);
int score(Ball ball, int *score1, int *score2);
int reset(Ball *ball, Rectangle *racket1, Rectangle *racket2);

int main (void){
    Rectangle border = {
        .width = screenWidth - 10,
        .height = screenHeight - 10 - hudOffset,
        .x = 5,
        .y = 5 + hudOffset
    };
    
    Rectangle division = {
        .width = 10,
        .height = screenHeight - 10 - hudOffset,
        .x = screenWidth / 2 - 5,
        .y = 5 + hudOffset,
    };
    
    Rectangle racket1 = {
        .width = 20,
        .height = 80,
        .x = 50,
        .y = screenHeight / 2 - 40 + hudOffset / 2
    };
    
    Rectangle racket2 = {
        .width = 20,
        .height = 80,
        .x = screenWidth - 70,
        .y = screenHeight / 2 - 40 + hudOffset / 2
    };
    
    Ball ball = {
        .shape.width = 20,
        .shape.height = 20,
        .shape.x = screenWidth / 2 - 10,
        .shape.y = screenHeight / 2 - 10 + hudOffset / 2,
        .xSpeed = 0,
        .ySpeed = 0,
        .baseAcceleration = 2.7f,
    };
    
    int score1 = 0;
    int score2 = 0;
    float racketSpeed = 12.0f;
    float timer = .0f;
    char buffer[100];
    
    InitWindow(screenWidth, screenHeight, "pong");
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        // player 1 movement
        if(racket1.y - racketSpeed >= 5 + hudOffset && IsKeyDown(KEY_W)){
            racket1.y -= racketSpeed;
        };
        if(racket1.y + racketSpeed + racket1.height <= screenHeight - 5 && IsKeyDown(KEY_S)){
            racket1.y += racketSpeed;
        };

        // player 2 movement
        if(racket2.y - racketSpeed >= 5 + hudOffset && IsKeyDown(KEY_UP)){
            racket2.y -= racketSpeed;
        };
        if(racket2.y + racketSpeed + racket2.height <= screenHeight - 5 && IsKeyDown(KEY_DOWN)){
            racket2.y += racketSpeed;
        };

        // ball init
        if(IsKeyPressed(KEY_SPACE) && !ball.isActive){
            ball.isActive = 1;
            ball.xSpeed = 2.7f;
            ball.ySpeed = GetRandomValue(-10, 10) / 10 * ball.baseAcceleration;
        };

        // ball movement
        if(ball.isActive){
            moveX(&ball, racket1, racket2);
            moveY(&ball);
        };

        if(score(ball, &score1, &score2)){
            reset(&ball, &racket1, &racket2);
        };

        BeginDrawing();
        ClearBackground(BLACK);
            DrawRectangleLinesEx(border, 10, PINK);
            DrawRectangleRec(division, PINK);
            DrawRectangleRec(racket1, PINK);
            DrawRectangleRec(racket2, PINK);
            DrawRectangleRec(ball.shape, PINK);
            sprintf(buffer, "%d", score1);
            DrawText(buffer, screenWidth / 4 - MeasureText(buffer, 20) / 2, 5, 20, PINK);
            sprintf(buffer, "%d", score2);
            DrawText(buffer, screenWidth / 4 * 3 - MeasureText(buffer, 20) / 2, 5, 20, PINK);
        EndDrawing();
    };
    CloseWindow();
    return 0;
};

void moveX(Ball *ball, Rectangle racket1, Rectangle racket2){
    float hitFactor;

    ball->shape.x += ball->xSpeed;
    if(CheckCollisionRecs(ball->shape, racket1)){
        ball->baseAcceleration *= 1.05f;
        ball->shape.x -= ball->xSpeed;
        ball->xSpeed = ball->baseAcceleration;
        hitFactor = ((ball->shape.y + ball->shape.height / 2) - (racket1.y + racket1.height / 2)) / racket1.height; 
        ball->ySpeed = hitFactor * ball->baseAcceleration;
    };
    if(CheckCollisionRecs(ball->shape, racket2)){
        ball->baseAcceleration *= 1.05f;
        ball->shape.x -= ball->xSpeed;
        ball->xSpeed = -ball->baseAcceleration;
        hitFactor = ((ball->shape.y + ball->shape.height / 2) - (racket2.y + racket2.height / 2)) / racket2.height; 
        ball->ySpeed = hitFactor * ball->baseAcceleration;
    };
};

void moveY(Ball *ball){
    ball->shape.y += ball->ySpeed;
    if(ball->shape.y < 15 + hudOffset){
        ball->ySpeed *= -1;
    };
    if(ball->shape.y > screenHeight - 15 - ball->shape.height){
        ball->ySpeed *= -1;
    };
};

int score(Ball ball, int *score1, int *score2){
    if(ball.shape.x > screenWidth - 15){
        *score1 += 1;
        return 1;
    };
    if(ball.shape.x < 15 - ball.shape.width){
        *score2 += 1;
        return 1;
    };
    return 0;
};

int reset(Ball *ball, Rectangle *racket1, Rectangle *racket2){
    ball->isActive = 0;
    ball->shape.x = screenWidth / 2 - ball->shape.width / 2;
    ball->shape.y = screenHeight / 2 - ball->shape.height / 2;
    ball->baseAcceleration = 2.7f;
    racket1->y = (racket2->y = screenHeight / 2 - racket2->height / 2);
};