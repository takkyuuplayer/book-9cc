CFLAGS=-std=c11 -g -static

9cc: 9cc.c

test: 9cc
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean

dump:
	@echo $(CC)
	@echo $(LDFLAGS)

docker:
	docker build -t compilerbook https://www.sigbus.info/compilerbook/Dockerfile

docker/run:
	docker run --volume=${PWD}:/srv:cached -w=/srv -it compilerbook bash
