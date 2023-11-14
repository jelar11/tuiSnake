#include "snake.h"

#include <stdlib.h>

#include "color_idx.h"

Snake::Snake(Point startPos)
{
    pos = startPos;
    dir = None;
    tailLength = 5;
}

void Snake::ChangeDirection(Direction newDir)
{
    if (newDir == Up && dir != Down)
        dir = newDir;
    else if (newDir == Down && dir != Up)
        dir = newDir;
    else if (newDir == Left && dir != Right)
        dir = newDir;
    else if (newDir == Right && dir != Left)
        dir = newDir;
    else
        dir = newDir;
}

void Snake::Update()
{
    switch (dir)
    {
        case Up:
            pos.y--;
            break;
        case Down:
            pos.y++;
            break;
        case Left:
            pos.x--;
            break;
        case Right:
            pos.x++;
            break;
        default:
            break;
    }
    if (dir != None)
    {
        UpdateTail();

        SelfCollision();
    }
}

void Snake::UpdateTail()
{
    tail.push_back(pos);
    if ((int)tail.size() > tailLength)
        tail.pop_front();
}

void Snake::Draw(WINDOW *win)
{
    wbkgdset(win, COLOR_PAIR(C_SNAKE));
    for (size_t i = 0; i < tail.size(); i++)
    {
        mvwprintw(win, tail[i].y, tail[i].x, "o");
    }

    wbkgdset(win, COLOR_PAIR(C_SNAKE_HEAD));
    mvwprintw(win, pos.y, pos.x, "O");

    wbkgdset(win, COLOR_PAIR(C_DEFAULT));
}

void Snake::Grow()
{
    tailLength++;
}

Point Snake::GetPosition()
{
    return pos;
}

bool Snake::IsPositionUsed(Point position)
{
    for (size_t i = 0; i < tail.size(); i++)
    {
        const Point t = tail[i];
        if (t.x == position.x && t.y == position.y)
            return true;
    }

    if (pos.x == position.x && pos.y == position.y)
        return true;

    return false;
}

void Snake::SelfCollision()
{
    for (size_t i = 0; i < tail.size(); i++)
    {
        const Point t = tail[i];
        int x = std::abs(pos.x - t.x);
        int y = std::abs(pos.y - t.y);
        if ((x == 0) && (y == 0) && (i != tail.size() - 1))
        {
            dir = None;
            return;
        }
    }
}
