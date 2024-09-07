
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
#include "connection.h"
#include "tic_tac_toe.c"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Não foi especificado a porta do servidor\n");
        return 1;
    }
    uint16_t port = atoi(argv[1]);
    int opt = 1;
    printf("Subindo servidor com porta %d\n", port);
    int connected_sock;
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    int s_opt = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    if (s_opt == SOCKET_ERR)
    {
        printf("Erro ao criar socket: %d\n", s_opt);
        return 1;
    }
    printf("Socket criado com sucesso: %d\n", sd);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(sd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == SOCKET_ERR)
    {
        printf("Erro ao registrar endereço: %d\n", ret);
        return 1;
    }

    ret = listen(sd, 10);
    if (ret == SOCKET_ERR)
    {
        printf("Erro ao escutar requisições: %d\n", ret);
        return 1;
    }

    int height;
    int width;
    msg_t msg;
    connected_sock = accept(sd, NULL, NULL);
    if (connected_sock == SOCKET_ERR)
    {
        printf("Erro ao obter requisição: %d\n", connected_sock);
    }

    int countJogadas = 0;
    game_t game;
    init_game(&game);

    limpar_console();
    show_board(game.board);
    while (true)
    {

        // Enviar Jogada
        printf("Digite a posição X-Y: ");
        scanf("%d-%d", &height, &width);

        if (validateInput(height, width) == TTT_ERROR) {
            printf("Input inválido\n");
            break;
        }

        msg.id = ++countJogadas;
        msg.player = PLAYER_X;
        msg.h = height;
        msg.w = width;
        if (play(&game, msg.h, msg.w) == TTT_ERROR) {
            printf("Input inválido\n");
            break;
        }
        limpar_console();
        show_board(game.board);
        send_play(connected_sock, &msg);

        printf("\nAguardando jogada do adversario O...\n");
        if (read_play(connected_sock, &msg) == SOCKET_ERR) {
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
    }
    close(connected_sock);
    close(sd);
}

