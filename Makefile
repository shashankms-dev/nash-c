CC=gcc
CFLAGS=-Wall
BIN=nash
INSTALL_DIR=/usr/local/bin

all: $(BIN)

$(BIN): *.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o $(BIN)

install:
	chmod 755 $(BIN)
	cp -f $(BIN) $(INSTALL_DIR)

uninstall:
	rm -f $(INSTALL_DIR)/$(BIN)
