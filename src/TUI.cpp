#include "TUI.h"

#include <climits>

bool TUI::OnInit()
{
    // Initialization
    initscr();              // new screen will be created
    nodelay(stdscr, TRUE);  // getch is non blocking
    keypad(stdscr, TRUE);   // We get special keys F1, F2, arrow etc.
    noecho();
    curs_set(FALSE);  // Hide the cursor
    raw();            // Line buffering disabled

    getmaxyx(stdscr, screen_lines, screen_char_pr_line);  // get the number of rows and columns

    // Init HUD and Board window
    hud = newwin(HUD_HEIGHT, screen_char_pr_line, 0, 0);
    board = newwin(screen_lines - HUD_HEIGHT, screen_char_pr_line, HUD_HEIGHT, 0);

    return true;
}

bool TUI::OnEvent()
{
    bool quit = false;
    const int ch = getch();
    switch (ch)
    {
        // case 27:    // Esc
        case 'q':
        case 'e':
            quit = true;
            break;
        case 's':
            step = true;
            break;
        case 'r':
        case ' ':
            run = !run;
            break;
        case '+':
            speed += SPEED_STEP;
            break;
        case '-':
            speed -= SPEED_STEP;
            if (speed <= 0)
                speed = 0;
            break;
        case '0':
            speed = SPEED_DEFAULT;
            break;
        default:
            // printf("%d\n", ch);
            break;
    }

    return quit;
}

void TUI::OnUpdate()
{
    static int slow = 0;

    if ((step || run) && (slow >= speed))
    {
        slow = 0;
        if (!run)
            step = false;

        // life->Tick();
        tick++;
    }
    slow++;
    if (slow >= INT_MAX)
        slow = 0;
}

void TUI::OnRender()
{
    wclear(hud);
    box(hud, 0, 0);
    mvwprintw(hud, 1, 1, "Tick: %d", tick);
    mvwprintw(hud, 1, 20, "Speed: %d", speed);
    wrefresh(hud);

    wclear(board);

    // for (int i = 0; i < screen_char_pr_line - 1; i++)
    // {
    //     for (int j = 0; j < (screen_lines - HUD_HEIGHT - 2); j++)
    //     {
    //         if (life->isAlive(i, j))
    //         {
    //             mvwprintw(board, j, i, "#");
    //         }
    //     }
    // }

    wrefresh(board);

    usleep(10000);  // Shorter delay between movements
}

void TUI::OnShutdown()
{
    delwin(hud);
    delwin(board);

    endwin();  // End curses mode
}
