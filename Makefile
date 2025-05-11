CC=        gcc
RM=        rm -rf

CFLAGS=    -std=c89 -Wall -Werror -Wpedantic
LDFLAGS=

DIS-SRC=   rvdis.c
DIS-OBJ=   $(DIS-SRC:.c=.o)
DIS-TRG=   rvdis

all: build
build: $(DIS-TRG)

$(DIS-TRG): $(DIS-OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(DIS-TRG) $(DIS-OBJ)
