#include <iostream> //for working with console
#include <conio.h> //for working with keyboard
#include <windows.h> //for small pause
#include <ctime> //to set new values to srand()



//===========================================================
//---------------First statements----------------------------
//===========================================================
using namespace std;

//---------------functions
void setup();//settings
void drawField();//drawing the field at first
void gotoxy(int x, int y);//change the cursor position
void input();//working with keyboard
void logic();//logic of the game
void draw();//drawing changing elements
bool isCoordInTail(int x, int y);//checks if sth is in tail

//---------------variables
bool gameOver; //becomes true when game is over

const int width = 20; //set size of the field
const int height = 20;

int headX, headY, fruitX, fruitY, score; //coordinates of the snake, fruits and amount of picked fruits

const int maxTail = 50;
int tailX[maxTail], tailY[maxTail]; // coordinates of snake's tail
int nTail; //length of the snake's tail
int lastX, lastY;

enum eDirection{ STOP = 0, LEFT, RIGHT, UP, DOWN } dir; //direction of snake



//===========================================================
//------------------Main function----------------------------
//===========================================================

int main()
{
	setup();

	while (!gameOver) {
		input();
		logic();
		draw();
		Sleep(200); //small pause
	}

	gotoxy(0, 30);
	return 0;
}



//===========================================================
//----------------------Functions----------------------------
//===========================================================



//----------------------settings
void setup()	
{
	gameOver = false;

	dir = STOP; // at start snake is standing in the middle of the field
	headX = width / 2;
	headY = height / 2;

	srand(time(NULL)); //generating the first fruit
	fruitX = 1 + (rand() % (width - 2));
	fruitY = 1 + (rand() % (height - 2));
	score = 0;

	//at start there's no tail
	nTail = 0;

	lastX = -1;



	drawField();

	//hide the cursor
	HANDLE StdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CURSOR;
	CURSOR.bVisible = FALSE;
	CURSOR.dwSize = 1;
	SetConsoleCursorInfo(StdHandle, &CURSOR);
}



//----------------------drawing
void draw() {

	gotoxy(headX, headY);
	cout << '0';

	gotoxy(fruitX, fruitY);
	cout << 'F';

	gotoxy(7, 20);
	cout << score;

	//Drawing the tail
	/*for (int i = 0; i < nTail; i++) {
		gotoxy(tailX[i], tailY[i]);
		cout << 'o';
	}*/
	if (nTail > 0) {
		gotoxy(tailX[0], tailY[0]);
		cout << 'o';
		gotoxy(tailX[nTail - 1], tailY[nTail - 1]);
		cout << 'o';
	}

	if (lastX >= 0) {
		gotoxy(lastX, lastY);
		cout << ' ';
	}
}




void drawField() {
	for (int i = 0; i < height; i++) {
		if ((i == 0) || (i == (height - 1))) {
			for (int m = 0; m < width; m++) {
				cout << "#";
			}
			cout << endl;
		}
		else {
			for (int j = 0; j < width; j++) {
				if (j == 0 || j == width - 1) { //if we're on the left or on the right side, draw border
					cout << "#";
				}
				else {
					cout << ' ';
				}
			}
			cout << endl;
		}
	}
	cout << "Score: ";
}



//----------------------keyboard
void input() 
{
	if (_kbhit()) { //if any key is pressed
		switch (_getch()) //recognise it and set direction
		{
		case 'a':
			dir = LEFT;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case 'w':
			dir = UP;
			break;
		case 's':
			dir = DOWN;
			break;
		case  'x': //if x is pressed, the game is over
			gameOver = true;
			break;
		}
	}
}



//----------------------Logic of the game
void logic()
{
	//Remember previous head position
	int prevHeadX = headX;
	int prevHeadY = headY;
	bool isTailGrown = false;

	//Update head's position
	//change coordinates
	switch (dir)
	{
	case LEFT:
		headX--;
		break;
	case RIGHT:
		headX++;
		break;
	case UP:
		headY--;
		break;
	case DOWN:
		headY++;
		break;
	}

	//going through walls
	if (headX >= (width - 1)) {
		headX = 1;
	}
	else if (headX < 1) {
		headX = width - 2;
	}
	if (headY >= (height - 1)) {
		headY = 1;
	}
	else if (headY < 1) {
		headY = height - 2;
	}

	//if the snake eats it's tail, the game will be over
	if (isCoordInTail(headX, headY)) {
		gameOver = true;
		return;
	}

	//if the snake eats a fruit
	if (headX == fruitX && headY == fruitY) {
		score++; //we will get score

		//generate a new fruit
		do {
			fruitX = 1 + (rand() % (width - 2));
			fruitY = 1 + (rand() % (height - 2));
		} while (isCoordInTail(fruitX, fruitY));

		if (nTail < maxTail)
			isTailGrown = true;
	}

	//Update tail (move towards the head)
	if ((nTail == 0) && (dir != STOP)) {
		lastX = prevHeadX;
		lastY = prevHeadY;
	}
	else {
		lastX = tailX[nTail - 1];
		lastY = tailY[nTail - 1];
	}

	if(isTailGrown){
		nTail++; //and the tail will become longer

		lastX = -1;
	}
	if (nTail > 0) {
		for (int i = nTail - 1; i > 0; i--) {
			tailX[i] = tailX[i - 1];
			tailY[i] = tailY[i - 1];
		}
		tailX[0] = prevHeadX;
		tailY[0] = prevHeadY;
	}
}



//----------------------sets cursor position
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}



//----------------------checking if sth in the snake's tail
bool isCoordInTail(int x, int y) {
	for (int i = 0; i < nTail; i++) {
		if (tailX[i] == x && tailY[i] == y) {
			return true;
		}
	}

	return false;
}