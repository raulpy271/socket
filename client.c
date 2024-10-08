
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "connection.h"
#include "tic_tac_toe.c"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Deve ser especificado a porta e o ip do servidor\n");
        return 1;
    }
    uint16_t port = atoi(argv[2]);
    printf("Conectando com servidor %s na porta %d\n", argv[1], port);
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == SOCKET_ERR)
    {
        printf("Erro ao criar socket: %d\n", sd);
        return 1;
    }
    printf("Socket criado com sucesso: %d\n", sd);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, argv[1], &addr.sin_addr);

    int ret = connect(sd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == SOCKET_ERR)
    {
        printf("Erro ao conectar com socket: %d\n", ret);
        return 1;
    }

    int height;
    int width;
    int connected_sock;

    msg_t msg;
    int countJogadas = 0;
    game_t game;
    init_game(&game);

    limpar_console();
    show_board(game.board);
    while (true)
    {   
        // Receber Jogada
        printf("\nAguardando jogada do adversario X...\n");
        if (read_play(sd, &msg) == SOCKET_ERR) {
            printf("Erro ao ler socket\n");
            break;
        };
        if (countJogadas + 1 != msg.id) {
            printf("Identificador de mensagem inválido\n");
            break;
        }
        countJogadas++;
        play(&game, msg.h, msg.w);
        limpar_console();
        show_board(game.board);
        if (wins(&game)) {
            printf("Você perdeu!\n");
            break;
        }

        // Enviar Jogada
        printf("Digite a posição X-Y: ");
        scanf("%d-%d", &height, &width);
        if(TTT_ERROR == validateInput(height, width)){
            printf("Input inválido\n");
            break;
        }

        msg.player = PLAYER_O;
        msg.id = ++countJogadas;
        msg.h = height;
        msg.w = width;
        if (play(&game, msg.h, msg.w) == TTT_ERROR) {
            printf("Input inválido\n");
            break;
        }
        send_play(sd, &msg);
        limpar_console();
        show_board(game.board);
        if (wins(&game)) {
            printf("Você venceu!\n");
            break;
        }
    }
    close(sd);
}
