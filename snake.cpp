#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
using namespace std;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

// ------------------------------------
// Snake Class
// ------------------------------------
class Snake {
private:
    vector<pair<int, int>> body;
    Direction dir;
public:
    Snake(int initX, int initY, int length = 3) {
        dir = RIGHT;
        for (int i = 0; i < length; ++i)
            body.push_back({initX - i, initY});
    }
    void setDirection(Direction d) {
        // prevent reversing direction
        if ((dir == LEFT && d != RIGHT) || (dir == RIGHT && d != LEFT) ||
            (dir == UP && d != DOWN) || (dir == DOWN && d != UP))
            dir = d;
    }
    Direction getDirection() { return dir; }
    pair<int,int> getHead() { return body.front(); }
    vector<pair<int,int>> getBody() { return body; }
    void move(bool grow = false) {
        pair<int,int> head = body.front();
        switch (dir) {
            case LEFT:  head.first--;  break;
            case RIGHT: head.first++;  break;
            case UP:    head.second--; break;
            case DOWN:  head.second++; break;
            default: break;
        }
        body.insert(body.begin(), head);
        if (!grow)
            body.pop_back();
    }
    bool hitSelf() {
        pair<int,int> head = body.front();
        for(size_t i=1;i<body.size();i++)
            if(body[i]==head) return true;
        return false;
    }
};

// ------------------------------------
// Food Class
// ------------------------------------
class Food {
private:
    pair<int,int> pos;
    int width, height;
public:
    Food(int w, int h) : width(w), height(h) { spawn(vector<pair<int,int>>()); }
    void spawn(const vector<pair<int,int>>& forbidden) {
        while (true) {
            // int x = rand() % width;
            // int y = rand() % height;
            int x = 1 + rand() % (width - 2);
			int y = 1 + rand() % (height - 2);

            bool bad = false;
            for (auto s : forbidden)
                if (s.first == x && s.second == y) { bad = true; break; }
            if (!bad) { pos = {x, y}; break; }
        }
    }
    pair<int,int> getPos() { return pos; }
};

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// ------------------------------------
// Game Class
// ------------------------------------
class Game {
private:
    Snake snake;
    Food food;
    int width, height;
    int score, highScore;
    bool gameOver;

public:
    Game(int w=25, int h=20) : snake(w/2, h/2), food(w, h), width(w), height(h) {
        score = 0; highScore = 0; gameOver = false;
    }

    void clearScreen() {
    	COORD coord = {0, 5};  // move cursor BELOW instructions
    	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}


    void draw() {
    clearScreen();
    
    // Border + Snake + Food Drawing
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == 0 || x == width-1 || y == 0 || y == height-1) {
                setColor(1); // Blue border
                cout << "#";
            }
            else if (x == food.getPos().first && y == food.getPos().second) {
                setColor(4); // Red food
                cout << "F";
            }
            else {
                bool snakePart = false;
                for (auto b : snake.getBody()) {
                    if (b.first == x && b.second == y) {
                        setColor(2); // Green snake
                        cout << "O";
                        snakePart = true;
                        break;
                    }
                }
                if (!snakePart) {
                    setColor(15); // Default white
                    cout << " ";
                }
            }
        }
        cout << "\n";
    }

    // Scoreboard text
    setColor(6); // Yellow
    cout << "Score: " << score << "   High Score: " << highScore << endl;
    if (gameOver) {
        setColor(4); // Red for Game Over
        cout << "GAME OVER!  Press R to restart or X to exit.\n";
    }

    	setColor(15); // Reset color to white
	}


    void input() {
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 224) { // arrow keys
                ch = _getch();
                switch (ch) {
                    case 72: snake.setDirection(UP); break;     // UP
                    case 80: snake.setDirection(DOWN); break;   // DOWN
                    case 75: snake.setDirection(LEFT); break;   // LEFT
                    case 77: snake.setDirection(RIGHT); break;  // RIGHT
                }
            } else {
                switch (ch) {
                    case 'a': case 'A': snake.setDirection(LEFT); break;
                    case 'd': case 'D': snake.setDirection(RIGHT); break;
                    case 'w': case 'W': snake.setDirection(UP); break;
                    case 's': case 'S': snake.setDirection(DOWN); break;
                    case 'r': case 'R': if (gameOver) restart(); break;
                    case 'x': case 'X': gameOver = true; break;
                }
            }
        }
    }

    void logic() {
        if (gameOver) return;

        snake.move(); // always move

        // Check if food eaten
        if (snake.getHead() == food.getPos()) {
            score++;
            if (score > highScore) highScore = score;
            food.spawn(snake.getBody());
            snake.move(true);
        }

        auto head = snake.getHead();
        if (head.first <= 0 || head.first >= width-1 || head.second <= 0 || head.second >= height-1 || snake.hitSelf()) {
            gameOver = true;
        }
    }

    void restart() {
        snake = Snake(width/2, height/2);
        score = 0;
        food.spawn(snake.getBody());
        gameOver = false;
    }

    void run() {
    system("cls");
    setColor(11);
    cout << "===================================================================\n";
    cout << "                        WELCOME TO SNAKE GAME\n";
    cout << "===================================================================\n";
    setColor(15);
    cout << "Controls: W/A/S/D or Arrow Keys to move | R to restart | X to exit\n";
    cout << "------------------------------------------------------------------\n";
    cout << "Press any key to start...";
    _getch();

    clearScreen(); // move below instructions

    while (true) {
        draw();       // draw grid starting below line 4
        input();      // handle keys
        logic();      // update snake + collisions

        if (gameOver) {
            draw();
            Sleep(300);
            cout << "Press R to Restart or X to Exit...\n";
            while (true) {
                if (_kbhit()) {
                    char ch = _getch();
                    if (ch == 'r' || ch == 'R') { restart(); break; }
                    else if (ch == 'x' || ch == 'X') return;
                }
            }
        }

        Sleep(100); // game speed
    }
}

};

// ------------------------------------
// Main Function
// ------------------------------------
int main() {
    srand((unsigned)time(0));
    Game game(25, 20);
    game.run();
    return 0;
}
