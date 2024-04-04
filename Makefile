all: clean
	$(CC) $(CFLAGS) -Wall -Wextra -o cgltf.o -c -fPIC cgltf.c
	$(CC) $(CFLAGS) -shared -o libcgltf.so cgltf.o
	ar rcs libcgltf.a cgltf.o

ossfuzz: all
	$(CC) $(CFLAGS) -o harness fuzz/test.c -I . -L . -lcgltf

clean:
	rm -rf cgltf.o
	rm -rf libcgltf.a
	rm -rf libcgltf.so
	rm -rf harness