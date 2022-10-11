#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

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

void draw_asteroid(int x, int y) {
	setcolor(6, 0);
	gotoxy(x, y);
	printf("*");
}

void erase_bullet(int x, int y) {
	setcolor(7, 0);
	gotoxy(x, y);
	printf(" ");
}

void draw_bullet(int x, int y) {
	setcolor(7, 0);
	gotoxy(x, y);
	printf("^");
}

char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))

		return '\0';
	else
		return buf[0];

}

void shoot(int *x, int *y, int *bstatus) {
	erase_bullet(*x, *y);
	if (*y == 0) {
		*x = 0, *y = 0;
		*bstatus = 0;
		return;
	}
	else {
		if (cursor(*x, *y - 1) == '*') {
			erase_bullet(*x, *y);
			Beep(500, 100);
			*x = 0, * y = 0;
			*bstatus = 0;
			return;
		}
		draw_bullet(*x, --*y);
	}
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
	int count = 0;

	gotoxy(80, 0);
	printf("Score : %d", count);

	srand(time(NULL));
	setcursor(0);
	setcolor(2, 4);
	draw_ship(x, y);

	for (int i = 0; i < 20; i++) {
		int ax = (rand() % 61) + 9;
		int ay = (rand() % 4) + 1;
		draw_asteroid(ax, ay);
	}

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
				for (int i = 0; i < 5; i++) {
					if (b[i].x == 0 && b[i].y == 0) {
						b[i].status = 1;
						b[i].x = x + 2; b[i].y = y - 1;
						break;
					}
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
		for (int i = 0; i < 5; i++) {
			if (b[i].status == 1) {
				erase_bullet(b[i].x, b[i].y);
				if (b[i].y == 0) {
					b[i].x = 0, b[i].y = 0;
					b[i].status = 0;
				}
				else {
					if (cursor(b[i].x, b[i].y - 1) == '*') {
						erase_bullet(b[i].x, b[i].y - 1);
						count++;
						gotoxy(88, 0);
						printf("%d", count);

						Beep(500, 100);

						int ax, ay;

						do {
							ax = (rand() % 61) + 9;
							ay = (rand() % 4) + 1;
						} while (cursor(ax, ay) == '*');
						draw_asteroid(ax, ay);

						b[i].x = 0, b[i].y = 0;
						b[i].status = 0;
					}
					else {
						draw_bullet(b[i].x, --b[i].y);
					}
				}
			}
		}
		Sleep(50);
	} while (ch != 'x');
	return 0;
}
