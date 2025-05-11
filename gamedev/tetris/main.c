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
    int points;
    int gameOver;
    int pause;
    int toFetch;
    int linesToDelete;
    float deleteTick;
    float deleteTickSpeed;
    float dropTick;
    float dropTickSpeed;
    Piece currentPiece;
    Piece nextPiece;
    unsigned int grid[GRID_C][GRID_L];
} State;

void drawMenu(void);
void initGame(State*);
void updateGame(State*);
void drawGame(State);
int checkCollisionY(State);
int checkCollisionX(State, int);
void dropPiece(State*);
Piece rotatePiece(State);
void deleteLines(State*);
void dumpGrid(unsigned int [GRID_C][GRID_L]);

int main(void){
    State state;
    int game = 0;
    
    InitWindow(SCREEN_W, SCREEN_H, "tetris");
    SetTargetFPS(60);
    SetExitKey(0);
    
    while(!WindowShouldClose()){
        // wait for game start
        if(!game && IsKeyPressed(KEY_SPACE)){
            game = 1;
            initGame(&state);
        }
        // switch between draw states
        if(game){
            updateGame(&state);
            drawGame(state);
        } else {
            drawMenu();
        }
        //dumpGrid(state.grid);
    }

    CloseWindow();
    return 0;
}

// set initial game state
void initGame(State *state){
    memset(state->grid, 0, sizeof(unsigned int) * GRID_C * GRID_L);
    state->points = 0;
    state->gameOver = 0;
    state->pause = 0;
    state->toFetch = 1;
    state->linesToDelete = 0;
    state->deleteTick = 0;
    state->deleteTickSpeed = .4f;
    state->dropTick = 0;
    state->dropTickSpeed = .4f;
    state->currentPiece.y = 0;
    state->nextPiece = fetchPiece(GetRandomValue(1, 7));
}

void updateGame(State *state){
    if(state->gameOver){
        // make sure game is not paused on game over
        state->pause = 0;
        if(IsKeyPressed(KEY_R)) initGame(state);
    }
    // while game is not over
    if(!state->gameOver){
        // toggle pause
        if(IsKeyPressed(KEY_ESCAPE) && !state->gameOver) state->pause = !state->pause;
        // call the funtion to delete lines if there are lines to delete
        if(state->linesToDelete) deleteLines(state);
        // if game is not paused and there aren't any lines to delete
        if(!state->pause && !state->linesToDelete){
            // if a new piece should be fetched
            if(state->toFetch){
                // if the placed piece is already above the grid
                if(state->currentPiece.y < 0){
                    state->gameOver = 1;
                    return;
                }
                // fetch new piece
                state->currentPiece = state->nextPiece;
                state->nextPiece = fetchPiece(GetRandomValue(1, 7));
                state->toFetch = 0;
                return;
            }

            // check horizontal collisions before moving
            if(!checkCollisionX(*state, (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) - (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)))){
                if(IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) state->currentPiece.x++;
                if(IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) state->currentPiece.x--;
            }
            // rotation
            if(IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)){
                state->currentPiece = rotatePiece(*state);
            }

            // update gravity tick
            state->dropTick += GetFrameTime();
            // change gravity tick when falling faster
            if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)){
                state->dropTickSpeed = .05f;
            } else {
                state->dropTickSpeed = .4f;
            }
            // drop piece
            if(state->dropTick >= state->dropTickSpeed){
                dropPiece(state);
                state->dropTick = 0;
                state->linesToDelete = 1;
            }
        }
    }
}

void drawMenu(void){
    BeginDrawing();
        ClearBackground(BLACK);
        DrawText("TETRIS", SCREEN_W / 2 - MeasureText("TETRIS", BLOCK_SIZE * 3) / 2, SCREEN_H / 2 - BLOCK_SIZE * 1.5f, BLOCK_SIZE * 3, PURPLE);
        DrawText("press SPACE to play", SCREEN_W / 2 - MeasureText("press SPACE to play", BLOCK_SIZE) / 2, SCREEN_H / 4 + SCREEN_H / 2 - BLOCK_SIZE / 2, BLOCK_SIZE, PURPLE);
    EndDrawing();
}

void drawGame(State state){
    BeginDrawing();
    // clear frame buffer
    ClearBackground(BLACK);
        // iterate over all grid blocks and draw each one
        for(int c = 0; c < GRID_C; c++){
            for(int l = 0; l < GRID_L; l++){
                // draw a collored piece when occupied
                if(state.grid[c][l]){
                    DrawRectangle((c + 1) * BLOCK_SIZE + 1, (l + 1) * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, DARKPURPLE);
                // draw it empty otherwise
                } else {
                    DrawRectangle((c + 1) * BLOCK_SIZE + 1, (l + 1) * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, DARKGRAY);
                }
            }
        }
        // do not draw the piece when it was just placed
        if(!state.toFetch){
            drawPiece(state.currentPiece);
        }
        // draw paused overlay
        if(state.pause){
            DrawRectangle(BLOCK_SIZE, BLOCK_SIZE, GRID_C * BLOCK_SIZE, GRID_L * BLOCK_SIZE, (Color){0, 0, 0, 120});
            DrawText("paused", GRID_C * BLOCK_SIZE / 2 + BLOCK_SIZE - MeasureText("paused", 40) / 2, GRID_L * BLOCK_SIZE / 2 + BLOCK_SIZE - 20, 40, RAYWHITE);
        }
        
        // draw game over overlay
        if(state.gameOver){
            DrawRectangle(BLOCK_SIZE, BLOCK_SIZE, GRID_C * BLOCK_SIZE, GRID_L * BLOCK_SIZE, (Color){0, 0, 0, 120});
            DrawText("GAME OVER", GRID_C * BLOCK_SIZE / 2 + BLOCK_SIZE - MeasureText("GAME OVER", 24) / 2, GRID_L * BLOCK_SIZE / 2 + BLOCK_SIZE - 12, 24, RAYWHITE);
            DrawText("press R to restart", GRID_C * BLOCK_SIZE / 2 + BLOCK_SIZE - MeasureText("press R to restart", 20) / 2, GRID_L * BLOCK_SIZE / 2 + BLOCK_SIZE - 10 + BLOCK_SIZE, 20, RAYWHITE);
        }

        // draw the interface
        int nextPieceX = (SCREEN_W / 4 - BLOCK_SIZE * 1.5f + SCREEN_W / 2) / BLOCK_SIZE;

        char buffer[256];
        sprintf(buffer, "points: %d", state.points);
        DrawText(buffer, SCREEN_W / 2 + BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, RAYWHITE);
        DrawText("next piece:", SCREEN_W / 2 + BLOCK_SIZE, BLOCK_SIZE * 3, BLOCK_SIZE, RAYWHITE);
        for(int c = 0; c < 3; c++){
            for(int l = 0; l < 3; l++){
                if(state.nextPiece.bricks[c][l] > 1){
                    DrawRectangle((nextPieceX + c + 1) * BLOCK_SIZE + 1, (4 + l) * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, PURPLE);
                } else if(state.nextPiece.bricks[c][l]){
                    DrawRectangle((nextPieceX + c) * BLOCK_SIZE + 1.5f, (5 + l) * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, PURPLE);
                }
            }
        }

        DrawText("controls:", SCREEN_W / 2 + BLOCK_SIZE, BLOCK_SIZE * 8, BLOCK_SIZE, RAYWHITE);
        DrawText("esc - pause", SCREEN_W / 4 - MeasureText("esc - pause", 20) / 2 + SCREEN_W / 2, BLOCK_SIZE * 10, 20, RAYWHITE);
        DrawText("right/d - move right", SCREEN_W / 4 - MeasureText("right/d - move right", 20) / 2 + SCREEN_W / 2, BLOCK_SIZE * 11, 20, RAYWHITE);
        DrawText("left/a - move left", SCREEN_W / 4 - MeasureText("left/a - move left", 20) / 2 + SCREEN_W / 2, BLOCK_SIZE * 12, 20, RAYWHITE);
        DrawText("up/w - rotate", SCREEN_W / 4 - MeasureText("up/w - rotate", 20) / 2 + SCREEN_W / 2, BLOCK_SIZE * 13, 20, RAYWHITE);
        DrawText("down/s - fast drop", SCREEN_W / 4 - MeasureText("down/s - fast drop", 20) / 2 + SCREEN_W / 2, BLOCK_SIZE * 14, 20, RAYWHITE);

    EndDrawing();
}

// check collisions with the grid floor and blocks right bellow
int checkCollisionY(State state){
    for(int c = 0; c < 3; c++){
        for(int l = 0; l < 3; l++){
            // if there is an occupied brick at {c, l}
            if(state.currentPiece.bricks[c][l]){
                // and right bellow a block is occupied or the position outbounds the grid
                if(l + state.currentPiece.y + 1 == GRID_L || state.grid[c + state.currentPiece.x][l + state.currentPiece.y + 1]) return 1;
            }
        }
    }
    return 0;
}

// check if a piece is horizontally next to another block or the grid walls
int checkCollisionX(State state, int dir){
    for(int c = 0; c < 3; c++){
        for(int l = 0; l < 3; l++){
            // if there is an occupied brick at {c, l}
            if(state.currentPiece.bricks[c][l]){
                // rightside collision
                if((c + state.currentPiece.x + 1 == GRID_C || state.grid[c + state.currentPiece.x + 1][l + state.currentPiece.y]) && dir == 1) return 1;
                // leftside collision
                if((c + state.currentPiece.x - 1 < 0 || state.grid[c + state.currentPiece.x - 1][l + state.currentPiece.y]) && dir == -1) return 1;
            }
        }
    }
    return 0;
}

// check if any brick of a piece is overlapping an occupied grid block
int checkOverlap(State state, Piece new){
    for(int c = 0; c < 3; c++){
        for(int l = 0; l < 3; l++){
            // if there is an occupied brick at {c, l}
            if(new.bricks[c][l]){
                // and also an occupied block at this same position on the grid, offseted by the piece position, return true
                if(state.grid[c + new.x][l + new.y] || new.x + c < 0 || new.x + c == GRID_C) return 1;
            }
        }
    }
    return 0;
}

// drop down a piece
void dropPiece(State *state){
    unsigned int brick = 0;

    // if the no collision is detected, move downwards
    if(!checkCollisionY(*state)){
        state->currentPiece.y++;
    // otherwise, place the piece in the grid and set to fetch a new one
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

// creates a rotated version the current piece.
// it returns the old one if the rotated version is invalid
Piece rotatePiece(State state){
    Piece old = state.currentPiece;
    Piece new = old;
    // if current piece is an O, return old
    if(old.bricks[1][1] == 1) return old;

    // hardcoded rotation
    new.bricks[0][0] = old.bricks[0][2];
    new.bricks[0][1] = old.bricks[1][2];
    new.bricks[0][2] = old.bricks[2][2];
    new.bricks[1][0] = old.bricks[0][1];
    new.bricks[1][1] = old.bricks[1][1];
    new.bricks[1][2] = old.bricks[2][1];
    new.bricks[2][0] = old.bricks[0][0];
    new.bricks[2][1] = old.bricks[1][0];
    new.bricks[2][2] = old.bricks[2][0];

    // check if the rotated version is valid
    if(checkOverlap(state, new)){
        return old;
    }

    return new;
}

void deleteLines(State *state){
    static int deletedLines = 0;

    for(int l = 0; l < GRID_L; l++){
        int count = 0;
        for(int c = 0; c < GRID_C; c++){
            if(state->grid[c][l]) count++;
        }
        if(count == GRID_C){
            state->deleteTick += GetFrameTime();
            if(state->deleteTick >= state->deleteTickSpeed){
                deletedLines++;
                for(int dL = l; dL > 0; dL--){
                    for(int dC = 0; dC < GRID_C; dC++){
                        if(dL == 0){
                            state->grid[dC][dL] = 0;
                        } else {
                            state->grid[dC][dL] = state->grid[dC][dL - 1];
                        }
                    }
                }
                state->deleteTick = 0;
            } else {
                return;
            }
        }
    }
    if(deletedLines) state->points += 100 + (deletedLines - 1) * 50;
    deletedLines = 0;
    state->linesToDelete = 0;
}

// dumps current grid state to the stdout
void dumpGrid(unsigned int grid[GRID_C][GRID_L]){
    // separation
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
