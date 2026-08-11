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
    Snake(int initX, int initY, int length = 3, Direction initDir = RIGHT) {
        dir = initDir;
        for (int i = 0; i < length; ++i) {
            switch (dir) {
                case LEFT:  body.push_back({initX + i, initY}); break;
                case RIGHT: body.push_back({initX - i, initY}); break;
                case UP:    body.push_back({initX, initY + i}); break;
                case DOWN:  body.push_back({initX, initY - i}); break;
                default:    body.push_back({initX - i, initY}); break;
            }
        }
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
    Snake snake1;
    Snake snake2;
    Food food;
    int width, height;
    int score1, score2;
    bool gameOver;

public:
    Game(int w=25, int h=20) : snake1(4, h/2, 3, RIGHT), snake2(w-5, h/2, 3, LEFT), food(w, h), width(w), height(h) {
        score1 = 0; score2 = 0; gameOver = false;
        food.spawn(forbidden());
    }

    vector<pair<int,int>> forbidden() {
        vector<pair<int,int>> body = snake1.getBody();
        vector<pair<int,int>> body2 = snake2.getBody();
        body.insert(body.end(), body2.begin(), body2.end());
        return body;
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
                for (auto b : snake1.getBody()) {
                    if (b.first == x && b.second == y) {
                        setColor(2); // Green snake
                        cout << "O";
                        snakePart = true;
                        break;
                    }
                }
                if (!snakePart) {
                    for (auto b : snake2.getBody()) {
                        if (b.first == x && b.second == y) {
                            setColor(11); // Cyan snake
                            cout << "X";
                            snakePart = true;
                            break;
                        }
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
    cout << "P1 Score: " << score1 << "   P2 Score: " << score2 << endl;
    if (gameOver) {
        setColor(4); // Red for Game Over
        if (score1 > score2) cout << "GAME OVER! Player 1 wins.\n";
        else if (score2 > score1) cout << "GAME OVER! Player 2 wins.\n";
        else cout << "GAME OVER! Draw.\n";
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
                    case 72: snake2.setDirection(UP); break;     // UP
                    case 80: snake2.setDirection(DOWN); break;   // DOWN
                    case 75: snake2.setDirection(LEFT); break;   // LEFT
                    case 77: snake2.setDirection(RIGHT); break;  // RIGHT
                }
            } else {
                switch (ch) {
                    case 'a': case 'A': snake1.setDirection(LEFT); break;
                    case 'd': case 'D': snake1.setDirection(RIGHT); break;
                    case 'w': case 'W': snake1.setDirection(UP); break;
                    case 's': case 'S': snake1.setDirection(DOWN); break;
                    case 'r': case 'R': if (gameOver) restart(); break;
                    case 'x': case 'X': gameOver = true; break;
                }
            }
        }
    }

    void logic() {
        if (gameOver) return;

        snake1.move(); // always move
        snake2.move();

        // Check if food eaten
        bool eat1 = snake1.getHead() == food.getPos();
        bool eat2 = snake2.getHead() == food.getPos();
        if (eat1) score1++;
        if (eat2) score2++;
        if (eat1 || eat2) {
            if (eat1) snake1.move(true);
            if (eat2) snake2.move(true);
            food.spawn(forbidden());
        }

        auto head1 = snake1.getHead();
        auto head2 = snake2.getHead();
        bool hit = head1.first <= 0 || head1.first >= width-1 || head1.second <= 0 || head1.second >= height-1 || snake1.hitSelf();
        hit = hit || head2.first <= 0 || head2.first >= width-1 || head2.second <= 0 || head2.second >= height-1 || snake2.hitSelf();
        auto body2 = snake2.getBody();
        for (size_t i = 1; i < body2.size(); ++i)
            if (body2[i] == head1) hit = true;
        auto body1 = snake1.getBody();
        for (size_t i = 1; i < body1.size(); ++i)
            if (body1[i] == head2) hit = true;
        if (hit) {
            gameOver = true;
        }
    }

    void restart() {
        snake1 = Snake(4, height/2, 3, RIGHT);
        snake2 = Snake(width-5, height/2, 3, LEFT);
        score1 = 0;
        score2 = 0;
        food.spawn(forbidden());
        gameOver = false;
    }

    void run() {
    system("cls");
    setColor(11);
    cout << "===================================================================\n";
    cout << "                        WELCOME TO SNAKE GAME\n";
    cout << "===================================================================\n";
    setColor(15);
    cout << "Controls: W/A/S/D for P1 | Arrow Keys for P2 | R to restart | X to exit\n";
    cout << "------------------------------------------------------------------\n";
    cout << "Press any key to start...";
    _getch();

    clearScreen(); // move below instructions

    while (true) {
        draw();       // draw grid starting below line 4
        if (gameOver) {
            cout << "Press R to Restart or X to Exit...\n";
            if (_kbhit()) {
                char ch = _getch();
                if (ch == 'r' || ch == 'R') restart();
                else if (ch == 'x' || ch == 'X') return;
            }
            Sleep(100);
            continue;
        }

        input();      // handle keys
        logic();      // update snake + collisions

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
