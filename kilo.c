// Filename: kilo.c
// Description: simple c text editor
// Source: https://viewsourcecode.org/snaptoken/kilo/
// Date: 11/22/21
// Author: Charles Roberson
//

// PRE-PROCESSOR DIRECTIVES
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// DEFINITIONS
#define CTRL_KEY(k) ((k) & 0x1f)

// DATA 
struct termios orig_termios;

// TERMINAL FUNCTIONS
void die(const char *s){
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
	perror(s);
	exit(1);
}

void disableRawMode(){
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1){
		die("tcsetattr");
	}
}

void enableRawMode(){
	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
	atexit(disableRawMode);
		
	struct termios raw = orig_termios;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;
	
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcgetattr");
}

char editorReadKey(){
	int nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1){
		if (nread == -1 && errno != EAGAIN) die("read");
	}
	return c;
}

// OUTPUT
void editorDrawRows(){
	int y;
	for (y = 0; y < 24; y++){
		write(STDOUT_FILENO, "~\r\n", 3);
	}
}

void editorRefreshScreen(){
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
	editorDrawRows();
	write(STDOUT_FILENO, "\x1b[H", 3);
}

// INPUT
void editorProcessKeypress(){
	char c = editorReadKey();
	switch (c){
		case CTRL_KEY('q'):
			write(STDOUT_FILENO, "\x1b[2J", 4);
			write(STDOUT_FILENO, "\x1b[H", 3);
			exit(0);
			break;
	}
}

// INITIALIZATION
int main(){
	enableRawMode();
	while (1){
		editorRefreshScreen();
		editorProcessKeypress();
	}
	return 0;
}
