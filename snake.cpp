#include <conio.h>
#include <iostream>
#include <unistd.h>
#include <windows.h>

using namespace std;

const int width = 60;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;

enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

bool gameOver;
bool quit; 

void Setup()
{
    gameOver = false;
    quit = false;
    dir = STOP;
    x = width - (width / 2) -1 ;
    y = height - (height / 2);
    srand(time(NULL));
    fruitX = (rand() % (width-1)) + 1;
    fruitY = (rand() % (height-1)) + 1;
    score = 0;
    nTail = 0;
}

void Border() 
{
    system("cls");
    for (int i = 0; i <= width; i++)
        cout << '#';
    cout << endl;
    for (int i = 0; i < height; i++) {
        cout << '#';
        for (int j = 0; j < width-1; j++) {
            if (i == y-1 && j == x)
                cout << "\e[1;32m0\e[1;33m";
            else
                cout << ' ';
        }
        cout << '#';
        if (i < 4)
            cout << "\e[1;30m";
        if (i == 0)
            cout << "\t\t\tCommands:";
        else if (i == 1)
            cout << "\t\tWASD or Arrow Keys to move snake";
        else if (i == 2)
            cout << "\t\tPress x to quit";
        else if (i == 3)
            cout << "\t\tPress FN11 to toggle fullscreen";
        if (i < 4)
            cout << "\e[1;33m";
        cout << endl;
    }
    for (int i = 0; i <= width; i++)
        cout << '#';
    cout << endl;
    for (int i = 0; i <= height; i++)
        cout << "\x1b[A";
    cout << '#';
    cout << "\x1b[s";
}

void Draw()
{
    cout << "\x1b[u";
    for (int i = 1; i <= height; i++) {
        for (int j = 1; j < width; j++) {
            if (i == y && j == x) 
                cout << "\e[1;32m0\e[1;33m";
            else if (i == fruitY && j == fruitX) {
                cout << "\e[1;31mF\e[1;33m";
            }
            else {
                bool print = false;
                for (int k = 0; k < nTail && !print; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "\e[1;32mo\e[1;33m";
                        print = true;
                    }
                }
                if (!print)
                    cout << ' ';
            }
        }
        cout << "\x1b[B";
        cout << "\r#";
    }
    
    cout << endl << "Score: " << score << endl;
}

void Input()
{
    if (_kbhit())
    {
        switch (_getch()) {
            case 'w': 
            case 72:
                dir = UP;
                break;
            case 'a':
            case 75:
                dir = LEFT;
                break;
            case 's':
            case 80:
                dir = DOWN;
                break;
            case 'd':
            case 77:
                dir = RIGHT;
                break;
            case 'x':
                gameOver = true;
                quit = true;
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
            usleep(60000);
            Draw();
            break;
        case RIGHT: 
            x++;
            usleep(60000);
            Draw();
            break;
        case UP:
            y--;
            usleep(60000);
            Draw();
            break;
        case DOWN:
            y++;
            usleep(60000);
            Draw();
            break;
        default:
            break;
    }
    if (x == 0 || x == width || y == 0 || y == height+1) {
        gameOver = true;
        cout << endl << "Out of bounds";
    }

    for (int i = 0; i < nTail && !gameOver; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = true;
            cout << endl << "Watch your tail!";
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
    if (nTail == 100) 
    {
        gameOver = true;
        cout << endl << "You're full! You win!";
    }

}

int main()
{
    bool play = true;
    system("COLOR 0e");

    while (play) 
    {
        cout << "\e[?25l";
        Setup();
        Border();
        
        while (!gameOver)
        {
            Input();
            Logic();
        }

        if (!quit) {
            cout << endl << endl << "Play again? (y/n): " << endl;
        
            bool cont = false;
        
            while (!cont)
            {
                switch (_getch()) {
                    case 'y': 
                        play = true;
                        cont = true;
                        break;
                    case 'n':
                        play = false;
                        cont = true;
                        break;
                    default:
                        break;
                }
            }
        }
        else
            play = false;
    }

    cout << "\e[?25h";
    system("cls");
    cout << "Rip snake" << endl;
    system("COLOR 07");

    return 0;
}