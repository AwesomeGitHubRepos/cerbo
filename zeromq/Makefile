BIN = server client

.PHONY : all clean

akk : $(BIN)


clean :
	rm -f $(BIN)

server : server.c
	gcc -o server server.c -lzmq

client: client.c
	gcc -o client client.c -lzmq
