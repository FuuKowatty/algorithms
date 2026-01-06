CC = gcc
CFLAGS = -g -Wall -Wextra -I./lib -I./lib/node
LDFLAGS =

.PHONY: clean

huffman_q: huffman_q.o node-queue.o node.o cMap.o string.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	./huffman_q.exe

huffman_q.o: ./huffman/queues_impl/huffman.c ./lib/node/node-queue.h ./lib/node/node.h ./lib/c-map.h ./lib/c-string.h
	$(CC) $(CFLAGS) -c $< -o $@

node-queue.o: ./lib/node/node-queue.c ./lib/node/node-queue.h
	$(CC) $(CFLAGS) -c $< -o $@

node.o: ./lib/node/node.c ./lib/node/node.h ./lib/c-string.h
	$(CC) $(CFLAGS) -c $< -o $@

cMap.o: ./lib/c-map.c ./lib/c-map.h ./lib/c-string.h
	$(CC) $(CFLAGS) -c $< -o $@

string.o: ./lib/c-string.c ./lib/c-string.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f huffman_q.o cMap.o string.o node-queue.o node.o