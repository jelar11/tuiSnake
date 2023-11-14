#pragma once

struct Point
{
    Point()
    {
        x = -1;
        y = -1;
    }
    Point(int pX, int pY)
    {
        x = pX;
        y = pY;
    }
    int x, y;
};
