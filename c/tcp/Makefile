BINS =client server 

all : $(BINS)

clean : 
	rm -f $(BINS)

client : client.c
	gcc $^ -o $@

server : server.c
	gcc $^ -o $@
