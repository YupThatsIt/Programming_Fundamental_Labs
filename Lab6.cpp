#include <stdio.h>
#include <windows.h>
#include <conio.h>

//Let's do it with Struct basically shit Class, but alright.

typedef struct Bullet
{
	int x = 0;
	int y = 0;
	int status = 0;
} bullet;

void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}

void setcolor(int fg, int bg) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg); //wAttribute = hex, index 0 for foreground, index 1 for background
}

void gotoxy(int x, int y) {
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void erase_bullet(int x, int y) {
	setcolor(0, 0);
	gotoxy(x, y);
	printf(" ");
}

void draw_bullet(int x, int y) {
	setcolor(7, 0);
	gotoxy(x, y);
	printf("^");
}

void shoot(int* x, int* y, int* bstatus) {
	if (*y == 0 && *x == 0) {
		return;
	}
	else if (*y == 0) {
		erase_bullet(*x, *y);
		*x = 0, * y = 0;
		*bstatus = 0;
		return;
	}

	erase_bullet(*x, *y);
	draw_bullet(*x, -- * y);
}

void erase_ship(int x, int y) {
	setcolor(0, 0);
	gotoxy(x, y);
	printf("      ");
}

void draw_ship(int x, int y) {
	gotoxy(x, y);
	setcolor(2, 4);
	printf("<-0->");
}

int main() {
	char ch = ' ', dir = 's';
	int x = 38, y = 20, bulletCount = 0;
	bullet b[5];

	setcursor(0);
	setcolor(2, 4);
	draw_ship(x, y);

	do {
		if (_kbhit()) { //_kbhit return non 0 when press
			ch = _getch();
			if (ch == 'a') {
				dir = 'l';
			}
			if (ch == 'd') {
				dir = 'r';
			}
			if (ch == 's') {
				dir = ' ';
			}
			if (ch == ' ') {
				if (bulletCount <= 5) {
					for (int i = 0; i < 5; i++) {
						if (b[i].x == 0 && b[i].y == 0) {
							b[i].status = 1;
							b[i].x = x + 2; b[i].y = y - 1;
							break;
						}
					}
					bulletCount++;
				}
			}
			fflush(stdin);
		}
		//movement by direction
		if (dir == 'r') {
			if (x < 75) {
				erase_ship(x, y);
				draw_ship(++x, y);
			}
		}
		else if (dir == 'l') {
			if (x > 0) {
				erase_ship(x, y);
				draw_ship(--x, y);
			}
		}
		//Shoot with the maximum of 5 bullets in the scene
		if (bulletCount != 0) {
			for (int i = 0; i < 5; i++) {
				if (b[i].status == 1) {
					shoot(&b[i].x, &b[i].y, &b[i].status);
					bulletCount--;
				}
			}
		}

		Sleep(100);
	} while (ch != 'x');
	return 0;
}
