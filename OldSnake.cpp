#include <iostream>
#include <Windows.h>
#include <ctime>
#include <conio.h>
#include <chrono>

using namespace std;



class Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> m_StartTime;

public:
	void Start();
	float GetDuration();
};

void Timer::Start()
{
    m_StartTime = std::chrono::high_resolution_clock::now();
}

float Timer::GetDuration()
{
    std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - m_StartTime;
    return duration.count();
}


int main()
{

	
	srand(time(0)); // start random number generator
	rand(); // idling random number generator
	system("mode con cols=51 lines=31"); // setting console window size
	MoveWindow(GetConsoleWindow(), 50, 50, 2000, 2000, true); // setting the start position of the console window (50 and 50 are pixels
	// relative to the top left corner of the monitor
	const int width = 50, height = 30; // the size of the field on which the snake runs
	const int max_length = 50; // setting the maximum length of the "snake"
	int array_X[max_length]; // an array that stores the abscissas of the "snake" links
	int array_Y[max_length]; // an array that stores the ordinates of the "snake" links
	int length = 1; // snake length variable
	array_X[0] = width / 2; // setting the starting abscissa "snake"
	array_Y[0] = height / 2; // setting the starting ordinate of the "snake"
	int dx = 1, dy = 0; // creating offsets along the axes for the movement of the "snake"
	int X_apple; // abscissa "apple"
	int Y_apple; // ordinate "apple"
	do // the loop puts the coordinates of the apple randomly - but so that they do not coincide with the "snake"
	{
		X_apple = rand() % (width - 2) + 1;
		Y_apple = rand() % (height - 2) + 1;
	} while (X_apple != array_X[length - 1] && Y_apple != array_Y[length - 1]);

	int sleep_time = 100; // variable frame rate



	char snake = '*'; // symbol for displaying the body of a "snake"
	char apple = 'o'; // character to display "apple"
	char head = 1; // symbol for displaying the head of a "snake"
	COORD c; // object to store coordinates
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // creating an output stream handle
	CONSOLE_CURSOR_INFO cci = { sizeof(cci), false }; // creating parameters for displaying the cursor
	SetConsoleCursorInfo(h, &cci); // linking parameters and handle

	SetConsoleTextAttribute(h, 4); // setting the color used to draw the field frame
	for (int y = 0; y < height; y++) // standard double loop for drawing the frame
	{
		for (int x = 0; x < width; x++)
		{
			char s;
			if (x == 0 && y == 0) // in the upper left corner of the field - the symbol of the corresponding corner
				s = 218;
			else if (x == 0 && y == height - 1) // lower left corner
				s = 192;
			else if (y == 0 && x == width - 1) // upper right corner
				s = 191;
			else if (y == height - 1 && x == width - 1) // lower right corner
				s = 217;
			else if (y == 0 || y == height - 1) // top and bottom margin
				s = 196;
			else if (x == 0 || x == width - 1) //left and right field border
				s = 179;
			else s = ' '; // in all other cases there should be just a space (meaning an empty area of the field)
			putchar(s); // print the symbol
		}
		cout << endl;
	}

	c.X = X_apple; // linking the coordinate object with the positions of the "apple"
	c.Y = Y_apple;
	SetConsoleCursorPosition(h, c); // send the cursor to the position of "apple"
	SetConsoleTextAttribute(h, 12); // set color to red to draw "apple"
	putchar(apple); // display the apple symbol

	c.X = array_X[0]; // we associate the coordinate object with the starting position of the "snake"
	c.Y = array_Y[0];
	SetConsoleCursorPosition(h, c); // send the cursor to the position of the head of the "snake"
	SetConsoleTextAttribute(h, 10); // set green color to draw "snake"
	putchar(head); // display the head symbol "snake"

	bool flag = true; // variable to control the course of the loop
	do // actual game cycle
	{
		Sleep(sleep_time); // delaying the program flow by a previously specified interval

		if (_kbhit()) // check if any key was pressed and start processing it if TRUE
		{
			int k = _getch(); // read key code from buffer
			if (k == 0 || k == 224) // if the first code is auxiliary, read the second code
				k = _getch();
			switch (k) // pass the code of the pressed key inside the select statement
			{
			case 80: // if the down key was pressed
				dy = 1; // then the increment along the y-axis will be positive
				dx = 0; // zero increment on the abscissa
				break;
			case 72: // if up
				dy = -1; // similar according to geometric logic
				dx = 0;
				break;
			case 75: // if left
				dy = 0;
				dx = -1;
				break;
			case 77: // if right
				dy = 0;
				dx = 1;
				break;
			case 27: // if the ESC key was pressed
				flag = false; // set the flag to FALSE to end the motion display
				break;
			}
		}


		int X = array_X[length - 1] + dx; // determine the value of the abscissa of the "snake" head after the offset
		int Y = array_Y[length - 1] + dy; // same for ordinate
		if (X == 0 || X == width - 1 || Y == 0 || Y == height - 1) // field boundary check
		{
			flag = false; // for now - just setting the loop control variable
		}
		else if (X == X_apple && Y == Y_apple) // apple achievement check
		{
			c.X = array_X[length - 1]; // setting the coordinates of the "snake" head position into the object
			c.Y = array_Y[length - 1];
			SetConsoleCursorPosition(h, c); // setting the cursor to this position
			putchar(snake); // display body symbol "snake"

			length++; // increase in the length of the "snake" (apple swallowed)
			c.X = array_X[length - 1] = X; // setting the positions of the new "snake" link in the arrays
			c.Y = array_Y[length - 1] = Y;
			SetConsoleCursorPosition(h, c); // placing cursor there
			putchar(head); // and displaying the "snake" head symbol there

			if (length == max_length) // checking if the length of the "snake" has reached its maximum value
			{
				break; // for now - just break the loop
			}

			int i; // variable for counting the number of snake links that do not match the position of the "apple"
			do
			{
				X_apple = rand() % (width - 2) + 1; // setting new coordinates of the "apple"
				Y_apple = rand() % (height - 2) + 1;
				i = 0; // resetting the number of mismatched coordinates
				for (; i < length; i++) // running a cycle for checking matches
					if (X_apple == array_X[i] && Y_apple == array_Y[i]) // if a match is found
						break; // then we break the for loop
			} while (i < length); // the search for new coordinates continues as long as the number of mismatched coordinates is less than the length of the "snake"

			c.X = X_apple; // setting the coordinates of the new correct position of the "apple" to the object
			c.Y = Y_apple;
			SetConsoleCursorPosition(h, c); // send cursor there
			SetConsoleTextAttribute(h, 12); // setting color to red
			putchar(apple); // display the symbol "apple"
			SetConsoleTextAttribute(h, 10); // reverse setting the color to green - for further display of the "snake"
		}
		else // the case when the head of the "snake" was in a new empty position
		{
			int i = 1; // variable for the number of links that do not match the new position - except for the tail of the "snake"
			for (; i < length; i++)
				if (X == array_X[i] && Y == array_Y[i]) // if a match is found in the loop - break
					break;
			if (i < length) // if the number of mismatched links is less than the length of the "snake" - then we interrupt the main game loop
			{
				break;
			}
			else //otherwise, we start processing the "snake" shift
			{
				c.X = array_X[0]; // set the position of the "snake" tail to the coordinate object
				c.Y = array_Y[0];
				SetConsoleCursorPosition(h, c); // move cursor there
				putchar(' '); // and display a space (tail grout)

				if (length > 1) // if the length of the snake is greater
				{
					c.X = array_X[length - 1]; // set the previous position of the "snake" head to the coordinate object
					c.Y = array_Y[length - 1];
					SetConsoleCursorPosition(h, c);  // move cursor there
					putchar(snake); // display the body symbol "snake"
				}

				for (int i = 0; i < length - 1; i++) // we start the cycle of shifting the coordinates of the "snake" links
				{
					array_X[i] = array_X[i + 1]; // we process all links - except for the last one
					array_Y[i] = array_Y[i + 1];
				}

				c.X = array_X[length - 1] = X; // set a new position of the "snake" head
				c.Y = array_Y[length - 1] = Y;
				SetConsoleCursorPosition(h, c); // move cursor there
				putchar(head); // display the head symbol "snake"
			}
		}
	} while (flag); // exit the loop if the control variable is reset
	system("cls"); // clear the screen
	cout << "GAME OVER\n"; // display a message about the end of the game
	system("pause");
}
