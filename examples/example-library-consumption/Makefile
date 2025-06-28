CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I/usr/local/include/btcmarkets
LDFLAGS = -lmarket_data -lcurl -lcjson

SRC = src/main.c
TARGET = main

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean