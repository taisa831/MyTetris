prd:
	gcc -o tetris main.c

install:
	install -s tetris /usr/local/bin
