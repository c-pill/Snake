#include <conio.h>
#include <iostream>
#include <unistd.h>

using namespace std;

const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;

enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

bool gameOver;

void Setup()
{
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height /2;
    srand(time(NULL));
    fruitX = (rand() % (width-1)) + 1;
    fruitY = (rand() % (height-1)) + 1;
    score = 0;
    nTail = 0;
}

void Draw()
{
    system("cls");
    for (int i = 0; i <= width; i++) 
        cout << '#';
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j <= width; j++) {
            if (j == 0 || j == width)
                cout << '#';
            else if (i == y && j == x)
                cout << '0';
            else if (i == fruitY && j == fruitX)
                cout << 'F';
            else {
                bool print = false;
                for (int k = 0; k < nTail && !print; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << 'o';
                        print = true;
                    }
                }
                if (!print)
                    cout << ' ';
            }
        }
        cout << endl;
    }

    for (int i = 0; i <= width; i++)
        cout << '#';
    
    cout << endl << "Score: " << score << endl;
}

void Input()
{
    if (_kbhit())
    {
        switch (_getch()) {
            case 'w': 
                dir = UP;
                break;
            case 'a':
                dir = LEFT;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'x':
                gameOver = true;
                break;
            default:
                break;
        }
    }
}

void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch(dir) {
        case LEFT:
            x--;
            usleep(100000);
            Draw();
            break;
        case RIGHT: 
            x++;
            usleep(100000);
            Draw();
            break;
        case UP:
            y--;
            usleep(100000);
            Draw();
            break;
        case DOWN:
            y++;
            usleep(100000);
            Draw();
            break;
        default:
            break;
    }
    if (x == 0 || x == width || y == 0 || y == height) {
        gameOver = true;
        cout << "Out of bounds";
    }

    for (int i = 0; i < nTail && !gameOver; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = true;
            cout << "Watch your tail!";
        }
    }
    if (x == fruitX && y == fruitY)
    {
        score++;
        srand(time(NULL));
        fruitX = (rand() % (width-1)) + 1;
        fruitY = (rand() % (height-1)) + 1;
        nTail++;
    }
}

int main()
{
    bool play = true;
    char option;
    while (play) {
        Setup();
        Draw();
        while (!gameOver)
        {
            Input();
            Logic();
        }
        cout << endl << "Play again? (y/n): ";
        cin >> option;
        play = option == 'y' ? true : false;
    }
    system("cls");

    return 0;
}