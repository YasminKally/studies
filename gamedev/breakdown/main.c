#include <stdio.h>
#include <raylib.h>

typedef struct {
    Rectangle shape;
    bool state;
} Brick;

typedef struct {
    Rectangle shape;
    float xSpeed;
    float ySpeed;
    bool isActive;
} Ball;

typedef enum {
    game,
    win,
    lose
} GameState;

const int screenWidth = 352;
const int screenHeight = 500;
const int lines = 8;
const int rows = 8;

int collide(Ball* ball, Brick bricks[lines][rows]);
int moveX(Ball* ball, Brick bricks[lines][rows]);
int moveY(Ball* ball, Brick bricks[lines][rows]);
void setBricks(Brick bricks[lines][rows]);
void reset(Rectangle *racket, Ball *ball, Brick bricks[lines][rows]);
void drawAtMiddle(char* text, int yOffset, int fontSize);

int main (void){
    char buffer[100];
    int score = 0;
    float racketSpeed = 4.3f;
    float timer = 0;
    float hitFactor;
    
    Brick bricks[lines][rows];

    Rectangle racket = {
        .width = 50,
        .height = 10,
        .x = screenWidth / 2 - 50 / 2,
        .y = screenHeight - 90
    };

    Ball ball = {
        .shape.height = 10,
        .shape.width = 10,
        .shape.x = racket.x + racket.width / 2 - 5,
        .shape.y = racket.y - 15,
        .xSpeed = 0,
        .ySpeed = 0,
        .isActive = 0
    };

    GameState gameState = game;

    reset(&racket, &ball, bricks);

    InitWindow(screenWidth, screenHeight, "breakdown");
    SetTargetFPS(60);

    while(!WindowShouldClose()){

        // raccket movement
        if(gameState == game){
            if(racket.x - racketSpeed >= 0 && IsKeyDown(KEY_LEFT)){
                racket.x -= racketSpeed;
                if(!ball.isActive){
                    ball.shape.x -= racketSpeed;
                };
            };
            if(racket.x + racketSpeed + racket.width <= screenWidth && IsKeyDown(KEY_RIGHT)){
                racket.x += racketSpeed;
                if(!ball.isActive){
                    ball.shape.x += racketSpeed;
                };
            };

            // ball initialization
            if(IsKeyPressed(KEY_SPACE) && !ball.isActive){
                ball.isActive = 1;
                ball.ySpeed = -2.7f;
                hitFactor = ((ball.shape.x + ball.shape.width / 2) - screenWidth / 2) / screenWidth;
                ball.xSpeed = hitFactor * 2.7f;
            };
            // ball movement
            if(ball.isActive){
                moveX(&ball, bricks);
                moveY(&ball, bricks);
            };
    
            // ball collision with the screen
            if(ball.shape.y < 42){
                ball.ySpeed *= -1;
            };
            if(ball.shape.x < 0 || ball.shape.x + ball.shape.width > screenWidth){
                ball.xSpeed *= -1;
            };
    
            // ball collison with the racket
            if(CheckCollisionRecs(ball.shape, racket)){
                hitFactor = ((ball.shape.x + ball.shape.width / 2) - (racket.x + racket.width / 2)) / racket.width;
                ball.xSpeed = hitFactor * 2.7f;
                ball.ySpeed *= -1;
            };
    
            if(ball.shape.y > screenHeight){
                gameState = lose;
                ball.isActive = 0;
                timer = 0;
            };
        };

        BeginDrawing();
        ClearBackground(BLACK);
        score = 0;
        for(int x = 0; x < lines; x += 1){
            for(int y = 0; y < rows; y += 1){
                if(bricks[x][y].state){
                    DrawRectangleRec(bricks[x][y].shape, RAYWHITE);
                } else {
                    score += 100;
                };
            };
        };
        if(score == lines * rows * 100){
            gameState = win;
            ball.isActive = 0;
        };
        sprintf(buffer, "SCORE: %d", score);
        DrawText(buffer, 2, 12, 20, RAYWHITE);
        DrawRectangleRec(racket, RAYWHITE);
        DrawRectangleRec(ball.shape, RAYWHITE);
        switch (gameState){
            case game:
                break;
            case win:
                ClearBackground(BLACK);
                drawAtMiddle("WIN", 0, 60);
                drawAtMiddle("press space to reset", 130, 20);
                if(IsKeyPressed(KEY_SPACE)){
                    gameState = game;
                    reset(&racket, &ball, bricks);
                };
                break;
            case lose:
                drawAtMiddle("LOSE", 0, 60);
                timer += GetFrameTime();
                if(timer >= 2){
                    reset(&racket, &ball, bricks);
                    gameState = game;
                    ClearBackground(BLACK);
                }
                break;
        };
        EndDrawing();
    };
    CloseWindow();
    return 0;
};

void drawAtMiddle(char* text, int yOffset, int fontSize){
    DrawText(text, screenWidth / 2 - MeasureText(text, fontSize) / 2, screenHeight / 2 - fontSize / 2 + yOffset, fontSize, RAYWHITE);
};

int collide(Ball* ball, Brick bricks[lines][rows]){
    for(int x = 0; x < lines; x += 1){
        for(int y = 0; y < rows; y += 1){
            Brick* brick = &bricks[x][y];
            if(brick->state){
                if(CheckCollisionRecs(ball->shape, brick->shape)){
                    brick->state = 0;
                    return 1;
                };
            };
        };
    };
    return 0;
};

int moveX(Ball* ball, Brick bricks[lines][rows]){
    ball->shape.x += ball->xSpeed;
    if(collide(ball, bricks)){
        ball->xSpeed *= -1;
    };
};

int moveY(Ball* ball, Brick bricks[lines][rows]){
    ball->shape.y += ball->ySpeed;
    if(collide(ball, bricks)){
        ball->ySpeed *= -1;
    };
};

void setBricks(Brick bricks[lines][rows]){
    // bricks initialization
    for(int x = 0; x < lines; x += 1){
        for(int y = 0; y < rows; y += 1){
            bricks[x][y].shape.width = 40;
            bricks[x][y].shape.height = 10;
            bricks[x][y].shape.x = x * 44 + 2;
            bricks[x][y].shape.y = y * 14 + 2 + 40;
            bricks[x][y].state = 1;
        };
    };
};

void reset(Rectangle *racket, Ball *ball, Brick bricks[lines][rows]){
    racket->x = screenWidth / 2 - 50 / 2;
    racket->y = screenHeight - 90;
    ball->shape.x = racket->x + racket->width / 2 - 5;
    ball->shape.y = racket->y - 15;
    ball->xSpeed = 0;
    ball->ySpeed = 0;
    ball->isActive = 0;

    setBricks(bricks);
};