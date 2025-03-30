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

typedef enum {
    mainMenu,
    game,
    config, 
    win
} GameState;

typedef enum {
    vsPlayer,
    vsCom,
    options, 
    exit
} MenuItems;

typedef enum {
    points,
    back,
} ConfigItems;

typedef enum {
    rematch,
    menu,
} WinOptions;

void moveX(Ball *ball, Rectangle racket1, Rectangle racket2);
void moveY(Ball *ball);
int score(Ball ball, int *score1, int *score2, int *ballDirection);
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
    
    int ballDirection = 1;
    int score1 = 0;
    int score2 = 0;
    int maxScore = 3;
    float racketSpeed = 12.0f;
    float timer = .0f;
    char buffer[100];
    char* menuItemsText[] = {"vs player", "vs com", "options", "exit"};
    char* configItemsText[] = {"points: <%d>", "return"};
    char* winItemsText[] = {"rematch", "main menu"};
    GameState state = mainMenu;
    MenuItems menuItem = vsPlayer;
    ConfigItems configItem = points;
    WinOptions winOptions = rematch;
    
    
    InitWindow(screenWidth, screenHeight, "pong");
    SetTargetFPS(60);
    
    while(!WindowShouldClose()){
        if(state == mainMenu){
            if(IsKeyPressed(KEY_W)){
                menuItem -= 1;
                if((int)menuItem < 0){
                    menuItem = 3;
                };
            };
            if(IsKeyPressed(KEY_S)){
                menuItem += 1;
                if(menuItem > 3){
                    menuItem = 0;
                };
            };
            if(IsKeyPressed(KEY_SPACE)){
                switch(menuItem){
                    case vsPlayer:
                        state = game;
                        break;
                    case options:
                        configItem = 0;
                        state = config;
                        break;
                    case exit:
                        goto finish;
                        break;
                };
            };
        } else if(state == game){
            // player 1 movement
            if(racket1.y - racketSpeed >= 5 + hudOffset + 10 && IsKeyDown(KEY_W)){
                racket1.y -= racketSpeed;
            };
            if(racket1.y + racketSpeed + racket1.height <= screenHeight - 10 && IsKeyDown(KEY_S)){
                racket1.y += racketSpeed;
            };
            
            // player 2 movement
            if(racket2.y - racketSpeed >= 5 + hudOffset + 10 && IsKeyDown(KEY_UP)){
                racket2.y -= racketSpeed;
            };
            if(racket2.y + racketSpeed + racket2.height <= screenHeight - 10 && IsKeyDown(KEY_DOWN)){
                racket2.y += racketSpeed;
            };
            
            // ball init
            if(IsKeyPressed(KEY_SPACE) && !ball.isActive){
                ball.isActive = 1;
                ball.xSpeed = 2.7f * ballDirection;
                ball.ySpeed = (GetRandomValue(-10, 10) / 10) * ball.baseAcceleration;
            };
            
            // ball movement
            if(ball.isActive){
                moveX(&ball, racket1, racket2);
                moveY(&ball);
            };
            
            if(ball.shape.y < border.y + 10) ball.shape.y = border.y + 11;
            if(ball.shape.y > screenHeight - 10) ball.shape.y = screenHeight - 11;
            
            if(score(ball, &score1, &score2, &ballDirection)){
                reset(&ball, &racket1, &racket2);
            };
            
            if(score1 == maxScore || score2 == maxScore){
                winOptions = 0;
                state = win;
            };
        } else if(state == win){
            if(IsKeyPressed(KEY_W)){
                winOptions -= 1;
                if((int)winOptions < 0){
                    winOptions = 1;
                };
            };
            if(IsKeyPressed(KEY_S)){
                winOptions += 1;
                if(winOptions > 1){
                    winOptions = 0;
                };
            };
            if(IsKeyPressed(KEY_SPACE)){
                if(winOptions == 0){
                    score1 = 0;
                    score2 = 0;
                    state = game;
                };
                if(winOptions == 1){
                    score1 = 0;
                    score2 = 0;
                    menuItem = 0;
                    puts("win");
                    state = mainMenu;
                };
            };
        } else if(state == config){
            if(IsKeyPressed(KEY_W)){
                configItem -= 1;
                if((int)configItem < 0){
                    configItem = 1;
                };
            };
            if(IsKeyPressed(KEY_S)){
                configItem += 1;
                if(configItem > 1){
                    configItem = 0;
                };
            };
            if(configItem == 0){
                if(IsKeyPressed(KEY_A)){
                    if(maxScore > 1) maxScore -= 1; 
                };
                if(IsKeyPressed(KEY_D)) maxScore += 1;
            };
            if(IsKeyPressed(KEY_SPACE) && configItem == 1) state = mainMenu;
        };
        
        BeginDrawing();
        ClearBackground(BLACK);
        switch(state){
            case mainMenu:
            DrawText("PONG", screenWidth / 2 - MeasureText("PONG", 100) / 2, screenHeight / 2 - 50, 100, PINK);
            for(int i = 0; i < 4; i += 1){
                    if(menuItem == i){
                        int size = MeasureText(menuItemsText[i], 30);
                        int x = screenWidth / 2 - size / 2;
                        int y = screenHeight / 2 + 50 + i * 30;
                        DrawRectangle(x - 6, y - 1, size + 12, 32, PINK);
                        DrawText(menuItemsText[i], x, y, 30, BLACK);
                    } else {
                        DrawText(menuItemsText[i], screenWidth / 2 - MeasureText(menuItemsText[i], 30) / 2, screenHeight / 2 + 50 + i * 30, 30, PINK);
                    };
                };
                break;
            case game:
                DrawRectangleLinesEx(border, 10, PINK);
                DrawRectangleRec(division, PINK);
                DrawRectangleRec(racket1, PINK);
                DrawRectangleRec(racket2, PINK);
                DrawRectangleRec(ball.shape, RAYWHITE);
                sprintf(buffer, "%d", score1);
                DrawText(buffer, screenWidth / 4 - MeasureText(buffer, 20) / 2, 5, 20, PINK);
                sprintf(buffer, "%d", score2);
                DrawText(buffer, screenWidth / 4 * 3 - MeasureText(buffer, 20) / 2, 5, 20, PINK);
                break;
            case win:
                if(score1 == maxScore){
                    sprintf(buffer, "player 1 wins");
                } else {
                    sprintf(buffer, "player 2 wins");
                };
                DrawText(buffer, screenWidth / 2 - MeasureText(buffer, 60) / 2, screenHeight / 2 - 30, 60, PINK);

                for(int i = 0; i < 2; i += 1){
                    if(i == winOptions){
                        int size = MeasureText(winItemsText[i], 30);
                        int x = screenWidth / 2 - size / 2;
                        int y = screenHeight / 2 + 50 + i * 30;
                        DrawRectangle(x - 6, y - 1, size + 12, 32, PINK);
                        DrawText(winItemsText[i], x, y, 30, BLACK);
                    } else {
                        DrawText(winItemsText[i], screenWidth / 2 - MeasureText(winItemsText[i], 30) / 2, screenHeight / 2 + 50 + i * 30, 30, PINK);
                    };
                };
                break;
            case config:
                DrawText("OPTIONS", screenWidth / 2 - MeasureText("OPTIONS", 80) / 2, screenHeight / 2 - 40, 80, PINK);
                for(int i = 0; i < 2; i += 1){
                    char* text;
                    if(i == points){
                        sprintf(buffer, configItemsText[i], maxScore);
                        text = buffer;
                    } else {
                        text = configItemsText[i];
                    };
                    if(configItem == i){
                        int size = MeasureText(text, 30);
                        int x = screenWidth / 2 - size / 2;
                        int y = screenHeight / 2 + 50 + i * 30;
                        DrawRectangle(x - 6, y - 1, size + 12, 32, PINK);
                        DrawText(text, x, y, 30, BLACK);
                    } else {
                        DrawText(text, screenWidth / 2 - MeasureText(text, 30) / 2, screenHeight / 2 + 50 + i * 30, 30, PINK);
                    };
                };
                break;
        };
        EndDrawing();
    };
    finish: CloseWindow();
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

int score(Ball ball, int *score1, int *score2, int *ballDirection){
    if(ball.shape.x > screenWidth - 15){
        *ballDirection = 1;
        *score1 += 1;
        return 1;
    };
    if(ball.shape.x < 15 - ball.shape.width){
        *ballDirection = -1;
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