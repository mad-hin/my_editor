#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#define CTRL_KEY(k) ((k)&0x1f)

struct termios orig_termios;

// Initialize all functions
void disableRawMode();
void enableRawMode();
void die(const char *s);
char editorReadKey();
void editorProcessKeypress();

/*** terminal part ***/
void disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
		die("tcsetattr");
}

void enableRawMode()
{
	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
		die("tcgetattr");

	atexit(disableRawMode);

	struct termios raw = orig_termios;
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag &= ~(CS8);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
		die("tcsetattr");
}

//show error
void die(const char *s)
{
	//prints an error message and exits the program
	perror(s);
	exit(1);
}
/***End of terminal part ***/

/*** input ***/
// Get character from standard input
char editorReadKey()
{
	int nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
	{
		if (nread == -1 && errno != EAGAIN)
			die("read");
	}
	return c;
}
/*** input ***/
void editorProcessKeypress()
{
	char c = editorReadKey();
	switch (c)
	{
	case CTRL_KEY('q'): // ctrl + q to quit
		exit(0);
		break;
	}
}

// End of input

//main programe
/*** Initialize the programe ***/
int main()
{
	enableRawMode();

	while (1)
	{
		editorProcessKeypress();
	}
	return 0;
}
/*** End of Initialize ***/