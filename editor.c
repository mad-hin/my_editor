#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#define CTRL_KEY(k) ((k)&0x1f)
#define ABUF_INIT {NULL, 0}
#define EDITOR_VERSION "0.0.1"

/***User Defined Data Types ***/
struct editorConfig
{
	int cx, cy;
	int screenrows;
	int screencols;
	struct termios orig_termios;
};
// Global struct
struct editorConfig E;

// append buffer
struct abuf
{
	char *b;
	int len;
};
/***User Defined Data Types ***/

/*
 * Initialize all functions
 * Prevent compilation error due to function not find
 */
void abAppend(struct abuf *ab, const char *s, int len);
void abFree(struct abuf *ab);
void die(const char *s);
void disableRawMode();
void enableRawMode();
char editorReadKey();
void editorProcessKeypress();
void editorRefreshScreen();
void editorDrawRows(struct abuf *ab);
void initEditor();
int getCursorPosition(int *rows, int *cols);
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
		// "C" = (Cursor Forward), "B" =  (Cursor Down), 999 move at most 999 time (must more than enough)
		// docuement:https://vt100.net/docs/vt100-ug/chapter3.html#CUD
		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
		{
			return -1;
		}
		return getCursorPosition(rows, cols);
	}
	else
	{
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return 0;
	}
}

int getCursorPosition(int *rows, int *cols)
{
	char buf[32];
	unsigned int i = 0;

	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) // \x1b[6n is the Cursor Position Report
		return -1;

	while (i < sizeof(buf) - 1)
	{
		if (read(STDIN_FILENO, &buf[i], 1) != 1)
			break;

		if (buf[i] == 'R')
			break;
		i++;
	}
	buf[i] = '\0';

	if (buf[0] != '\x1b' || buf[1] != '[')
		return -1;

	if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
		return -1;

	return 0;
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
		if (nread == -1 && errno != EAGAIN) // EAGAIN = "there is no data available right now, try again later"
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
// Check if it can get the window size
void initEditor()
{
	E.cx = 0;
	E.cy = 0;

	if (getWindowSize(&E.screenrows, &E.screencols) == -1)
	{
		die("getWindowSize");
	}
}
// End of initEditor

/*** End of Initialize ***/

/*** output ***/
// Clear the screen and reposition the cursor
void editorRefreshScreen()
{
	struct abuf ab = ABUF_INIT;

	// x1b == "esc" in ASCII
	abAppend(&ab, "\x1b[?25l", 6); // \x1b[?25l is used to show the cursor.
	abAppend(&ab, "\x1b[H", 3);	   // \x1b[H is the code for returning the cursor to the home position

	editorDrawRows(&ab);

	char buf[32];
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1); // \x1b[%d;%dH is use to set cursor position
	abAppend(&ab, buf, strlen(buf));

	write(STDOUT_FILENO, ab.b, ab.len);
	abFree(&ab);
}

// Draw "~" on screen like vi/vim editor
void editorDrawRows(struct abuf *ab)
{
	int y;
	for (y = 0; y < E.screenrows; y++)
	{
		if (y == E.screenrows / 3)
		{
			char welcome[80];
			int welcomelen = snprintf(welcome, sizeof(welcome),
									  "Editor -- version %s", EDITOR_VERSION);
			if (welcomelen > E.screencols)
				welcomelen = E.screencols;
			int padding = (E.screencols - welcomelen) / 2;
			if (padding)
			{
				abAppend(ab, "~", 1);
				padding--;
			}
			while (padding--)
				abAppend(ab, " ", 1);
			abAppend(ab, welcome, welcomelen);
		}
		else
		{
			abAppend(ab, "~", 1);
		}

		abAppend(ab, "\x1b[K", 3); // Erase from cursor to end of line

		if (y < E.screenrows - 1)
		{
			abAppend(ab, "\r\n", 1);
		}
	}
}
// End of output

/*** Append Buffer ***/
void abAppend(struct abuf *ab, const char *s, int len)
{
	char *new = realloc(ab->b, ab->len + len);
	if (new == NULL)
		return;
	memcpy(&new[ab->len], s, len);
	ab->b = new;
	ab->len += len;
}

void abFree(struct abuf *ab)
{
	free(ab->b);
}
// End of Append Buffer part