#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
using namespace std;

// Snake direction
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

class Snake {
private:
    vector<pair<int, int>> body; // (x, y) positions
    Direction dir;
public:
    Snake(int initX, int initY, int length = 3) {
        dir = RIGHT;
        for (int i = 0; i < length; ++i)
            body.push_back({initX - i, initY}); // Horizontal start
    }
    void setDirection(Direction d) {
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

class Food {
private:
    pair<int,int> pos;
    int width, height;
public:
    Food(int w, int h) : width(w), height(h) { spawn(vector<pair<int,int>>()); }
    void spawn(const vector<pair<int,int>>& forbidden) {
        while (true) {
            int x = rand() % width;
            int y = rand() % height;
            bool bad = false;
            for (auto s : forbidden)
                if (s.first == x && s.second == y) { bad = true; break; }
            if (!bad) { pos = {x, y}; break; }
        }
    }
    pair<int,int> getPos() { return pos; }
};

class Game {
private:
    Snake snake;
    Food food;
    int width, height, score;
    bool gameOver;
public:
    Game(int w=20, int h=20) : snake(w/2, h/2), food(w, h), width(w), height(h) {
        score = 0; gameOver = false;
    }
    void draw() {
        system("cls");
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (x == 0 || x == width-1 || y == 0 || y == height-1)
                    cout << "#";
                else if (x == food.getPos().first && y == food.getPos().second)
                    cout << "F";
                else {
                    bool snakePart = false;
                    for (auto b : snake.getBody())
                        if (b.first == x && b.second == y) {snakePart=true;break;}
                    cout << (snakePart ? "O" : " ");
                }
            }
            cout << "\n";
        }
        cout << "Score: " << score << endl;
        if (gameOver) cout << "GAME OVER! Press R to restart or X to exit.\n";
    }
    void input() {
        if (_kbhit()) {
            switch (_getch()) {
                case 'a': snake.setDirection(LEFT); break;
                case 'd': snake.setDirection(RIGHT); break;
                case 'w': snake.setDirection(UP); break;
                case 's': snake.setDirection(DOWN); break;
                case 'r': if (gameOver) restart(); break;
                case 'x': gameOver = true; break;
            }
        }
    }
    void logic() {
        if (gameOver) return;
        pair<int,int> prevHead = snake.getHead();
        snake.move(prevHead == food.getPos());
        if (prevHead == food.getPos()) {
            score++;
            food.spawn(snake.getBody());
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
        while (true) {
            draw();
            input();
            logic();
            if (gameOver) {
                Sleep(300);
                continue;
            }
            Sleep(100);
        }
    }
};

int main() {
    srand((unsigned)time(0));
    Game game(20, 20);
    game.run();
    return 0;
}
