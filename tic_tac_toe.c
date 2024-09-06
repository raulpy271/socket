
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
    #include <windows.h>
#endif
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

typedef struct {
    int id;
    enum Player player;
    int h;
    int w;
} msg_t;


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

    // check winner on diagonal
     if (
                (game->board[0][0] != MARK_EMPTY) && 
                (game->board[0][0] == game->board[1][1]) && 
                (game->board[1][1] == game->board[2][2]) 

                ||

                (game->board[0][2] != MARK_EMPTY) && 
                (game->board[0][2] == game->board[1][1]) && 
                (game->board[1][1] == game->board[2][0]) 
        ) {
            return true;
        }
    return false;
}

char whichPlayerWon(game_t* game){
    if(!wins(game)) return 'N';
    
    enum Player player;
    for (int i = 0; i < BOARD_H; i++) {
        if (
                (game->board[i][0] != MARK_EMPTY) && 
                (game->board[i][0] == game->board[i][1]) && 
                (game->board[i][1] == game->board[i][2]) 
        ) {
            player = game->board[i][0];
        }
    }
    // check winner vertically 
    for (int j = 0; j < BOARD_W; j++) {
        if (
                (game->board[0][j] != MARK_EMPTY) && 
                (game->board[0][j] == game->board[1][j]) && 
                (game->board[1][j] == game->board[2][j]) 
        ) {
             player = game->board[0][j];
        }
    }

    // check winner on diagonal
     if (
                (game->board[0][0] != MARK_EMPTY) && 
                (game->board[0][0] == game->board[1][1]) && 
                (game->board[1][1] == game->board[2][2]) 

                ||

                (game->board[0][2] != MARK_EMPTY) && 
                (game->board[0][2] == game->board[1][1]) && 
                (game->board[1][1] == game->board[2][0]) 
        ) {
             player = game->board[1][1];
        }

        if(player == PLAYER_O) return '0';
        
        return 'X';
}

void send_play(int sock, msg_t* msg) {
    char buff[BUFFER_SIZE] = "_-_-_-_";
    char player;
    if (msg->player == PLAYER_X) {
        player = 'x';
    } else {
        player = 'o';
    }

    sprintf(buff, "%d-%c-%d-%d", msg->id, player, msg->h, msg->w);
    int ret = write(sock, buff, BUFFER_SIZE);
    if (ret == SOCKET_ERR) {
        printf("Erro ao escrever no socket\n");
    }
}

int validateInput(int h, int w){
    if( h>=0 && h<=2) return 1;
    if( w>=0 && w<=2) return 1;

    printf("x E Y devem está entre 0-2");
    return 0;
}

void read_play(int sock, msg_t* msg) {
    char player;
    char buff[BUFFER_SIZE];
    read(sock, buff, BUFFER_SIZE);
    sscanf(buff, "%d-%c-%d-%d", &msg->id, &player, &msg->h, &msg->w);
    if (player == 'x') {
        msg->player = PLAYER_X;
    } else {
        msg->player = PLAYER_O;
    }
}

void limpar_console() {
    #ifdef _WIN32
        system("cls"); 
    #else
        system("clear");
    #endif
}

// test main
// int main(int argc, char *argv[]) {
//    game_t game;
//    init_game(&game);
//    play(&game, 0, 0);
//    play(&game, 0, 1);

//    play(&game, 1, 1);
//    play(&game, 1, 0);
   
//    play(&game, 2, 2);
//    play(&game, 2, 1);
//    show_board(game.board);

//    printf("Wins: %d\n", wins(&game)); 
//    printf("Win: %c\n", whichPlayerWon(&game));
// }

