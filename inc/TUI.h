#pragma once

#include <ncurses.h>
#include <unistd.h>  // usleep

const int SPEED_DEFAULT = 10;
const int SPEED_STEP = 2;

class TUI
{
public:
    bool OnInit();
    bool OnEvent();
    void OnUpdate();
    void OnRender();
    void OnShutdown();

private:
    WINDOW *hud;
    WINDOW *board;

    const int HUD_HEIGHT = 3;

    int screen_lines, screen_char_pr_line;

    int speed = SPEED_DEFAULT;
    bool step = false;
    bool run = false;
    int tick = 0;
};
