CC = mpicxx
CFLAGS = -O3 -Ofast -ffast-math
all: matmul

matmul: main.cc matmul.cc 
	$(CC) $(CFLAGS) $^ -o $@ 


clean:
	rm -f matmul