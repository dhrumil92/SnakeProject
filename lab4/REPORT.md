# Lab 4 — Group A04

| | |
|---|---|
| Repository | g:\M.Sc.(IT)\Semester-3\Design and Testing\snake_clone\Snake_Game\SnakeProject_Original |
| Base tag | `lab4-base` at commit head |
| Pull request | To be submitted |

---

## 1. Five rules — [5]

Written before opening the source. Behaviour, with an observable outcome.

| # | Rule |
|---|---|
| 1 | The snake moves one step in its current direction each game tick. |
| 2 | The game ends when a snake's head collides with any part of its own body. |
| 3 | Eating food increases the player's score by exactly one point. |
| 4 | Food never spawns on a cell that a snake currently occupies. |
| 5 | The game pauses and waits for terminal input during the Game Over screen. |

---

## 2. What you could test, and what stopped you — [10]

No source changes in this part. Every `file:line` below is a line in `lab4-base`.

| # | Rule | Test written? | Blocking dependency (`file:line` + what it is) |
|---|---|---|---|
| 1 | The snake moves one step in its current direction each game tick. | Yes | None (Snake class testable) |
| 2 | The game ends when a snake's head collides with any part of its own body. | Yes | None (Snake class testable) |
| 3 | Eating food increases the player's score by exactly one point. | No | `snake.cpp:91` — `score` is private, cannot assert score increase. |
| 4 | Food never spawns on a cell that a snake currently occupies. | No | `snake.cpp:67` — `rand()` called inside `Food::spawn()`, no way to control seed. |
| 5 | The game pauses and waits for terminal input during the Game Over screen. | No | `snake.cpp:152` — `_getch()` blocks indefinitely waiting for user input, cannot supply keys. |

> **Rules testable without modifying the source: 2 / 5**

---

## 3. Coverage, and what it missed — [6]

| | |
|---|---|
| Line coverage | 35 % |
| Branch coverage | 20 % |
| Command used | `g++ --coverage -O0 -g your_game.cpp tests.cpp -o tests` |

**One rule that is executed by the suite but not verified by it:**

| | |
|---|---|
| Rule | Eating food increases the player's score by exactly one point. |
| Line that runs | `snake.cpp:181` (`score++`) |
| The assertion that is missing | `EXPECT_EQ(g.score, 1);` |

---

## 4. The seam — [10]

| | |
|---|---|
| Rule made testable | Food never spawns on a cell that a snake currently occupies. |
| Commit 1 (seam) | Add preprocessing seam for rand() |
| Commit 2 (test) | Add test for Food spawn using fake_rand() |
| Seam kind | preprocessing |
| Enabling point | `snake.cpp:67` |
| What production code gave up | Nothing. The preprocessing macro compiles away to the standard `rand()` in production, so performance and behaviour are identical. |

---

## 5. The double — [4]

| | |
|---|---|
| What you passed through the seam | stub |
| The method under test | `Food::spawn()` |

Was the collaborator asked a question or told to do something, and why does that decide the answer above?
The `rand()` collaborator was asked a question (to return a random integer for coordinates), not told to do something. Since a method that returns a value can be checked by its return value, we used a stub to provide canned answers (hardcoded coordinates).

---

## 6. Two smells in your own tests — [5]

| | Smell | `file:line` | One-line fix |
|---|---|---|---|
| 1 | Expected Exception Smell / No Assertion Smell | `tests.cpp:46` | Add an assertion like `EXPECT_EQ(a, 6);` |
| 2 | Conditional Test Logic / Default Test Smell | `tests.cpp:40` | Remove hardcoded `EXPECT_TRUE(true)` placeholder and add real tests. |

