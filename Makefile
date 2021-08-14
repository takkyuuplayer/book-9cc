CFLAGS=-std=c11 -g -static

chibicc: chibicc.c

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
