
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
    int ret = connect(sd, (struct sockaddr *) &addr, sizeof(addr));
    if (ret == SOCKET_ERR) {
        printf("Erro ao conectar com socket: %d\n", ret);
        return 1;
    }

    for (int i = 0; i < argc; i++) {
        printf("Escrevendo no socket: %s\n", argv[i]);
        ret = write(sd, argv[i], strlen(argv[i]) + 1);
        if (ret == SOCKET_ERR) {
            printf("Erro ao escrever no socket\n");
        }
    }

    close(sd);
}

