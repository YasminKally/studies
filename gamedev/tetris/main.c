#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <raylib.h>
#include <raymath.h>

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

#define GRID_C 10
#define GRID_L 20
#define BLOCK_SIZE 24
#define SCREEN_H (GRID_L + 2) * BLOCK_SIZE
#define SCREEN_W (GRID_C * 2 + 4) * BLOCK_SIZE

#define assert(cond) { if(!cond){ fprintf(stderr, "%s:%d:1 %s", __FILE__, __LINE__, #cond); exit(1); } }

#include "pieces.c"

typedef struct {
    int points;
    int highScore;
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

typedef struct {
    Sound place;
    Sound fastPlace;
    Sound movement;
    Sound lineDelete;
    Sound gameover;
} Sfx;

Music theme;

void drawMenu(void);
void initGame(State*);
void updateGame(State*, Sfx);
void drawGame(State);
int checkCollisionY(State);
int checkCollisionX(State, int);
void dropPiece(State*, Sfx);
Piece rotateKickPiece(State, Sfx);
void deleteLines(State*, Sfx);
void dumpGrid(unsigned int [GRID_C][GRID_L]);

int main(void){
    State state;
    Sfx sfx;
    FILE *hScoreData;
    int game = 0;
    
    InitWindow(SCREEN_W, SCREEN_H, "tetris");
    InitAudioDevice();
    SetTargetFPS(60);
    SetExitKey(0);
    
    theme = LoadMusicStream("assets/theme.wav");
    theme.looping = 1;
    SetMusicVolume(theme, .3f);
    PlayMusicStream(theme);

    sfx.place = LoadSound("assets/place.wav");
    sfx.fastPlace = LoadSound("assets/fastplace.wav");
    sfx.movement = LoadSound("assets/move.wav");
    sfx.lineDelete = LoadSound("assets/linedelete.wav");
    sfx.gameover = LoadSound("assets/gameover.wav");
    
    if(access("highScore.txt", F_OK) == 0){
        hScoreData = fopen("highScore.txt", "r");
        fscanf(hScoreData, "%d", &state.highScore);
        fclose(hScoreData);
    }
    
    while(!WindowShouldClose()){
        UpdateMusicStream(theme);
        // wait for game start
        if(!game && IsKeyPressed(KEY_SPACE)){
            game = 1;
            initGame(&state);
        }
        // switch between draw states
        if(game){
            updateGame(&state, sfx);
            drawGame(state);

            // exit game if escape is pressed on game over
            if(IsKeyPressed(KEY_ESCAPE)) break;
        } else {
            drawMenu();
        }
        //dumpGrid(state.grid);
    }

    CloseAudioDevice();
    CloseWindow();
    hScoreData = fopen("highScore.txt", "w+");
    fprintf(hScoreData, "%d", state.highScore);
    fclose(hScoreData);
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
    PlayMusicStream(theme);
}

void updateGame(State *state, Sfx sfx){
    if(state->gameOver){
        // make sure game is not paused on game over
        state->pause = 0;
        StopMusicStream(theme);
        if(state->points > state->highScore) state->highScore = state->points;
        if(IsKeyPressed(KEY_R)) initGame(state);
    }
    // while game is not over
    if(!state->gameOver){
        // toggle pause
        if(IsKeyPressed(KEY_ESCAPE) && !state->gameOver) state->pause = !state->pause;
        // call the funtion to delete lines if there are lines to delete
        if(state->linesToDelete) deleteLines(state, sfx);
        // if game is not paused and there aren't any lines to delete
        if(!state->pause && !state->linesToDelete){
            // if a new piece should be fetched
            if(state->toFetch){
                // if the placed piece is already above the grid
                if(state->currentPiece.y < 0){
                    state->gameOver = 1;
                    PlaySound(sfx.gameover);
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
                if(IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)){
                    state->currentPiece.x++;
                    PlaySound(sfx.movement);
                }
                if(IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)){
                    state->currentPiece.x--;
                    PlaySound(sfx.movement);
                }
            }
            // rotation
            if(IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)){
                state->currentPiece = rotateKickPiece(*state, sfx);
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
                dropPiece(state, sfx);
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

        // base X position for the next piece miniature
        int nextPieceX = (SCREEN_W / 4 - BLOCK_SIZE * 1.5f + SCREEN_W / 2) / BLOCK_SIZE;
        // check how far should the loop iterate
        int max = state.nextPiece.bricks[1][1] == 2 ? 4 : 3;

        char buffer[256];
        sprintf(buffer, "points: %d", state.points);
        DrawText(buffer, SCREEN_W / 2 + BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, RAYWHITE);
        sprintf(buffer, "highscore: %d", state.highScore);
        DrawText(buffer, SCREEN_W / 2 + BLOCK_SIZE, BLOCK_SIZE * 2, BLOCK_SIZE, RAYWHITE);
        DrawText("next piece:", SCREEN_W / 2 + BLOCK_SIZE, BLOCK_SIZE * 4, BLOCK_SIZE, RAYWHITE);

        for(int c = 0; c < max; c++){
            for(int l = 0; l < max; l++){
                if(state.nextPiece.bricks[c][l] == 2){
                    DrawRectangle((nextPieceX + l + 1) * BLOCK_SIZE + 1, (5 + c) * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, PURPLE);
                } else if(state.nextPiece.bricks[c][l] > 1){
                    DrawRectangle((nextPieceX + c + 1) * BLOCK_SIZE + 1, (5 + l) * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, PURPLE);
                } else if(state.nextPiece.bricks[c][l]){
                    DrawRectangle((nextPieceX + c) * BLOCK_SIZE + 1.5f, (6 + l) * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, PURPLE);
                }
            }
        }

        DrawText("controls:", SCREEN_W / 2 + BLOCK_SIZE, BLOCK_SIZE * 9, BLOCK_SIZE, RAYWHITE);
        DrawText("esc - pause", SCREEN_W / 4 - MeasureText("esc - pause", 20) / 2 + SCREEN_W / 2, BLOCK_SIZE * 11, 20, RAYWHITE);
        DrawText("right/d - move right", SCREEN_W / 4 - MeasureText("right/d - move right", 20) / 2 + SCREEN_W / 2, BLOCK_SIZE * 12, 20, RAYWHITE);
        DrawText("left/a - move left", SCREEN_W / 4 - MeasureText("left/a - move left", 20) / 2 + SCREEN_W / 2, BLOCK_SIZE * 13, 20, RAYWHITE);
        DrawText("up/w - rotate", SCREEN_W / 4 - MeasureText("up/w - rotate", 20) / 2 + SCREEN_W / 2, BLOCK_SIZE * 14, 20, RAYWHITE);
        DrawText("down/s - fast drop", SCREEN_W / 4 - MeasureText("down/s - fast drop", 20) / 2 + SCREEN_W / 2, BLOCK_SIZE * 15, 20, RAYWHITE);

    EndDrawing();
}

// check collisions with the grid floor and blocks right bellow
int checkCollisionY(State state){
    // check how far should the loop iterate
    int max = (state.currentPiece.bricks[1][1] == 2 || state.currentPiece.bricks[2][2] == 2) ? 4 : 3;
    for(int c = 0; c < max; c++){
        for(int l = 0; l < max; l++){
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
    // check how far should the loop iterate
    int max = (state.currentPiece.bricks[1][1] == 2 || state.currentPiece.bricks[2][2] == 2) ? 4 : 3;
    for(int c = 0; c < max; c++){
        for(int l = 0; l < max; l++){
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

// drop down a piece
void dropPiece(State *state, Sfx sfx){
    // current piece's brick value
    unsigned int brick = 0;
    // check how far should the loop iterate
    int max = (state->currentPiece.bricks[1][1] == 2 || state->currentPiece.bricks[2][2] == 2) ? 4 : 3;
    
    // if the no collision is detected, move downwards
    if(!checkCollisionY(*state)){
        state->currentPiece.y++;
    // otherwise, place the piece in the grid and set to fetch a new one
    } else {
        for(int c = 0; c < max; c++){
            for(int l = 0; l < max; l++){
                brick = state->currentPiece.bricks[c][l];
                if(brick && state->currentPiece.y + l >= 0){
                    state->grid[c + state->currentPiece.x][l + state->currentPiece.y] = brick;
                }
            }
        }
        // check at which speed the piece is being placed
        if(state->dropTickSpeed == .4f){
            PlaySound(sfx.place);
        } else {
            PlaySound(sfx.fastPlace);
        }
        state->toFetch = 1;  
    }
}

// check if any brick of a piece is overlapping an occupied grid block
int checkOverlap(State state, Piece new){
    // check how far should the loop iterate
    int max = (state.currentPiece.bricks[1][1] == 2 || state.currentPiece.bricks[2][2] == 2) ? 4 : 3;
    for(int c = 0; c < max; c++){
        for(int l = 0; l < max; l++){
            // if there is an occupied brick at {c, l}
            if(new.bricks[c][l]){
                // and also an occupied block at this same position on the grid, offseted by the piece position, return true
                if(state.grid[c + new.x][l + new.y] || new.x + c < 0 || new.x + c == GRID_C) return 1;
            }
        }
    }
    return 0;
}

// returns a rotated version a given piece.
Piece rotatePiece(Piece old){
    Piece new = old;
    // if current piece is an O, return old
    if(old.bricks[1][1] == 1) return old;
    // hardcoded rotation for 3x3 pieces
    else if(old.bricks[1][1] == 2 || old.bricks[2][2] == 2){
        new.bricks[0][0] = old.bricks[0][3];
        new.bricks[0][1] = old.bricks[1][3];
        new.bricks[0][2] = old.bricks[2][3];
        new.bricks[0][3] = old.bricks[3][3];
        new.bricks[1][0] = old.bricks[0][2];
        new.bricks[1][1] = old.bricks[1][2];
        new.bricks[1][2] = old.bricks[2][2];
        new.bricks[1][3] = old.bricks[3][2];
        new.bricks[2][0] = old.bricks[0][1];
        new.bricks[2][1] = old.bricks[1][1];
        new.bricks[2][2] = old.bricks[2][1];
        new.bricks[2][3] = old.bricks[3][1];
        new.bricks[3][0] = old.bricks[0][0];
        new.bricks[3][1] = old.bricks[1][0];
        new.bricks[3][2] = old.bricks[2][0];
        new.bricks[3][3] = old.bricks[3][0];
    // hardcoded rotation for 4x4 pieces
    } else {
        new.bricks[0][0] = old.bricks[0][2];
        new.bricks[0][1] = old.bricks[1][2];
        new.bricks[0][2] = old.bricks[2][2];
        new.bricks[1][0] = old.bricks[0][1];
        new.bricks[1][1] = old.bricks[1][1];
        new.bricks[1][2] = old.bricks[2][1];
        new.bricks[2][0] = old.bricks[0][0];
        new.bricks[2][1] = old.bricks[1][0];
        new.bricks[2][2] = old.bricks[2][0];
    }
    
    return new;
}

// rotates the current piece and attempts to place it by "kicking" it to the sides 
Piece rotateKickPiece(State state, Sfx sfx){
    Piece old = state.currentPiece;
    Piece new = rotatePiece(old);
    // check if the rotated version is valid
    if(checkOverlap(state, new)){
        // attempt to rotate at max three times
        for(int i = 0; i < 3; i++){
            // iterate over every kick variation
            for(int x = -1; x < 2; x++){
                for(int y = 0; y < 2; y++){
                    // update kicked position
                    new.x = old.x + x;
                    new.y = old.y + y;
                    if(!checkOverlap(state, new)){
                        PlaySound(sfx.movement);
                        return new;
                    }
                }
            }
            // reset kicked position
            new.x = old.x;
            new.y = old.y;
            new = rotatePiece(new);
            if(!checkOverlap(state, new)){
                PlaySound(sfx.movement);
                return new;
            }
        }
    } else {
        PlaySound(sfx.movement);
        return new;
    }

    return old;
}

// checks for completed lines and then delete them, pulling the grid down
void deleteLines(State *state, Sfx sfx){
    static int deletedLines = 0;

    // iterate over entire grid
    for(int l = 0; l < GRID_L; l++){
        int count = 0;
        for(int c = 0; c < GRID_C; c++){
            if(state->grid[c][l]) count++;
        }
        // if number of blocks in the current line is equal to the line size (i.e. completed)
        if(count == GRID_C){
            // delayed countdown to deletion
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
                PlaySound(sfx.lineDelete);
            } else {
                return;
            }
        }
    }
    if(deletedLines){
        state->points += 100 + (deletedLines - 1) * 50;
    }
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
