#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#define CTRL_KEY(k) ((k)&0x1f)

// User Defined Data Types
struct editorConfig
{
	int screenrows;
	int screencols;
	struct termios orig_termios;
};
// Global struct
struct editorConfig E;

/*
 * Initialize all functions
 * Prevent compilation error due to function not find
 */
void die(const char *s);
void disableRawMode();
void enableRawMode();
char editorReadKey();
void editorProcessKeypress();
void editorRefreshScreen();
void editorDrawRows();
void initEditor();
int getWindowSize(int *rows, int *cols);

/*** terminal part ***/
void disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
		die("tcsetattr");
}

void enableRawMode()
{
	if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1)
		die("tcgetattr");

	atexit(disableRawMode);

	struct termios raw = E.orig_termios;
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
	// x1b == "esc" in ASCII
	write(STDOUT_FILENO, "\x1b[2J", 4); // \x1b[2J is the code for clearing the screen and set cursor to home
	write(STDOUT_FILENO, "\x1b[H", 3);	// \x1b[H is the code for returning the cursor to the home position
	//prints an error message and exits the program
	perror(s);
	exit(1);
}

// Get the current window size
int getWindowSize(int *rows, int *cols)
{
	struct winsize ws;
	// ioctl maens (input/output control)
	// TIOCGWINSZ = Terminal Input/Output Control Get WINdow SiZe.
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
	{
		return -1;
	}
	else
	{
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return 0;
	}
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
		// x1b == "esc" in ASCII
		write(STDOUT_FILENO, "\x1b[2J", 4); // \x1b[2J is the code for clearing the screen and set cursor to home
		write(STDOUT_FILENO, "\x1b[H", 3);	// \x1b[H is the code for returning the cursor to the home position
		exit(0);
		break;
	}
}
// End of input

/*** Initialize the programe ***/
//main programe
int main()
{
	enableRawMode();
	initEditor();

	while (1)
	{
		editorRefreshScreen();
		editorProcessKeypress();
	}
	return 0;
}

void initEditor()
{
	if (getWindowSize(&E.screenrows, &E.screencols) == -1)
		die("getWindowSize");
}

/*** End of Initialize ***/

/*** output ***/
// Clear the screen and reposition the cursor
void editorRefreshScreen()
{
	// x1b == "esc" in ASCII
	write(STDOUT_FILENO, "\x1b[2J", 4); // \x1b[2J is the code for clearing the screen and set cursor to home
	write(STDOUT_FILENO, "\x1b[H", 3);	// \x1b[H is the code for returning the cursor to the home position

	editorDrawRows();
	write(STDOUT_FILENO, "\x1b[H", 3); // \x1b[H is the code for returning the cursor to the home position
}

// Draw "~" on screen like vi/vim editor
void editorDrawRows()
{
	int y;
	for (y = 0; y < E.screenrows; y++)
	{
		write(STDOUT_FILENO, "~\r\n", 3);
	}
}
// End of output