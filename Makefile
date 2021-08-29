CFLAGS=-std=c11 -g -static # Cの最新規格であるC11で書かれたソースコード
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o) # *.c → *.o に変えたもの

chibicc: $(OBJS)
	$(CC) -o chibicc $(OBJS) $(LDFLAGS)

$(OBJS): chibicc.h

test: chibicc
	./test.sh

clean:
	rm -f chibicc *.o *~ tmp*

.PHONY: test clean

dump:
	@echo $(CC)
	@echo $(LDFLAGS)

docker:
	docker build -t compilerbook https://www.sigbus.info/compilerbook/Dockerfile

docker/run:
	docker run --volume=${PWD}:/srv:cached -w=/srv -it compilerbook bash
