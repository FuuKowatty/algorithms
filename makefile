CC = gcc
CFLAGS = -Wall -Wextra -I./lib
LDFLAGS =

.PHONY: clean

huffman_q: huffman_q.o cMap.o string.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	./huffman_q.exe

huffman_q.o: ./huffman/queues_impl/huffman.c ./lib/c-map.h ./lib/c-string.h
	$(CC) $(CFLAGS) -c $< -o $@

cMap.o: ./lib/c-map.c ./lib/c-map.h ./lib/c-string.h
	$(CC) $(CFLAGS) -c $< -o $@

string.o: ./lib/c-string.c ./lib/c-string.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f huffman_q.o cMap.o string.o