
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "connection.h"

#define SOCKET_ERR -1


int main(int argc, char *argv[]) {
    int connected_sock;
    char buffer[BUFFER_SIZE];
    int sd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (sd == SOCKET_ERR) {
        printf("Erro ao criar socket: %d\n", sd);
        return 1;
    } 
    printf("Socket criado com sucesso: %d\n", sd);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);
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
            read(connected_sock, buffer, BUFFER_SIZE);
            buffer[BUFFER_SIZE - 1] = 0;
            printf("Texto recebido: %s\n", buffer);
        }
        close(connected_sock);
    }

    close(sd);
    unlink(SOCKET_NAME);
}
