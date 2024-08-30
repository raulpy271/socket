
#include <stdbool.h>
#include <stdio.h>

#define BOARD_W 3
#define BOARD_H 3

enum Mark {
    MARK_EMPTY,
    MARK_O,
    MARK_X
};

enum Player {
    PLAYER_X,
    PLAYER_O
};

typedef enum Mark board_t[BOARD_H][BOARD_W];

typedef struct {
    board_t board;
    enum Player turn;
} game_t;

char show_mark(enum Mark m) {
    switch (m) {
        case MARK_O: 
            return 'O';
        case MARK_X: 
            return 'X';
        default:
            return '_';
    };
}

void show_board(board_t b) {
    for (int i = 0; i < BOARD_H; i++) {
        printf(" %c | %c | %c\n", show_mark(b[i][0]), show_mark(b[i][1]), show_mark(b[i][2]));
    }
}

void init_board(board_t b) {
    for (int i = 0; i < BOARD_H; i++) {
        for (int j = 0; j < BOARD_W; j++) {
            b[i][j] = MARK_EMPTY;
        }
    }
}

void init_game(game_t* game) {
    init_board(game->board);
    game->turn = PLAYER_X;
}

int play(game_t* game, int h, int w) {
    enum Mark mark;
    enum Player next_turn;
    if (game->board[h][w] != MARK_EMPTY) {
        return -1;
    }
    switch (game->turn) {
        case PLAYER_X:
            mark = MARK_X;
            next_turn = PLAYER_O;
            break;
        case PLAYER_O:
            mark = MARK_O;
            next_turn = PLAYER_X;
            break;
    }
    game->board[h][w] = mark;
    game->turn = next_turn;
    return 0;
};

bool wins(game_t* game) {
    // check winner horizontally
    for (int i = 0; i < BOARD_H; i++) {
        if (
                (game->board[i][0] != MARK_EMPTY) && 
                (game->board[i][0] == game->board[i][1]) && 
                (game->board[i][1] == game->board[i][2]) 
        ) {
            return true;
        }
    }
    // check winner vertically 
    for (int j = 0; j < BOARD_W; j++) {
        if (
                (game->board[0][j] != MARK_EMPTY) && 
                (game->board[0][j] == game->board[1][j]) && 
                (game->board[1][j] == game->board[2][j]) 
        ) {
            return true;
        }
    }
    // TODO: check winner on diagonal
    return false;
}

// test main
//int main(int argc, char *argv[]) {
//    game_t game;
//    init_game(&game);
//    play(&game, 0, 0);
//    play(&game, 0, 1);
//    play(&game, 1, 0);
//    play(&game, 1, 1);
//    play(&game, 2, 2);
//    play(&game, 2, 1);
//    show_board(game.board);
//    printf("Wins: %d\n", wins(&game)); 
//}

