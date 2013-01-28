CFLAGS  = -Wall -Wextra -Werror -pedantic -std=c99 -O3 -D_XOPEN_SOURCE=700
LDFLAGS = -O3
TARGETS = block alpha shift

all: $(TARGETS)

%: %.c
	gcc $(LDFLAGS) $(CFLAGS) $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o

destroy: clean
	rm -f $(TARGETS)

rebuild: destroy all
