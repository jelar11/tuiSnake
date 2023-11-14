#pragma once

#include <ncurses.h>

#include <stack>

#include "point.h"

enum Direction
{
    None,
    Right,
    Down,
    Left,
    Up
};

class Snake
{
public:
    Snake(Point startPos);

    void ChangeDirection(Direction newDir);
    void Update();
    void Draw(WINDOW *window);
    void Grow();

    Point GetPosition();
    bool IsPositionUsed(Point pos);

private:
    Point pos;
    Direction dir;
    std::deque<Point> tail;
    int tailLength;

    void UpdateTail();
    void SelfCollision();
};
