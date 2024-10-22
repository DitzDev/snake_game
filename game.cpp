#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <vector>
using namespace std;

const int width = 30;
const int height = 20;
int x, y, fruitX, fruitY, score;
bool gameOver;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

struct Segment {
    int x, y;
};
vector<Segment> snake;

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    snake.clear();
    snake.push_back({x, y});
}

void Draw() {
    clear();
    for (int i = 0; i < width + 2; i++)
        printw("#");
    printw("\n");

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                printw("#");
            bool isSnake = false;
            for (const auto& segment : snake) {
                if (segment.x == j && segment.y == i) {
                    printw("O");
                    isSnake = true;
                    break;
                }
            }
            if (!isSnake) {
                if (i == fruitY && j == fruitX)
                    printw("F");
                else
                    printw(" ");
            }
            if (j == width - 1)
                printw("#");
        }
        printw("\n");
    }

    for (int i = 0; i < width + 2; i++)
        printw("#");
    printw("\nScore: %d\n", score);
    refresh();
}

void Input() {
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    int c = getch();
    
    switch (c) {
        case KEY_LEFT: dir = LEFT; break;
        case KEY_RIGHT: dir = RIGHT; break;
        case KEY_UP: dir = UP; break;
        case KEY_DOWN: dir = DOWN; break;
        case 'x': gameOver = true; break;
    }
}

void Logic() {
    Segment prevSegment = snake.front();
    Segment newSegment;

    switch (dir) {
        case LEFT: x--; break;
        case RIGHT: x++; break;
        case UP: y--; break;
        case DOWN: y++; break;
        default: break;
    }

    if (x >= width || x < 0 || y >= height || y < 0) {
        gameOver = true;
        return;
    }
    
    newSegment = {x, y};
    snake.insert(snake.begin(), newSegment);

    if (x == fruitX && y == fruitY) {
        score++;
        fruitX = rand() % width;
        fruitY = rand() % height;
    } else {
        snake.pop_back();
    }
}

int main() {
    srand(time(0));
    initscr();
    noecho();
    curs_set(0);
    Setup();

    int baseDelay = 150000;
    int speedIncreaseFactor = 10000;
    while (!gameOver) {
        Draw();
        Input();
        
        int delay = baseDelay - (score * speedIncreaseFactor);
        delay = (delay < 50000) ? 50000 : delay;
        Logic();
        usleep(delay);
    }

    endwin();
    printw("Game Over! Your score: %d\n", score);
    refresh();
    sleep(2);
    return 0;
}