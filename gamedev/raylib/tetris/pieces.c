typedef struct {
    int x;
    int y;
    unsigned int bricks[4][4]; 
} Piece;

const Piece lPiece = {
    .bricks = {
        {0, 0, 7, 0},
        {7, 7, 7, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    .x = 0,
    .y = 0,
};

const Piece jPiece = {
    .bricks = {
        {0, 0, 0, 0},
        {6, 6, 6, 0},
        {0, 0, 6, 0},
        {0, 0, 0, 0},
    },
    .x = 0,
    .y = 0,
};

const Piece tPiece = {
    .bricks = {
        {0, 5, 0, 0},
        {0, 5, 5, 0},
        {0, 5, 0, 0},
        {0, 0, 0, 0},
    },
    .x = 0,
    .y = 0,
};

const Piece zPiece = {
    .bricks = {
        {0, 0, 4, 0},
        {0, 4, 4, 0},
        {0, 4, 0, 0},
        {0, 0, 0, 0},
    },
    .x = 0,
    .y = 0,
};

const Piece sPiece = {
    .bricks = {
        {0, 3, 0, 0},
        {0, 3, 3, 0},
        {0, 0, 3, 0},
        {0, 0, 0, 0},
    },
    .x = 0,
    .y = 0,
};

const Piece iPiece = {
    .bricks = {
        {0, 0, 0, 0},
        {2, 2, 2, 2},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    .x = 0,
    .y = 0,
};

const Piece oPiece = {
    .bricks = {
        {0, 0, 0, 0},
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
    },
    .x = 0,
    .y = 0,
};

// returns a copy of the coresponding piece given its ID
Piece fetchPiece(int index){
    Piece newPiece;

    switch(index){
    case 1:
        memcpy(&newPiece, &oPiece, sizeof(Piece));
        break;
    case 2:
        memcpy(&newPiece, &iPiece, sizeof(Piece));
        break;
    case 3:
        memcpy(&newPiece, &sPiece, sizeof(Piece));
        break;
    case 4:
        memcpy(&newPiece, &zPiece, sizeof(Piece));
        break;
    case 5:
        memcpy(&newPiece, &tPiece, sizeof(Piece));
        break;
    case 6:
        memcpy(&newPiece, &jPiece, sizeof(Piece));
        break;
    case 7:
        memcpy(&newPiece, &lPiece, sizeof(Piece));
        break;
    }

    // set the initial position to the middle of the grid and above the grid by 1
    newPiece.x = GRID_C / 2 - 1;
    newPiece.y = -1;

    return newPiece;
}

// draws a given piece
void drawPiece(Piece piece){
    // check how far should the loop iterate
    int max = (piece.bricks[1][1] == 2 || piece.bricks[2][2] == 2) ? 4 : 3;
    for(int c = 0; c < max; c++){
        for(int l = 0; l < max; l++){
            if(piece.bricks[c][l] && piece.y + l + 1 > 0){
                DrawRectangle((piece.x + c + 1) * BLOCK_SIZE + 1, (piece.y + l + 1) * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, PURPLE);
            }
        }
    }
}
