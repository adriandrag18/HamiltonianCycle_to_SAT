build:
	gcc tema.c -Wall
run: build
	./a.out
clean:
	rm a.out
