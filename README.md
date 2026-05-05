# 🐍 Snake Game (C++ Console Project)...

A classic **Snake Game** implemented in **C++**, using **Object-Oriented Programming (OOP)** and **Windows console graphics**.  
The player controls a snake that grows when it eats food. The game ends when the snake collides with the boundary or itself.  
Not only project also teaches the fundamentals of OOPs while building
OOP is not bounded just to **inheritance**, abstraction, encapsulation, and **polymorphisms** it is just first step in OOP
---

## 📁 Project Overview

- **Language:** C++  
- **Environment:** Windows Console  
- **Concepts Used:**  
  - Classes and Objects (Snake, Food, Game)  
  - Encapsulation  
  - Loops and Conditionals  
  - Dynamic Data Structures (`vector<pair<int,int>>`)  
  - Randomized Food Spawning  
  - Keyboard Input (`_kbhit()` and `_getch()`)  
  - Console Coloring and Cursor Control (from `windows.h`)  

---

## ⚙️ A. For Developers (Extending the Game)

If you want to **extend or enhance** this project, this section explains the code structure and possible improvements.

### 🧩 1. Project Structure

| Class | Responsibility |
|--------|----------------|
| **Snake** | Handles the snake’s body, direction, and movement logic. |
| **Food** | Spawns food randomly on the grid, avoiding overlap with the snake. |
| **Game** | Manages the main game loop, rendering, keyboard input, and collision detection. |

---

### 🧱 2. Key Methods and Their Roles

| Method | Description |
|---------|-------------|
| `Snake::move()` | Moves the snake one step in the current direction. |
| `Snake::hitSelf()` | Checks if the snake’s head collides with its body. |
| `Food::spawn()` | Generates food in a random valid cell (not on the snake). |
| `Game::draw()` | Renders the game board, snake, and food using colored output. |
| `Game::logic()` | Updates positions, handles eating and collision detection. |
| `Game::input()` | Captures keyboard input (W/A/S/D or arrow keys). |
| `Game::run()` | Main loop that continuously runs draw → input → logic. |

---

### 🧠 3. Ideas for Future Enhancements

#### 🧱 (a) Add Obstacles
- Maintain a list of obstacles (`vector<pair<int,int>> obstacles`).
- Draw them in `draw()` and check collision in `logic()`.

#### 🌟 (b) Add Special Food
- Derive a `SpecialFood` class from `Food`.
- Add timers or point multipliers.
- Spawn it occasionally for extra rewards.

#### 🚀 (c) Add Levels or Speed Increase
Increase speed as the player’s score grows:
```cpp
if (score % 5 == 0 && speed > 30) speed -= 10;
Use Sleep(speed); to adjust difficulty dynamically.

#### 💾 (d) Save High Score to File

Add file I/O:
ofstream out("highscore.txt");
out << highScore;
and load it at the start of the game.

#### 🔧 4. Design Principles

Encapsulation: Each class manages its own data and logic.

Modularity: Game flow is divided into clear methods (draw, input, logic).

Extensibility: Easy to add new features (obstacles, levels, food types).

## 🎮 B. For Users (How to Play)

This section helps users run and play the game on their system.

### 🖥️ 1. System Requirements

Operating System: Windows (required for <conio.h> and <windows.h>)

Compiler: Any C++ compiler (MinGW, Code::Blocks, Visual Studio, etc.)

💡 On macOS/Linux: You’ll need to replace Windows-specific libraries (conio.h, windows.h)
with cross-platform alternatives like ncurses. Colors and controls may differ slightly.

⚙️ 2. How to Compile and Run

Option 1 – Command Line (Windows):

g++ snake_game.cpp -o snake_game
snake_game


Option 2 – Using an IDE:

Create a new C++ project.

Paste the game code into the main file.

Press Run or Build & Execute.

🕹️ 3. Controls
Key	Action
W / ↑	Move Up
S / ↓	Move Down
A / ←	Move Left
D / →	Move Right
R	Restart (after Game Over)
X	Exit Game
🎯 4. Gameplay Rules

The snake starts with 3 blocks.

Each time it eats food (F), it grows by one and your score increases.

The game ends if the snake hits the wall or its own body.

Your high score is tracked during the session.

Upon Game Over, the entire screen turns red, with clear instructions to restart or exit.

🎨 5. Visual Design
Element	Symbol	Color
Snake	O	🟢 Green
Food	F	🔴 Red
Wall	#	🔵 Blue
Scoreboard	Text	🟡 Yellow
Game Over Screen	Full Red	❌
📜 6. Example Output
#########################
#                       #
#     OOO               #
#            F          #
#                       #
#########################
Score: 5   High Score: 8

🧩 Project Author
👨‍💻 Developer: Dhrumil Doshi
🧑‍🏫 Guided by: **Ankush Sir**
📚 Course: M.Sc. (IT) – DAIICT
🎯 Goal: To design a modular, colorful, and extensible Snake Game using C++ OOP concepts.
