#pragma once

#include <ncurses.h>

#include "board.h"
#include "point.h"
#include "snake.h"

class Game
{
public:
    Game();
    void Execute();

private:
    bool OnInit();
    bool OnEvent();
    void OnUpdate();
    void OnRender();
    void OnShutdown();

    bool IsPositionUsed(Point pos);
    Point RandomPosition();

    void NewApple();
    void DrawApple();

    void NewRock();
    void DrawRocks();
    void CollisionDetection();

    Point apple;
    std::deque<Point> rocks;

    int screen_lines, screen_char_pr_line;

    WINDOW *winBoard;

    Board *board;
    Snake *snake;
    bool pause;

    short COLOR_GRAY;
    long DarkSlateGray2;

    int delayCounter;
};
