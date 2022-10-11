#include <Windows.h>
#include <time.h>
#include <stdio.h>

const int scount = 40;
#define screen_x 80
#define screen_y 25

HANDLE wHnd;
HANDLE rHnd;
COORD bufferSize = { screen_x, screen_y };
COORD characterPos = { 0,0 };
COORD star[scount];
SMALL_RECT windowSize = { 0, 0, screen_x - 1, screen_y - 1 };
CHAR_INFO consoleBuffer[screen_x * screen_y];
DWORD fdwMode;

int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}

void clear_buffer() {
	for (int y = 0; y < screen_y; y++) {
		for (int x = 0; x < screen_x; x++) {
			consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
			consoleBuffer[x + screen_x * y].Attributes = 0;
		}
	}
}

void init_star() {
	for (int i = 0; i < scount; i++) {
		star[i] = { (short)(rand() % screen_x), (short)(rand() % screen_y) };
	}
}

int setMode()
{
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	SetConsoleMode(rHnd, fdwMode);
	return 0;
}

//Update star pos to buffer
void fill_star_to_buffer()
{
	for (int i = 0; i < scount; i++) {
		consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar = '*';
		consoleBuffer[star[i].X + screen_x * star[i].Y].Attributes = 7;
	}
}

void fill_ship_to_buffer(int posx, int posy, int color) {
	consoleBuffer[posx + screen_x * posy - 2].Char.AsciiChar = '<';
	consoleBuffer[posx + screen_x * posy - 2].Attributes = color;
	consoleBuffer[posx + screen_x * posy - 1].Char.AsciiChar = '-';
	consoleBuffer[posx + screen_x * posy - 1].Attributes = color;
	consoleBuffer[posx + screen_x * posy].Char.AsciiChar = 'O';
	consoleBuffer[posx + screen_x * posy].Attributes = color;
	consoleBuffer[posx + screen_x * posy + 1].Char.AsciiChar = '-';
	consoleBuffer[posx + screen_x * posy + 1].Attributes = color;
	consoleBuffer[posx + screen_x * posy + 2].Char.AsciiChar = '>';
	consoleBuffer[posx + screen_x * posy + 2].Attributes = color;
}

void star_fall() {
	for (int i = 0; i < scount; i++) {
		if (star[i].Y >= screen_y - 1) {
			star[i] = { (short)(rand() % screen_x), 1 };
		}
		else {
			star[i] = { star[i].X, (short)(star[i].Y + 1) };
		}
	}
}

//Update display
void fill_buffer_to_console()
{
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos, &windowSize);
}

void collision(int posx, int posy, int *hp) {
	for (int i = 0; i < scount; i++) {
		for (int j = -2; j <= 2; j++) {
			if (star[i].Y == posy && star[i].X == posx + j) {
				star[i] = { (short)(rand() % screen_x), 1 };
				*hp -= 1;
				//Beep((*hp + 1) * 100, 100);
			}
		}
	}
}

int main() {
	srand(time(NULL));
	setConsole(screen_x, screen_y);
	bool play = true;
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	setMode();
	init_star();

	int color = 7;
	int count = 0;
	int posx = 0;
	int posy = 0;
	int hp = 10;
	while (play == true && count <= 1000 && hp > 0) {
		GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		if (numEvents != 0) {
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
			for (DWORD i = 0; i < numEventsRead; ++i) {
				if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true) {
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
						play = false;
					}
					if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'c') {
						color = rand() % 256;
					}
				}
				else if (eventBuffer[i].EventType == MOUSE_EVENT) {
					posx = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
					posy = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
					if (eventBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
						color = rand() % 256;
					}
				}
			}
			delete[] eventBuffer;
		}
		star_fall();
		clear_buffer();
		fill_star_to_buffer();
		fill_ship_to_buffer(posx, posy, color);
		collision(posx, posy, &hp);
		fill_buffer_to_console();
		Sleep(100);
		count++;
	}
	return 0;
}
