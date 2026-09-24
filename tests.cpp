#include <gtest/gtest.h>

int fake_rand_val = 0;
int fake_rand() { return fake_rand_val; }

#define GET_RAND fake_rand
#define main game_main
#include "snake.cpp"
#undef main

TEST(SnakeTest, MoveTest) {
    Snake s(10, 10, 3);
    s.setDirection(RIGHT);
    s.move();
    EXPECT_EQ(s.getHead().first, 11);
}

TEST(SnakeTest, CollideSelfTest) {
    Snake s(10, 10, 5);
    s.setDirection(LEFT); // Can't go LEFT if it's already facing RIGHT, but let's just test hitSelf
    EXPECT_FALSE(s.hitSelf());
}

TEST(GameTest, ScoreIncreasesRunsButCannotAssert) {
    Game g;
    // g.logic(); // We could call it if score wasn't private
}

TEST(FoodTest, SpawnsCorrectly) {
    Food f(20, 20);
    fake_rand_val = 5; 
    vector<pair<int,int>> empty_snake;
    f.spawn(empty_snake);
    // x = 1 + fake_rand() % (width - 2) = 1 + 5 % 18 = 6
    // y = 1 + fake_rand() % (height - 2) = 1 + 5 % 18 = 6
    EXPECT_EQ(f.getPos().first, 6);
    EXPECT_EQ(f.getPos().second, 6);
}

TEST(BadTest, HardCodedDelaySmell) {
    // This is an example for Part F: Hard-Coded Delay
    // Sleep(100);
    EXPECT_TRUE(true);
}

TEST(BadTest, NoAssertionSmell) {
    // Example for Part F: No Assertion
    int a = 5;
    a++;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
