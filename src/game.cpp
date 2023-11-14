#include "game.h"

#include <stdlib.h> /* srand, rand */
#include <time.h>
#include <unistd.h>  // usleep

#include "color_idx.h"

Game::Game()
{
}

void Game::Execute()
{
    pause = false;
    bool quit = !OnInit();

    srand(time(NULL));
    // board = new Board(winBoard);
    snake = new Snake(Point(screen_char_pr_line / 2, screen_lines / 2));

    NewApple();
    NewRock();

    while (!quit)
    {
        quit = OnEvent();

        OnUpdate();

        OnRender();
    }

    OnShutdown();
}

bool Game::OnInit()
{
    // Initialization
    initscr();              // new screen will be created
    nodelay(stdscr, TRUE);  // getch is non blocking
    keypad(stdscr, TRUE);   // We get special keys F1, F2, arrow etc.
    noecho();
    curs_set(FALSE);  // Hide the cursor
    raw();            // Line buffering disabled

    getmaxyx(stdscr, screen_lines, screen_char_pr_line);  // get the number of rows and columns

    screen_lines -= 10;
    screen_char_pr_line -= 50;

    // Init HUD and Board window
    // hud = newwin(HUD_HEIGHT, screen_char_pr_line, 0, 0);
    // winBoard = newwin(screen_lines - HUD_HEIGHT, screen_char_pr_line, HUD_HEIGHT, 0);
    winBoard = newwin(screen_lines, screen_char_pr_line, 5, 10);

    if (has_colors() == FALSE)
    {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();

    init_pair(C_DEFAULT, COLOR_WHITE, COLOR_BLACK);
    init_pair(C_SNAKE, COLOR_GREEN, COLOR_BLACK);
    init_pair(C_SNAKE_HEAD, COLOR_YELLOW, COLOR_BLACK);
    init_pair(C_APPLE, COLOR_RED, COLOR_BLACK);
    // init_color(COLOR_GRAY,163, 163, 163);
    init_pair(C_ROCK, COLOR_WHITE, COLOR_BLACK);
    init_color(C_PaleGreen3, 372, 843, 372);

    return true;
}

bool Game::OnEvent()
{
    bool quit = false;
    // quit = ui->OnEvent();

    const int ch = getch();
    switch (ch)
    {
        // case 27:    // Esc
        case 'q':
        case 'e':
            quit = true;
            break;
        case KEY_UP:
            snake->ChangeDirection(Up);
            break;
        case KEY_DOWN:
            snake->ChangeDirection(Down);
            break;
        case KEY_LEFT:
            snake->ChangeDirection(Left);
            break;
        case KEY_RIGHT:
            snake->ChangeDirection(Right);
            break;
        case ' ':
            pause = !pause;
            break;
        case 'n':
            NewApple();
            break;
    }

    return quit;
}

void Game::OnUpdate()
{
    if ((delayCounter % 10) == 0)
    {
        delayCounter = 0;
        if (!pause)
        {
            // board->Update();
            snake->Update();
            CollisionDetection();
        }
    }
    delayCounter++;
    usleep(5000);
}

void Game::OnRender()
{
    wclear(winBoard);
    box(winBoard, 0, 0);
    // board->Draw();

    snake->Draw(winBoard);

    DrawApple();
    DrawRocks();

    mvwprintw(winBoard, 1, 1, "(s) %d, %d", snake->GetPosition().x, snake->GetPosition().y);

    wrefresh(winBoard);
    refresh();
}

void Game::OnShutdown()
{
    delwin(winBoard);

    endwin();
}

bool Game::IsPositionUsed(Point pos)
{
    for (size_t i = 0; i < rocks.size(); i++)
    {
        const Point r = rocks[i];
        if (r.x == pos.x && r.y == pos.y)
            return true;
    }

    return false;
}

Point Game::RandomPosition()
{
    Point res = Point(-1, -1);
    int retry = 0;
    do
    {
        const int x = rand() % screen_char_pr_line - 1;
        const int y = rand() % screen_lines - 1;
        const Point p = Point(x, y);

        if ((p.x > 0 && p.x < screen_char_pr_line - 1) && (p.y > 0 && p.y < screen_lines - 1) && !IsPositionUsed(p) &&
            !snake->IsPositionUsed(p))
        {
            retry = 10;
            res = p;
        }
        retry++;
    }
    while (retry < 9);
    return res;
}

void Game::DrawApple()
{
    wbkgdset(winBoard, COLOR_PAIR(C_APPLE));
    mvwprintw(winBoard, apple.y, apple.x, "*");
    wbkgdset(winBoard, COLOR_PAIR(C_DEFAULT));
}

void Game::NewApple()
{
    apple = RandomPosition();
    NewRock();
}

void Game::NewRock()
{
    rocks.push_back(RandomPosition());
}

void Game::DrawRocks()
{
    wbkgdset(winBoard, COLOR_PAIR(C_ROCK));
    for (size_t i = 0; i < rocks.size(); i++)
    {
        mvwprintw(winBoard, rocks[i].y, rocks[i].x, "#");
    }

    wbkgdset(winBoard, COLOR_PAIR(C_DEFAULT));
}

void Game::CollisionDetection()
{
    const Point s = snake->GetPosition();

    // Within board
    if (s.x < 0 || s.x > screen_char_pr_line - 1 || s.y < 0 || s.y > screen_lines - 1)
    {
        snake->ChangeDirection(None);
    }

    // Rocks collision
    for (size_t i = 0; i < rocks.size(); i++)
    {
        const Point r = rocks[i];
        const int x = std::abs(s.x - r.x);
        const int y = std::abs(s.y - r.y);
        if ((x == 0) && (y == 0))
            snake->ChangeDirection(None);
    }

    // Apple collision
    if (std::abs(s.x - apple.x) <= 0 && std::abs(s.y - apple.y) <= 0)
    {
        snake->Grow();
        NewApple();
    }
}
