CC=g++
CFLAGS=-std=c++11 -Wall -Wextra -Wno-unused-parameter
EXEC=tp03

$(EXEC): src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o $(EXEC)

clean:
	rm -rf *.o