CC=clang
CFLAGS=-Wall -g


BINS=libmyalloc.so test


all: $(BINS)

libmyalloc.so: allocator.c
	$(CC) $(CFLAGS) -fPIC -shared allocator.c -o libmyalloc.so

perform: allocator.c
	clang -O2 -DNDEBUG -Wall -fPIC -shared allocator.c -o libmyalloc.so

test: test.c allocator.c
	clang -Wall -g -o test test.c allocator.c


clean:
	rm -rf $(BINS) *.dSYM
