run: huffman.o cMap.o string.o
	gcc huffman.o cMap.o string.o -o huffman -lws2_32
	./huffman.exe

huffman.o: huffman.c ./lib/c-map.h ./lib/c-string.h
	gcc -c huffman.c -o huffman.o

cMap.o: ./lib/c-map.c ./lib/c-map.h ./lib/c-string.h
	gcc -c ./lib/c-map.c -o cMap.o

string.o: ./lib/c-string.c ./lib/c-string.h
	gcc -c ./lib/c-string.c -o string.o