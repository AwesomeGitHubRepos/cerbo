BINS =client server 

all : $(BINS)

clean : 
	rm -f $(BINS)

client : client.c
	gcc -fsanitize=address $^ -o $@ -lasan

server : server.c
	gcc -fsanitize=address $^ -o $@ -lasan
