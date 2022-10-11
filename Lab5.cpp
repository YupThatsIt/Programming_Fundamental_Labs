#include <stdio.h>
#include <windows.h>
#include <conio.h>

void gotoxy(int x, int y) {
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void erase_ship(int x, int y) {
	gotoxy(x, y);
	printf("     ");
}

void draw_ship(int x, int y) {
	gotoxy(x, y);
	printf("<-0->");
}

int main() {
	char ch = ' ';
	int x = 38, y = 20;

	draw_ship(x, y);

	do {
		if (_kbhit()) { //_kbhit return non 0 when press
			ch = _getch();
			if (ch == 'a') {
				if (x > 0) {
					erase_ship(x, y);
					draw_ship(--x, y);
				}
			}
			if (ch == 'd') {
				if (x < 75) { //the ship is 5 characters meaning leftmost of the ship will stop at 80
					erase_ship(x, y);
					draw_ship(++x, y);
				}
			}
			if (ch == 'w') {
				if (y > 0) {
					erase_ship(x, y);
					draw_ship(x, --y);
				}
			}
			if (ch == 's') {
				if (y < 24) {
					erase_ship(x, y);
					draw_ship(x, ++y);
				}
			}
			fflush(stdin);
		}
		Sleep(100);
	} while (ch != 'x');
}
