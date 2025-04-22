#include <stdio.h>
#include <raylib.h>
#include <raymath.h>

#define hardReset score1 = 0; score2 = 0; ball.direction = 1

const int screenWidth = 800;
const int screenHeight = 525;
const int hudOffset = 25;

typedef struct {
    Rectangle shape;
    int isActive;
    int direction; 
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

void stateMachine(Ball*, Rectangle*);
void moveX(Ball *ball, Rectangle racket1, Rectangle racket2);
void moveY(Ball *ball);
int score(Ball *ball, int *score1, int *score2);
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
        .direction = 1,
        .xSpeed = 0,
        .ySpeed = 0,
        .baseAcceleration = 2.7f,
    };

    int cpu = 0;
    int score1 = 0;
    int score2 = 0;
    int maxScore = 3;
    int paused = 0;

    float racketSpeed = 12.0f;
    float timer = .0f;

    char buffer[100];
    char* menuItemsText[] = {"vs player", "vs com", "options", "exit"};
    char* configItemsText[] = {"points: <%d>", "return"};
    char* winItemsText[] = {"rematch", "main menu"};

    GameState currentGameState = mainMenu;
    MenuItems menuItem = vsPlayer;
    ConfigItems configItem = points;
    WinOptions winOptions = rematch;

    InitWindow(screenWidth, screenHeight, "pong");
    SetTargetFPS(60);
    SetExitKey(0);
    
    while(!WindowShouldClose()){
        // main menu
        if(currentGameState == mainMenu){
            // close game shortcut
            if(IsKeyPressed(KEY_ESCAPE)){
                goto finish;
            };
            // scroll over menu items
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
            // select highlighted option
            if(IsKeyPressed(KEY_SPACE)){
                switch(menuItem){
                    case vsPlayer:
                        currentGameState = game;
                        cpu = 0;
                        break;
                    case vsCom:
                        currentGameState = game;
                        cpu = 1;
                        break;
                    case options:
                        configItem = 0;
                        currentGameState = config;
                        break;
                    case exit:
                        goto finish;
                        break;
                };
            };
        // main game loop
        } else if(currentGameState == game && !paused){
            // pause
            if(IsKeyPressed(KEY_ESCAPE)){
                paused = 1;
            };

            // left racket movement
            if(racket1.y - racketSpeed >= 5 + hudOffset + 10 && IsKeyDown(KEY_W)){
                racket1.y -= racketSpeed;
            };
            if(racket1.y + racketSpeed + racket1.height <= screenHeight - 10 && IsKeyDown(KEY_S)){
                racket1.y += racketSpeed;
            };
            // right racket movement
            if(!cpu){
                // by player 2
                if(racket2.y - racketSpeed >= 5 + hudOffset + 10 && IsKeyDown(KEY_UP)){
                    racket2.y -= racketSpeed;
                };
                if(racket2.y + racketSpeed + racket2.height <= screenHeight - 10 && IsKeyDown(KEY_DOWN)){
                    racket2.y += racketSpeed;
                };
            // by CPU
            } else {
                // delay each call by 5 frames
                timer += GetFrameTime();
                if(timer >= (1/60) * 5){
                    timer = 0;
                    stateMachine(&ball, &racket2);
                };
            };

            // ball init
            if(IsKeyPressed(KEY_SPACE) && !ball.isActive){
                // activate ball and set initial speed
                ball.isActive = 1;
                ball.xSpeed = 2.7f * ball.direction;
                ball.ySpeed = (GetRandomValue(-10, 10) / 10) * ball.baseAcceleration;
            };
            // ball movement
            if(ball.isActive){
                moveX(&ball, racket1, racket2);
                moveY(&ball);
                // cap max acceleration
                if(ball.baseAcceleration > 2.7f * 8) ball.baseAcceleration = 2.7f * 8;
            };
            // ball y-axis collision
            if(ball.shape.y < border.y + 10) ball.shape.y = border.y + 11;
            if(ball.shape.y > screenHeight - 10) ball.shape.y = screenHeight - 11;
            
            // on player scoring, reset positions and ball state
            if(score(&ball, &score1, &score2)){
                reset(&ball, &racket1, &racket2);
            };
            // a player won the match
            if(score1 == maxScore || score2 == maxScore){
                winOptions = 0;
                currentGameState = win;
            };
        // pause menu
        } else if(currentGameState == game && paused) {
            if(IsKeyPressed(KEY_ESCAPE)){
                paused = 0;
            };
            // scroll over menu items
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
            // select highlighted option
            if(IsKeyPressed(KEY_SPACE)){
                paused = 0;
                if(winOptions == 0){
                    hardReset;
                    reset(&ball, &racket1, &racket2);
                    currentGameState = game;
                };
                if(winOptions == 1){
                    hardReset;
                    reset(&ball, &racket1, &racket2);
                    menuItem = 0;
                    currentGameState = mainMenu;
                };
            };
        // end of match screen
        } else if(currentGameState == win){
            // scroll over menu items
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
            // select highlighted option
            if(IsKeyPressed(KEY_SPACE)){
                if(winOptions == 0){
                    hardReset;
                    currentGameState = game;
                };
                if(winOptions == 1){
                    hardReset;
                    menuItem = 0;
                    currentGameState = mainMenu;
                };
            };
        // settings screen
        } else if(currentGameState == config){
            // scroll over menu items
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
            // update match win score
            if(configItem == 0){
                if(IsKeyPressed(KEY_A)){
                    if(maxScore > 1) maxScore -= 1; 
                };
                if(IsKeyPressed(KEY_D)) maxScore += 1;
            };
            // return to main menu
            if(IsKeyPressed(KEY_SPACE) && configItem == 1) currentGameState = mainMenu;
        };
        
        BeginDrawing();
        ClearBackground(BLACK);
        // draw current state
        switch(currentGameState){
            case mainMenu:
                DrawText("PONG", screenWidth / 2 - MeasureText("PONG", 100) / 2, screenHeight / 2 - 50, 100, PINK);
                // draw each menu option
                for(int i = 0; i < 4; i += 1){
                    // highlight if selected
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
                // draw game elements
                DrawRectangleLinesEx(border, 10, PINK);
                DrawRectangleRec(division, PINK);
                DrawRectangleRec(racket1, PINK);
                DrawRectangleRec(racket2, PINK);
                DrawRectangleRec(ball.shape, RAYWHITE);
                
                // update score text
                sprintf(buffer, "%d", score1);
                DrawText(buffer, screenWidth / 4 - MeasureText(buffer, 20) / 2, 5, 20, PINK);
                sprintf(buffer, "%d", score2);
                DrawText(buffer, screenWidth / 4 * 3 - MeasureText(buffer, 20) / 2, 5, 20, PINK);

                // pause menu
                if(paused){
                    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0,0,0, 200});
                    DrawText("PAUSED", screenWidth / 2 - MeasureText("PAUSED", 30) / 2, screenHeight / 2 - 15, 30, PINK);
                    // draw each pause menu item
                    for(int i = 0; i < 2; i += 1){
                        // highlight selected option
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
                };
                break;
            case win:
                // draw corresponding win message
                if(score1 == maxScore){
                    sprintf(buffer, "player 1 wins");
                } else {
                    sprintf(buffer, "player 2 wins");
                };
                DrawText(buffer, screenWidth / 2 - MeasureText(buffer, 60) / 2, screenHeight / 2 - 30, 60, PINK);
                // draw each menu option
                for(int i = 0; i < 2; i += 1){
                    // highlight selected option
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
                // draw each menu option
                for(int i = 0; i < 2; i += 1){
                    char* text;
                    // set menu item text depending on which one is selected
                    if(i == points){
                        sprintf(buffer, configItemsText[i], maxScore);
                        text = buffer;
                    } else {
                        text = configItemsText[i];
                    };
                    // highlight selected option
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

// x-axis ball movement and check for collisions on rackets
void moveX(Ball *ball, Rectangle racket1, Rectangle racket2){
    float hitFactor;
    ball->shape.x += ball->xSpeed;

    if(CheckCollisionRecs(ball->shape, racket1)){
        // make ball faster
        ball->baseAcceleration *= 1.05f;
        // tilt back the ball
        ball->shape.x -= ball->xSpeed;
        // update x speed
        ball->xSpeed = ball->baseAcceleration;
        // calculate ball inclination depending on the racket section of collision
        hitFactor = ((ball->shape.y + ball->shape.height / 2) - (racket1.y + racket1.height / 2)) / racket1.height; 
        ball->ySpeed = hitFactor * ball->baseAcceleration;
    };
    if(CheckCollisionRecs(ball->shape, racket2)){
        // make ball faster
        ball->baseAcceleration *= 1.05f;
        // tilt back the ball
        ball->shape.x -= ball->xSpeed;
        // update x speed
        ball->xSpeed = -ball->baseAcceleration;
        // calculate ball inclination depending on the racket section of collision
        hitFactor = ((ball->shape.y + ball->shape.height / 2) - (racket2.y + racket2.height / 2)) / racket2.height; 
        ball->ySpeed = hitFactor * ball->baseAcceleration;
    };
};

// y-axis ball movement
void moveY(Ball *ball){
    ball->shape.y += ball->ySpeed;
    // reflect acceleration on wall collisions
    if(ball->shape.y < 15 + hudOffset){
        ball->ySpeed *= -1;
    };
    if(ball->shape.y > screenHeight - 15 - ball->shape.height){
        ball->ySpeed *= -1;
    };
};

// check for player scoring
int score(Ball *ball, int *score1, int *score2){
    if(ball->shape.x > screenWidth - 15){
        ball->direction = -1;
        *score1 += 1;
        return 1;
    };
    if(ball->shape.x < 15 - ball->shape.width){
        ball->direction = 1;
        *score2 += 1;
        return 1;
    };
    return 0;
};

// reset position and state of ball and rackets
int reset(Ball *ball, Rectangle *racket1, Rectangle *racket2){
    // deactivate ball movement
    ball->isActive = 0;
    // set to original positions
    ball->shape.x = screenWidth / 2 - ball->shape.width / 2;
    ball->shape.y = screenHeight / 2 - ball->shape.height / 2;
    ball->baseAcceleration = 2.7f;
    racket1->y = (racket2->y = screenHeight / 2 - racket2->height / 2);
};

// finite state machine for player 2 (CPU)
void stateMachine(Ball *ball, Rectangle *racket){
    // define movement factor depending on distance between ball and racket
    float factor = ((ball->shape.y + ball->shape.height / 2) - (racket->y + racket->height / 2)) / (screenHeight + 5);
    factor = fabs(factor) * 20;

    // act only when ball is moving towards the right racket and is after the screen's middle
    if(ball->shape.x >= screenWidth / 2 && ball->xSpeed > 0){
        if(ball->shape.y + ball->shape.height / 2 < racket->y + racket->height / 2 && racket->y - factor >= 5 + hudOffset + 10){
            racket->y -= factor;
        };
        if(ball->shape.y + ball->shape.height / 2 > racket->y + racket->height / 2 && racket->y + factor + racket->height <= screenHeight - 10){
            racket->y += factor;
        };
    };
};
