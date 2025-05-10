CC=        gcc
RM=        rm -rf

CFLAGS=    -std=c99 -Wall -Werror -Wpedantic
LDFLAGS=

SRC=   $(wildcard *.c)
OBJ=   $(SRC:.c=.o)
TRG=   rvdis

all: build
build: $(TRG)

$(TRG): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(TRG) $(OBJ)
