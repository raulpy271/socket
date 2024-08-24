
client.out: client.c connection.h
	gcc client.c -o client.out

server.out: server.c connection.h
	gcc server.c -o server.out

