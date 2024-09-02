
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


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Não foi especificado a porta do servidor\n");
        return 1;
    }
    uint16_t port = atoi(argv[1]);
    int opt = 1;
    printf("Subindo servidor com porta %d\n", port);
    int connected_sock;
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    int s_opt = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    if (s_opt == SOCKET_ERR) {
        printf("Erro ao criar socket: %d\n", s_opt);
        return 1;
    } 
    printf("Socket criado com sucesso: %d\n", sd);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(sd, (struct sockaddr *) &addr, sizeof(addr));
    if (ret == SOCKET_ERR) {
        printf("Erro ao registrar endereço: %d\n", ret);
        return 1;
    }

    ret = listen(sd, 10);
    if (ret == SOCKET_ERR) {
        printf("Erro ao escutar requisições: %d\n", ret);
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        connected_sock = accept(sd, NULL, NULL);
        if (connected_sock == SOCKET_ERR) {
            printf("Erro ao obter requisição: %d\n", connected_sock);
            break;
        }
        for (int j = 0; j < 10; j++) {
            msg_t msg;
            read_play(connected_sock, &msg);
            printf("Mensagem: %d-%d-%d-%d\n", msg.id, (int) msg.player, msg.h, msg.w);
        }
        close(connected_sock);
    }

    close(sd);
}
