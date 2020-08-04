editor: editor.c
	gcc -Ofast editor.c -o editor -Wall -Wextra -pedantic -std=c99
clean:
	rm -f editor
run: editor
	./editor