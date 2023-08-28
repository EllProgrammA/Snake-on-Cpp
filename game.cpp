#include <iostream> //for working with console
#include <conio.h> //for working with keyboard
#include <windows.h> //for small pause
#include <ctime> //to set new values to srand()



//===========================================================
//---------------First statements----------------------------
//===========================================================
using namespace std;

//---------------functions
void setup();							//settings
void drawField();						//drawing the field at first
void gotoxy(COORD coord);				//change the cursor position
void input();							//working with keyboard
void logic();							//logic of the game
void draw();							//drawing changing elements
void drawStartScreen();					//drawing the start screen
void drawSnakeText(COORD startPosition);//drawing the "Snake" on the start screen
void drawGameover();					//drawing the gameover screen
bool isCoordInTail(COORD coord);		//checks if sth is in tail

//---------------variables
bool gameOver;							//becomes true when game is over

const int width = 20;					//set size of the field
const int height = 20;

int score;								//amount of picked fruits

COORD fruitPos;							//position of a fruit

const int maxLength = 50;				//maximum snake's length allowed
int snakeLength;						//length of the snake

COORD snakeElements[maxLength];			// array with coordinates for whole snake
										// first element [0] is the head
										// middle elements are the snake's tail
										// last element is empty to erase the end of the tail

enum eDirection							//direction of the snake
{ 
	STOP,
	LEFT, 
	RIGHT, 
	UP, 
	DOWN 
} dir; 



//===========================================================
//------------------Main function----------------------------
//===========================================================

int main()
{
	setup();				//settings

	while (dir == STOP) {	//while snake is standing on one place, wait for the first tap
		input();
	}

	//game process
	while (!gameOver) {
		input();			//working with a keyboard
		logic();			//logic of the game
		draw();				//drawing the objects on the field
		Sleep(200);			//small pause
	}

	//stuff to make the stopcode table under the field
	COORD returnPlace = { 0, 30 };
	gotoxy(returnPlace);
	return 0;
}



//===========================================================
//----------------------Functions----------------------------
//===========================================================



//----------------------settings
void setup()	
{
	//hide the cursor
	HANDLE StdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CURSOR;
	CURSOR.bVisible = 0;
	CURSOR.dwSize = 1;
	SetConsoleCursorInfo(StdHandle, &CURSOR);

	//drawing the start screen
	bool startScreenOff = _kbhit();				//while you don't tap any key
	while (!startScreenOff) {					//the start screen will be on
		drawStartScreen();

		startScreenOff = _kbhit();				//check if a key is pressed
	}
	system("cls");								//clear console to draw the field

	gameOver = false;							//at start game is not over

	// at start snake is standing in the middle of the field
	snakeElements[0].X = width / 2;
	snakeElements[0].Y = height / 2;

	//generating the first fruit
	srand(time(NULL)); 
	fruitPos.X = 1 + (rand() % (width - 2));
	fruitPos.Y = 1 + (rand() % (height - 2));

	score = 0;									//at start the score equals zero

	snakeLength = 1;							//at start snake's got only it's head

	drawField();								//drawing the field

	dir = STOP;
}







//----------------------working with keyboard
void input()
{
	if (_kbhit()) {					//if any key is pressed
		switch (_getch())			//recognise it and set direction
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
		case  'x':					//if x is pressed, the game is over
			gameOver = true;
			break;
		}
	}
}



//----------------------logic of the game
void logic()
{
	//if snake eats a fruit
	if (snakeElements[0].X == fruitPos.X && snakeElements[0].Y == fruitPos.Y) {
		//we will get score
		score++; 

		//generate a new fruit
		do {
			fruitPos.X = 1 + (rand() % (width - 2));
			fruitPos.Y = 1 + (rand() % (height - 2));
		} while (isCoordInTail(fruitPos));

		//and add an element to the tail
		if (snakeLength < (maxLength - 1)) {
			snakeLength++;
		}
	}

	//updating tail coordinates
	for (int i = snakeLength; i > 0; i--) 
	{
		snakeElements[i] = snakeElements[i - 1];
	}

	//updating head position
	switch (dir)
	{
	case LEFT:
		snakeElements[0].X--;
		break;
	case RIGHT:
		snakeElements[0].X++;
		break;
	case UP:
		snakeElements[0].Y--;
		break;
	case DOWN:
		snakeElements[0].Y++;
		break;
	}

	//if the snake hits it's tail
	if (isCoordInTail(snakeElements[0])) {
		gameOver = true;					//thegame will be over
		return;
	}

	//going through the walls
	if (snakeElements[0].X >= (width - 1)) {
		snakeElements[0].X = 1;
	}
	else if (snakeElements[0].X < 1) {
		snakeElements[0].X = width - 2;
	}
	if (snakeElements[0].Y >= (height - 1)) {
		snakeElements[0].Y = 1;
	}
	else if (snakeElements[0].Y < 1) {
		snakeElements[0].Y = height - 2;
	}
}



//----------------------drawing
void draw() {
	//drawing a fruit
	gotoxy(fruitPos);
	cout << 'F';

	//drawing the snake
	gotoxy(snakeElements[0]);			//head
	cout << '0';
	gotoxy(snakeElements[1]);			//first element of the tail (others won't be erased)
	cout << 'o';
	gotoxy(snakeElements[snakeLength]);	//erase the last element
	cout << ' ';

	//showing score
	COORD scoreCoord = { 7, 20 };
	gotoxy(scoreCoord);
	cout << score;
}



//----------------------drawing the field at start
void drawField() {
	for (int i = 0; i < height; i++) {
		if ((i == 0) || (i == (height - 1))) {	//if we're on the top or the bottom, draw a border
			for (int m = 0; m < width; m++) {
				cout << "#";
			}
			cout << endl;
		}
		else {
			for (int j = 0; j < width; j++) {
				if (j == 0 || j == width - 1) { //if we're on the left or on the right side, draw a border
					cout << "#";
				}
				else {
					cout << ' ';
				}
			}
			cout << endl;
		}
	}
	cout << "Score: ";							//make a place to show score

	gotoxy(snakeElements[0]);
	cout << "0";
	gotoxy(fruitPos);
	cout << "F";
}


//----------------------drawing the start screen
void drawStartScreen() {
	//
	//   SSS         k
	//   S           k
	//   SSS nnn  aa k k eee
	//     S n n a a kk  ee
	//   SSS n n  aa k k eee
	//
	//     by EllProgrammA
	//
	//
	//Press any key to start!!!

	COORD startPos = { 3, 3 };
	drawSnakeText(startPos);

	startPos.X += 2;
	startPos.Y += 6;
	gotoxy(startPos);
	cout << "by EllProgrammA";

	startPos.X = 0;
	startPos.Y += 3;
	gotoxy(startPos);
	cout << "Press any key to start!!!";
}

void drawSnakeText(COORD startPosition) { // drawing the "Snake" on the start screen
	//   SSS         k
	//   S           k
	//   SSS nnn  aa k k eee
	//     S n n a a kk  ee
	//   SSS n n  aa k k eee

	COORD cursorPosition = startPosition;
	gotoxy(cursorPosition);
	cout << "SSS         k"; cursorPosition.Y++; gotoxy(cursorPosition);
	cout << "S           k"; cursorPosition.Y++; gotoxy(cursorPosition);
	cout << "SSS nnn  aa k k eee"; cursorPosition.Y++; gotoxy(cursorPosition);
	cout << "  S n n a a kk  ee"; cursorPosition.Y++; gotoxy(cursorPosition);
	cout << "SSS n n  aa k k eee";
}

//----------------------drawing the gameover screen
void drawGameover() {

}



//----------------------sets cursor position
void gotoxy(COORD coord)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}



//----------------------checking if sth in the snake's tail
bool isCoordInTail(COORD coord) {
	for (int i = 1; i < snakeLength; i++) {
		if (snakeElements[i].X == coord.X && snakeElements[i].Y == coord.Y) {
			return true;
		}
	}

	return false;
}