CC=gcc
CFLAGS=-Wall
EXEC=nash

all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

$(EXEC): main.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o $(EXEC)

install:
	chmod 755 $(EXEC)
	cp $(EXEC) /usr/local/bin/
