#pragma once

#include <ncurses.h>

class Board
{
public:
    explicit Board(WINDOW *window);

    void Draw();
    void Update();

private:
    WINDOW *win;
};
