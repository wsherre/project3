CC=clang
CFLAGS=-Wall -g


BINS=libmyalloc.so t perform


all: libmyalloc.so

libmyalloc.so: allocator.c
	$(CC) $(CFLAGS) -fPIC -shared allocator.c -o libmyalloc.so

perform: allocator.c
	clang -O2 -DNDEBUG -Wall -fPIC -shared allocator.c -o libmyalloc.so

t: test.c allocator.c
	clang -Wall -g -o t test.c allocator.c


clean:
	rm -rf $(BINS) *.dSYM
