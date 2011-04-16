all: c99 c89 gnu

c99: restrict-exam.c
	gcc -o c99 restrict-exam.c  -std=c99

c89: restrict-exam.c
	gcc -o c89 restrict-exam.c  -std=c89

gnu: restrict-exam.c
	gcc -o c89 restrict-exam.c

clean:
	rm c99 c89 gnu
