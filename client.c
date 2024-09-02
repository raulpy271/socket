
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


int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Deve ser especificado a porta e o ip do servidor\n");
        return 1;
    }
    uint16_t port = atoi(argv[2]);
    printf("Conectando com servidor %s na porta %d\n", argv[1], port);
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == SOCKET_ERR) {
        printf("Erro ao criar socket: %d\n", sd);
        return 1;
    } 
    printf("Socket criado com sucesso: %d\n", sd);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, argv[1], &addr.sin_addr);

    int ret = connect(sd, (struct sockaddr *) &addr, sizeof(addr));
    if (ret == SOCKET_ERR) {
        printf("Erro ao conectar com socket: %d\n", ret);
        return 1;
    }

    for (int i = 0; i < argc; i++) {
        printf("Escrevendo no socket mensagem\n");
        msg_t msg;
        msg.id = 0;
        msg.player = PLAYER_O;
        msg.h = 1;
        msg.w = 1;
        send_play(sd, &msg);
    }

    close(sd);
}

