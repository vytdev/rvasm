CC=        gcc
RM=        rm -rf

CFLAGS=    -std=c89 -Wall -Werror -Wpedantic
LDFLAGS=

DIS-SRC=   rvdis.c utils.c
DIS-OBJ=   $(DIS-SRC:.c=.o)
DIS-TRG=   rvdis

ASM-SRC=   lexer.c rvasm.c utils.c
ASM-OBJ=   $(ASM-SRC:.c=.o)
ASM-TRG=   rvasm

all: build
build: $(DIS-TRG) $(ASM-TRG)

$(DIS-TRG): $(DIS-OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

$(ASM-TRG): $(ASM-OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(DIS-TRG) $(DIS-OBJ) $(ASM-TRG) $(ASM-OBJ)
