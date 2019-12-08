CFLAGS = -I. -Itests -g -O2 -Wall -W -mavx

all: driver

main.c:
	bash tests/gentest.sh tests/test-*.c > $@

driver: main.c tests/test-cirbuf.c tests/unit-test.c main.c
	$(CC) $(CFLAGS) -o $@ $^
	./$@
bench: tests/benchmark.c
	$(CC) $(CFLAGS) -o $@ $^ 
plot: bench
	bash tests/plot.sh


clean:
	$(RM) main.c driver bench *.png
