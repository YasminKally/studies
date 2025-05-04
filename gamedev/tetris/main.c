#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <raylib.h>
#include <raymath.h>

#define GRID_C 10
#define GRID_L 20
#define BLOCK_SIZE 24
#define SCREEN_H (GRID_L + 2) * BLOCK_SIZE
#define SCREEN_W (GRID_C * 2 + 4) * BLOCK_SIZE

#define assert(cond) { if(!cond){ fprintf(stderr, "%s:%d:1 %s", __FILE__, __LINE__, #cond); exit(1); } }

#include "pieces.c"

typedef struct {
    int gameOver;
    int pause;
    int toFetch;
    float dropTick;
    float dropTickSpeed;
    float moveTick;
    float moveTickSpeed;
    Piece currentPiece;
    unsigned int grid[GRID_C][GRID_L];
} State;


void initGame(State*);
void updateGame(State*);
void drawGame(State);
int checkCollisionY(State*);
int checkCollisionX(State*, int);
void dropPiece(State*);
void dumpGrid(unsigned int [GRID_C][GRID_L]);

int main(void){
    State state;
    
    InitWindow(SCREEN_W, SCREEN_H, "tetris");
    SetTargetFPS(60);
    SetExitKey(0);
    initGame(&state);

    while(!WindowShouldClose()){
        updateGame(&state);
        drawGame(state);
        //dumpGrid(state.grid);
    }

    CloseWindow();
    return 0;
}

void initGame(State *state){
    memset(state->grid, 0, sizeof(unsigned int) * GRID_C * GRID_L);
    state->dropTick = 0;
    state->dropTickSpeed = .4f;
    state->moveTick = 0;
    state->moveTickSpeed = .2f;
    state->gameOver = 0;
    state->pause = 0;
    state->toFetch = 1;
}

void updateGame(State *state){
    if(!state->gameOver){
        if(IsKeyPressed(KEY_ESCAPE) && !state->gameOver) state->pause = !state->pause;
        if(state->gameOver) state->pause = 0;
        if(!state->pause){
            if(state->toFetch){
                if(state->currentPiece.y < 0){
                    state->gameOver = 1;
                    return;
                }
                state->currentPiece = fetchPiece(GetRandomValue(1, 7));
                state->toFetch = 0;
                return;
            }

            state->moveTick += GetFrameTime();
            if(state->moveTick >= state->moveTickSpeed){
                if(!checkCollisionX(state, (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) - (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)))){
                    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) state->currentPiece.x++;
                    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) state->currentPiece.x--;
                }
                state->moveTick = 0;
            }


            state->dropTick += GetFrameTime();
            if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)){
                state->dropTickSpeed = .05f;
            } else {
                state->dropTickSpeed = .4f;
            }
            if(state->dropTick >= state->dropTickSpeed){
                dropPiece(state);
                state->dropTick = 0;
            }
        }
    }
}

void drawGame(State state){
    BeginDrawing();
        ClearBackground(BLACK);
        for(int c = 0; c < GRID_C; c++){
            for(int l = 0; l < GRID_L; l++){
                if(state.grid[c][l]){
                    DrawRectangle((c + 1) * BLOCK_SIZE + 1, (l + 1) * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, DARKPURPLE);
                } else {
                    DrawRectangle((c + 1) * BLOCK_SIZE + 1, (l + 1) * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, DARKGRAY);
                }
            }
        }
        if(!state.toFetch){
            drawPiece(state.currentPiece);
        }
        if(state.pause){
            DrawRectangle(BLOCK_SIZE, BLOCK_SIZE, GRID_C * BLOCK_SIZE, GRID_L * BLOCK_SIZE, (Color){0, 0, 0, 120});
            DrawText("paused", GRID_C * BLOCK_SIZE / 2 + BLOCK_SIZE - MeasureText("paused", 40) / 2, GRID_L * BLOCK_SIZE / 2 + BLOCK_SIZE - 20, 40, RAYWHITE);
        }
    EndDrawing();
}

int checkCollisionY(State* state){
    for(int c = 0; c < 3; c++){
        for(int l = 0; l < 3; l++){
            if(state->currentPiece.bricks[c][l]){
                if(l + state->currentPiece.y + 1 == GRID_L || state->grid[c + state->currentPiece.x][l + state->currentPiece.y + 1]) return 1;
            }
        }
    }
    return 0;
}

int checkCollisionX(State* state, int dir){
    for(int c = 0; c < 3; c++){
        for(int l = 0; l < 3; l++){
            if(state->currentPiece.bricks[c][l]){
                if((c + state->currentPiece.x + 1 == GRID_C || state->grid[c + state->currentPiece.x + 1][l + state->currentPiece.y]) && dir == 1) return 1;
                if((c + state->currentPiece.x - 1 < 0 || state->grid[c + state->currentPiece.x - 1][l + state->currentPiece.y]) && dir == -1) return 1;
            }
        }
    }
    return 0;
}

void dropPiece(State *state){
    unsigned int brick = 0;

    if(!checkCollisionY(state)){
        state->currentPiece.y++;
    } else {
        for(int c = 0; c < 3; c++){
            for(int l = 0; l < 3; l++){
                brick = state->currentPiece.bricks[c][l];
                if(brick && state->currentPiece.y + l >= 0){
                    state->grid[c + state->currentPiece.x][l + state->currentPiece.y] = brick;
                }
            }
        }
        state->toFetch = 1;
    }
}

void dumpGrid(unsigned int grid[GRID_C][GRID_L]){
    printf("----------\n");
    for(int l = 0; l < GRID_L; l++){
        for(int c = 0; c < GRID_C; c++){
            if(grid[c][l]){
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }    
}
