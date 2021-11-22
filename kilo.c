// Filename: kilo.c
// Description: simple c text editor
// Source: https://viewsourcecode.org/snaptoken/kilo/
// Date: 11/22/21
// Author: Charles Roberson
//

#include <termios.h>
#include <unistd.h>

void enableRawMode(){
	struct termios raw;
	tcgetattr(STDIN_FILENO, &raw);
	raw.c_lflag &= ~(ECHO);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(){
	enableRawMode();
	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
	return 0;
}
