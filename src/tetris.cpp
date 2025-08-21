#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <utility>
#include <time.h>
#include <random>

TCHAR figure = 219;

#define _SIZE_OF_SHAPES_ 7

#define BEGIN_X          2
#define BEGIN_Y          25/2

#ifdef _UNICODE
typedef wchar_t TCHAR;
#else
typedef char TCHAR;
#endif

constexpr int HEIGHT{ 25 };
constexpr int WIDTH{ 50/2 };

int map[::HEIGHT][::WIDTH];

std::vector<std::pair<int, int>> coordinates_base;

::std::vector<std::pair<int, int>> base_rectangle;
::std::vector<std::pair<int, int>> base_square;
::std::vector<std::pair<int, int>> base_l_shape;
::std::vector<std::pair<int, int>> base_t_shape;
::std::vector<std::pair<int, int>> base_z_shape;
::std::vector<std::pair<int, int>> base_s_shape;

::std::vector<int> rand_arr;

wint_t next_index_figure = 0;

int x = BEGIN_X, y = BEGIN_Y;
int count_x = x;
int object_x = 0, object_y = 0;

int last_x = -1, last_y = -1;

static wint_t test_counting = 0;

clock_t lastTime = 0;
int StopObject = 50;
long NOWTime = clock();

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

enum ShapeColor {
    COLOR_CIRCLE = FOREGROUND_RED | FOREGROUND_INTENSITY, // Red
    COLOR_RECTANGLE = FOREGROUND_GREEN | FOREGROUND_INTENSITY, // Green
    COLOR_SQUARE = FOREGROUND_BLUE | FOREGROUND_INTENSITY, // Blue
    COLOR_L_SHAPE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, // Yellow
    COLOR_T_SHAPE = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY, // Magenta
    COLOR_Z_SHAPE = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // Cyan
    COLOR_S_SHAPE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY // White
};

char CIRCLE = '0';
::std::vector<char> RECTANGLE = {
    '0','0',
    '0','0',
    '0','0',
    '0','0'
};

const int choice_shapes[_SIZE_OF_SHAPES_] = { 0, 1, 2, 3, 4, 5, 6 };
VOID PRINT_PARAMETERS_FOR_MENU(VOID);

void change_mode_CIRCLE(VOID);
BOOL stop_obj(VOID);

VOID INIT() {
    for (wint_t i = 0; i < HEIGHT; i++) {
        for (wint_t j = 0; j < WIDTH; j++) {
            ::map[i][j] = (
                i == 0 ||
                j == 0 ||
                i == HEIGHT - 1 ||
                j == WIDTH - 1
                ) ? L'#' : L' ';
        }
    }
    map[15][25] = '-';
}

BOOL stop_obj_CIRCLE(BOOL);

BOOL DRAW_CIRCLE() {
    BOOL GAME_OVER = FALSE;
    COORD cursorPos = { 0, 0 };
    SetConsoleCursorPosition(hConsole, cursorPos);

    SetConsoleTextAttribute(hConsole, COLOR_CIRCLE);

    for (wint_t i = 0; i < HEIGHT; i++) {
        for (wint_t j = 0; j < WIDTH; j++) {
            putchar(map[i][j]);
        }
        putchar(L'\n');
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    PRINT_PARAMETERS_FOR_MENU();

    printf("x = %i; y = %i\n", x, y);
    printf("last_x = %i; last_y = %i\n", last_x, last_y);

    for (int i = 0; i != coordinates_base.size(); i++) {
        if (BEGIN_X == (coordinates_base[i].first - 1) && BEGIN_Y == coordinates_base[i].second) {
            GAME_OVER = TRUE;
            return GAME_OVER;
        }
    }

    if (map[x + 1][y] == figure || map[x + 1][y] == L'#') {
        printf("SIGNAL: STOPPING");
        stop_obj_CIRCLE(true);
        change_mode_CIRCLE();
        return TRUE;
    }

    BOOL is_C = 0;
    if (stop_obj() == FALSE) {
        if (x == HEIGHT - 2 || x == last_x - 1 && y == last_y || map[x + 1][y] == figure) {
            printf("SIGNAL: STOPPING");
            is_C = TRUE;
            stop_obj_CIRCLE(is_C);
            change_mode_CIRCLE();
        }
    }

    return is_C;
}

BOOL stop_obj_CIRCLE(BOOL is_C = FALSE) {
    return is_C;
}

BOOL stop_obj(VOID) {
    for (wint_t i = 0; i != coordinates_base.size(); i++) {
        if ((x == (coordinates_base[i].first - 1)) && (y == coordinates_base[i].second)) {
            change_mode_CIRCLE();
            return 1;
        }
    }
    return 0;
}

VOID MOVE_CIRCLE(VOID) {
    int prev_x = x, prev_y = y;
    clock_t CURRENT_TIME = clock();

    if (map[x + 1][y] != figure) {
        if (map[x][y] != L'#') {
            if (CURRENT_TIME - lastTime >= StopObject) {
                map[::x][::y] = ' ';
                x++;
                lastTime = CURRENT_TIME;
            }
            if (_kbhit() != 0) {
                map[x][y] = ' ';
                switch (_getch()) {
                case L'w':
                case L'W':
                    x--;
                    break;
                case L's':
                case L'S':
                    x++;
                    break;
                case L'a':
                case L'A':
                    y--;
                    break;
                case L'd':
                case L'D':
                    y++;
                    break;
                case 'f':
                case 'F':
                    x = WIDTH - 2;
                    break;
                }
            }
        }
    }
    else {
        change_mode_CIRCLE();
    }

    if (map[x][y] == L'#') {
        x = prev_x;
        y = prev_y;
    }
    map[x][y] = figure;
    object_x = x;
    object_y = y;
}

void add_to_database(int x, int y) {
    coordinates_base.push_back({ x, y });
}

void change_mode_CIRCLE(VOID) {
    add_to_database(object_x, object_y);
    last_x = x; last_y = y;
    object_x = BEGIN_X, object_y = BEGIN_Y;
    x = object_x; y = object_y;
}

BOOL CHECK_STOP_OBJECT(int prev_x) {
    if (map[23][y] == '#') {
        map[23][object_y] = ::CIRCLE;
        return true;
    }
    return false;
}

VOID PRINT_CONSOLE_DATABASE(VOID) {
    unsigned int count = 0;
    for (wint_t i{ 0 }; i != coordinates_base.size(); i++) {
        if (i != 0 && i % 2 == 0)
            putchar('\n');
        printf("%i. x: %i; y: %i\t\t", ++count, coordinates_base[i].first, coordinates_base[i].second);
    }
}

BOOL stop_obj_RECTANGLE(BOOL);
VOID change_mode_RECTANGLE(VOID);
VOID PRINT_CONSOLE_DATABASE_RECTANGLE(VOID);
VOID ADD_ELEMENTS(TCHAR);
static VOID add_to_database_RECTANGLE(int, int);

namespace rect {
    int last_X_1 = -1;
    int last_Y_1 = -1;
    int x_1 = x + 1;
    int x_2 = x + 2;
    int x_3 = x + 3;
    int y_1 = y + 1;

    int& X = x;
    int& Y = y;

    int& X_1 = x_1;
    int& X_2 = x_2;
    int& X_3 = x_3;

    int& Y_1 = y_1;
}

BOOL DRAW_RECTANGLE() {
    BOOL GAME_OVER = FALSE;
    COORD cursorPos = { 0, 0 };
    SetConsoleCursorPosition(hConsole, cursorPos);

    SetConsoleTextAttribute(hConsole, COLOR_RECTANGLE);

    for (wint_t i = 0; i < HEIGHT; i++) {
        for (wint_t j = 0; j < WIDTH; j++) {
            putchar(map[i][j]);
        }
        putchar(L'\n');
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    PRINT_PARAMETERS_FOR_MENU();

    printf("x = %i; y = %i\n", x, y);
    printf("last_x = %i; last_y = %i\n", rect::last_X_1, rect::last_Y_1);
    for (wint_t i = 0; i != base_rectangle.size(); i++) {
        if (BEGIN_X == (base_rectangle[i].first) && BEGIN_Y == base_rectangle[i].second) {
            GAME_OVER = TRUE;
            PRINT_CONSOLE_DATABASE_RECTANGLE();
            return GAME_OVER;
        }
    }
    static int count_rectangle = 0;

    if (map[rect::X_3 + 1][rect::Y] == figure || map[rect::X_3 + 1][rect::Y_1] == figure || map[rect::X_3 + 1][rect::Y_1] == L'#') {
        printf("SIGNAL: STOPPING: %i count", count_rectangle++);
        stop_obj_RECTANGLE(TRUE);
        change_mode_RECTANGLE();
        return TRUE;
    }
    BOOL is_Conflit = 0;
    if (rect::X_3 == HEIGHT - 2 || rect::X_3 == figure) {
        printf("SIGNAL: STOPPING: %i count", count_rectangle++);
        is_Conflit = 1;
        stop_obj_RECTANGLE(is_Conflit);
        change_mode_RECTANGLE();
    }

    ::std::cout << std::endl;
    for (int i = 0; i != base_rectangle.size(); i++) {
        printf("%i %i  ", base_rectangle[i].first, base_rectangle[i].second);
    }
    return is_Conflit;
}

VOID change_mode_RECTANGLE(VOID) {
    rect::last_X_1 = rect::X_1; rect::last_Y_1 = rect::Y_1;
    add_to_database_RECTANGLE(rect::X, rect::Y);
    int y = BEGIN_Y, x = BEGIN_X;
    rect::Y = y;
    rect::X = x;
    int x_1 = x + 1;
    rect::X_1 = x_1;
    int x_2 = x + 2;
    rect::X_2 = x_2;
    int x_3 = x + 3;
    rect::X_3 = x_3;
    int y_1 = y + 1;
    rect::Y_1 = y_1;
}

BOOL stop_obj_RECTANGLE(BOOL is_Con = false) {
    return is_Con;
}

int PREV_X = BEGIN_X;
int PREV_Y = BEGIN_Y;
int PREV_X_1 = x + 1;
int PREV_X_2 = x + 2;
int PREV_X_3 = x + 3;
int PREV_Y_1 = y + 1;

static VOID MOVE_RECTANGLE() {
    BOOL Conflict = FALSE;
    clock_t CURRENT_TIME = clock();

    if (map[rect::X_3 + 1][rect::Y] != figure && map[rect::X_3 + 1][rect::Y_1] != figure) {
        if (map[rect::X_3 + 1][y] != L'#') {
            ADD_ELEMENTS(L' ');
            if (CURRENT_TIME - lastTime >= StopObject) {
                rect::X++;
                rect::X_1++;
                rect::X_2++;
                rect::X_3++;
                lastTime = CURRENT_TIME;
            }
            if (_kbhit()) {
                switch (_getch()) {
                case L'a':
                case L'A':
                case L'ф':
                case L'Ф':
                    rect::Y--;
                    rect::Y_1--;
                    break;
                case L'd':
                case L'D':
                case L'в':
                case L'В':
                    rect::Y++;
                    rect::Y_1++;
                    break;
                case L's':
                case L'S':
                case L'Ы':
                case L'ы':
                    if (rect::X_3 + 1 != '#') {
                        rect::X++;
                        rect::X_1++;
                        rect::X_2++;
                        rect::X_3++;
                    }
                    break;
                case 'f':
                    rect::X = WIDTH - 5;
                    rect::X_1 = WIDTH - 4;
                    rect::X_2 = WIDTH - 3;
                    rect::X_3 = WIDTH - 2;
                    break;
                }
            }
        }
    } else {
        change_mode_RECTANGLE();
    }
    ADD_ELEMENTS(figure);
}

VOID ADD_ELEMENTS(TCHAR s) {
    map[rect::X][rect::Y] = s;
    map[rect::X][rect::Y_1] = s;
    map[rect::X_1][rect::Y] = s;
    map[rect::X_1][rect::Y_1] = s;
    map[rect::X_2][rect::Y_1] = s;
    map[rect::X_2][rect::Y] = s;
    map[rect::X_3][rect::Y_1] = s;
    map[rect::X_3][rect::Y] = s;
}

static VOID add_to_database_RECTANGLE(int X, int Y) {
    base_rectangle.push_back({ X, Y });
}

VOID PRINT_CONSOLE_DATABASE_RECTANGLE(VOID) {
    unsigned int count = 0;
    for (wint_t i{ 0 }; i != base_rectangle.size(); i++) {
        if (i != 0 && i % 2 == 0)
            putchar('\n');
        printf("%i. x: %i; y: %i\t\t", ++count, base_rectangle[i].first, base_rectangle[i].second);
    }
}

namespace sq {
    int x_1 = x + 1;
    int x_2 = x + 2;
    int x_3 = x + 3;
    int y_1 = y + 1;

    int& X = x;
    int& Y = y;

    int& X_1 = x_1;
    int& X_2 = x_2;
    int& X_3 = x_3;

    int& Y_1 = y_1;
}
using namespace sq;

VOID ADD_ELEMENTS_SQUARE(TCHAR);
VOID change_mode_SQUARE(VOID);
static VOID add_to_database_SQAURE(int, int);

BOOL DRAW_SQUARE() {
    BOOL GAME_OVER = FALSE;
    COORD cursorPos = { 0, 0 };
    SetConsoleCursorPosition(hConsole, cursorPos);

    SetConsoleTextAttribute(hConsole, COLOR_SQUARE);

    for (wint_t i = 0; i < HEIGHT; i++) {
        for (wint_t j = 0; j < WIDTH; j++) {
            putchar(map[i][j]);
        }
        putchar(L'\n');
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    PRINT_PARAMETERS_FOR_MENU();
    printf("x = %i; y = %i\t\t\tCounting seconds: %li\n", x, y, NOWTime);
    printf("object_x = %i; object_y = %i\n", object_x, object_y);
    printf("last_x = %i; last_y = %i\n", last_x, last_y);

    for (wint_t i = 0; i != base_rectangle.size(); i++) {
        if (BEGIN_X == (base_rectangle[i].first) && BEGIN_Y == base_rectangle[i].second) {
            GAME_OVER = TRUE;
            PRINT_CONSOLE_DATABASE_RECTANGLE();
            return GAME_OVER;
        }
    }

    static int count_rectangle = 0;
    if (map[sq::X_3 - 1][sq::Y_1] == L'#' || map[sq::X_3 - 1][sq::Y_1] == figure) {
        printf("SIGNAL: STOPPING: %i count", count_rectangle++);
        change_mode_SQUARE();
        return TRUE;
    }
    BOOL is_Conflit = 0;
    if (map[sq::X_3 - 1][sq::Y_1] == L'#' || map[sq::X_3 - 1][sq::Y_1] == figure) {
        printf("SIGNAL: STOPPING: %i count", count_rectangle++);
        is_Conflit = stop_obj_RECTANGLE(TRUE);
        change_mode_SQUARE();
    }

    ::std::cout << std::endl;
    for (int i = 0; i != base_rectangle.size(); i++) {
        printf("%i %i  ", base_rectangle[i].first, base_rectangle[i].second);
    }
    return is_Conflit;
}

static VOID MOVE_SQUARE() {
    BOOL Conflict = FALSE;
    clock_t CURRENT_TIME = clock();

    if (map[sq::X_3 - 1][y] != figure && map[sq::X_3 - 1][sq::Y_1] != figure && map[sq::X][sq::Y_1] != '#') {
        if (map[sq::X_1 + 1][sq::Y] != L'#') {
            ADD_ELEMENTS_SQUARE(' ');
            if (CURRENT_TIME - lastTime >= StopObject) {
                sq::X++;
                sq::X_1++;
                sq::X_2++;
                sq::X_3++;
                lastTime = CURRENT_TIME;
            }
            if (_kbhit()) {
                switch (_getch()) {
                case L'a':
                case L'A':
                case L'ф':
                case L'Ф':
                    sq::Y--;
                    sq::Y_1--;
                    break;
                case L'd':
                case L'D':
                case L'в':
                case L'В':
                    sq::Y++;
                    sq::Y_1++;
                    break;
                case 'f':
                case 'F':
                    sq::X = WIDTH - 4;
                    sq::X_1 = WIDTH - 3;
                    sq::X_2 = WIDTH - 2;
                    break;
                }
            }
        }
        else {
            change_mode_SQUARE();
        }
    }
    else {
        change_mode_SQUARE();
    }
    ADD_ELEMENTS_SQUARE(figure);
}

static VOID add_to_database_SQAURE(int X, int Y) {
    base_square.push_back({ X, Y });
}

VOID change_mode_SQUARE(VOID) {
    int y = BEGIN_Y, x = BEGIN_X;
    sq::Y = y;
    sq::X = x;
    int x_1 = x + 1;
    sq::X_1 = x_1;
    int x_2 = x + 2;
    sq::X_2 = x_2;
    int x_3 = x + 3;
    sq::X_3 = x_3;
    int y_1 = y + 1;
    sq::Y_1 = y_1;
}

VOID ADD_ELEMENTS_SQUARE(TCHAR s) {
    map[sq::X][sq::Y] = s;
    map[sq::X][sq::Y_1] = s;
    map[sq::X_1][sq::Y] = s;
    map[sq::X_1][sq::Y_1] = s;
}

namespace l_shape {
    int last_X_1 = -1;
    int last_Y_1 = -1;
    int x_1 = x + 1;
    int x_2 = x + 2;
    int y_1 = y + 1;
    int y_2 = y + 2;

    int& X = x;
    int& Y = y;
    int& X_1 = x_1;
    int& X_2 = x_2;
    int& Y_1 = y_1;
    int& Y_2 = y_2;
}

VOID ADD_ELEMENTS_L_SHAPE(TCHAR s) {
    map[l_shape::X][l_shape::Y] = s;
    map[l_shape::X_1][l_shape::Y] = s;
    map[l_shape::X_2][l_shape::Y] = s;
    map[l_shape::X_2][l_shape::Y_1] = s;
}

static VOID add_to_database_L_SHAPE(int X, int Y) {
    base_l_shape.push_back({ X, Y });
}

VOID change_mode_L_SHAPE(VOID) {
    l_shape::last_X_1 = l_shape::X_1;
    l_shape::last_Y_1 = l_shape::Y_1;
    add_to_database_L_SHAPE(l_shape::X, l_shape::Y);
    int y = BEGIN_Y, x = BEGIN_X;
    l_shape::Y = y;
    l_shape::X = x;
    l_shape::X_1 = x + 1;
    l_shape::X_2 = x + 2;
    l_shape::Y_1 = y + 1;
    l_shape::Y_2 = y + 2;
}

BOOL DRAW_L_SHAPE() {
    BOOL GAME_OVER = FALSE;
    COORD cursorPos = { 0, 0 };
    SetConsoleCursorPosition(hConsole, cursorPos);

    SetConsoleTextAttribute(hConsole, COLOR_L_SHAPE);

    for (wint_t i = 0; i < HEIGHT; i++) {
        for (wint_t j = 0; j < WIDTH; j++) {
            putchar(map[i][j]);
        }
        putchar(L'\n');
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    PRINT_PARAMETERS_FOR_MENU();
    printf("x = %i; y = %i\n", l_shape::X, l_shape::Y);
    printf("last_x = %i; last_y = %i\n", l_shape::last_X_1, l_shape::last_Y_1);

    for (wint_t i = 0; i != base_l_shape.size(); i++) {
        if (BEGIN_X == (base_l_shape[i].first) && BEGIN_Y == base_l_shape[i].second) {
            GAME_OVER = TRUE;
            return GAME_OVER;
        }
    }

    static int count_l_shape = 0;
    if (map[l_shape::X_2 + 1][l_shape::Y] == figure || map[l_shape::X_2][l_shape::Y_1] == figure || map[l_shape::X_2 + 1][l_shape::Y] == L'#') {
        printf("SIGNAL: STOPPING: %i count", count_l_shape++);
        change_mode_L_SHAPE();
        return TRUE;
    }
    BOOL is_Conflit = 0;
    if (l_shape::X_2 == HEIGHT - 2) {
        printf("SIGNAL: STOPPING: %i count", count_l_shape++);
        is_Conflit = 1;
        change_mode_L_SHAPE();
    }

    return is_Conflit;
}

static VOID MOVE_L_SHAPE() {
    BOOL Conflict = FALSE;
    clock_t CURRENT_TIME = clock();

    if (map[l_shape::X_2 + 1][l_shape::Y] != figure && map[l_shape::X_2][l_shape::Y_1] != figure) {
        if (map[l_shape::X_2 + 1][l_shape::Y] != L'#') {
            ADD_ELEMENTS_L_SHAPE(L' ');
            if (CURRENT_TIME - lastTime >= StopObject) {
                l_shape::X++;
                l_shape::X_1++;
                l_shape::X_2++;
                lastTime = CURRENT_TIME;
            }
            if (_kbhit()) {
                switch (_getch()) {
                case L'a':
                case L'A':
                case L'ф':
                case L'Ф':
                    l_shape::Y--;
                    l_shape::Y_1--;
                    l_shape::Y_2--;
                    break;
                case L'd':
                case L'D':
                case L'в':
                case L'В':
                    l_shape::Y++;
                    l_shape::Y_1++;
                    l_shape::Y_2++;
                    break;
                case L's':
                case L'S':
                case L'Ы':
                case L'ы':
                    if (map[l_shape::X_2 + 1][l_shape::Y] != L'#') {
                        l_shape::X++;
                        l_shape::X_1++;
                        l_shape::X_2++;
                    }
                    break;
                case 'f':
                    l_shape::X = WIDTH - 3;
                    l_shape::X_1 = WIDTH - 2;
                    l_shape::X_2 = WIDTH - 1;
                    break;
                }
            }
        }
    } else {
        change_mode_L_SHAPE();
    }
    ADD_ELEMENTS_L_SHAPE(figure);
}

namespace t_shape {
    int last_X_1 = -1;
    int last_Y_1 = -1;
    int x_1 = x + 1;
    int y_1 = y + 1;
    int y_2 = y + 2;

    int& X = x;
    int& Y = y;
    int& X_1 = x_1;
    int& Y_1 = y_1;
    int& Y_2 = y_2;
}

VOID ADD_ELEMENTS_T_SHAPE(TCHAR s) {
    map[t_shape::X][t_shape::Y_1] = s;
    map[t_shape::X_1][t_shape::Y] = s;
    map[t_shape::X_1][t_shape::Y_1] = s;
    map[t_shape::X_1][t_shape::Y_2] = s;
}

static VOID add_to_database_T_SHAPE(int X, int Y) {
    base_t_shape.push_back({ X, Y });
}

VOID change_mode_T_SHAPE(VOID) {
    t_shape::last_X_1 = t_shape::X_1;
    t_shape::last_Y_1 = t_shape::Y_1;
    add_to_database_T_SHAPE(t_shape::X, t_shape::Y);
    int y = BEGIN_Y, x = BEGIN_X;
    t_shape::Y = y;
    t_shape::X = x;
    t_shape::X_1 = x + 1;
    t_shape::Y_1 = y + 1;
    t_shape::Y_2 = y + 2;
}

BOOL DRAW_T_SHAPE() {
    BOOL GAME_OVER = FALSE;
    COORD cursorPos = { 0, 0 };
    SetConsoleCursorPosition(hConsole, cursorPos);

    SetConsoleTextAttribute(hConsole, COLOR_T_SHAPE);

    for (wint_t i = 0; i < HEIGHT; i++) {
        for (wint_t j = 0; j < WIDTH; j++) {
            putchar(map[i][j]);
        }
        putchar(L'\n');
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    PRINT_PARAMETERS_FOR_MENU();
    printf("x = %i; y = %i\n", t_shape::X, t_shape::Y);
    printf("last_x = %i; last_y = %i\n", t_shape::last_X_1, t_shape::last_Y_1);

    for (wint_t i = 0; i != base_t_shape.size(); i++) {
        if (BEGIN_X == (base_t_shape[i].first) && BEGIN_Y == base_t_shape[i].second) {
            GAME_OVER = TRUE;
            return GAME_OVER;
        }
    }

    static int count_t_shape = 0;
    if (map[t_shape::X_1 + 1][t_shape::Y] == figure || map[t_shape::X_1 + 1][t_shape::Y_1] == figure || map[t_shape::X_1 + 1][t_shape::Y_2] == figure || map[t_shape::X_1 + 1][t_shape::Y_1] == L'#') {
        printf("SIGNAL: STOPPING: %i count", count_t_shape++);
        change_mode_T_SHAPE();
        return TRUE;
    }
    BOOL is_Conflit = 0;
    if (t_shape::X_1 == HEIGHT - 2) {
        printf("SIGNAL: STOPPING: %i count", count_t_shape++);
        is_Conflit = 1;
        change_mode_T_SHAPE();
    }

    return is_Conflit;
}

static VOID MOVE_T_SHAPE() {
    BOOL Conflict = FALSE;
    clock_t CURRENT_TIME = clock();

    if (map[t_shape::X_1 + 1][t_shape::Y] != figure && map[t_shape::X_1 + 1][t_shape::Y_1] != figure && map[t_shape::X_1 + 1][t_shape::Y_2] != figure) {
        if (map[t_shape::X_1 + 1][t_shape::Y_1] != L'#') {
            ADD_ELEMENTS_T_SHAPE(L' ');
            if (CURRENT_TIME - lastTime >= StopObject) {
                t_shape::X++;
                t_shape::X_1++;
                lastTime = CURRENT_TIME;
            }
            if (_kbhit()) {
                switch (_getch()) {
                case L'a':
                case L'A':
                case L'ф':
                case L'Ф':
                    t_shape::Y--;
                    t_shape::Y_1--;
                    t_shape::Y_2--;
                    break;
                case L'd':
                case L'D':
                case L'в':
                case L'В':
                    t_shape::Y++;
                    t_shape::Y_1++;
                    t_shape::Y_2++;
                    break;
                case L's':
                case L'S':
                case L'Ы':
                case L'ы':
                    if (map[t_shape::X_1 + 1][t_shape::Y_1] != L'#') {
                        t_shape::X++;
                        t_shape::X_1++;
                    }
                    break;
                case 'f':
                    t_shape::X = WIDTH - 2;
                    t_shape::X_1 = WIDTH - 1;
                    break;
                }
            }
        }
    } else {
        change_mode_T_SHAPE();
    }
    ADD_ELEMENTS_T_SHAPE(figure);
}

namespace z_shape {
    int last_X_1 = -1;
    int last_Y_1 = -1;
    int x_1 = x + 1;
    int y_1 = y + 1;
    int y_2 = y + 2;

    int& X = x;
    int& Y = y;
    int& X_1 = x_1;
    int& Y_1 = y_1;
    int& Y_2 = y_2;
}

VOID ADD_ELEMENTS_Z_SHAPE(TCHAR s) {
    map[z_shape::X][z_shape::Y] = s;
    map[z_shape::X][z_shape::Y_1] = s;
    map[z_shape::X_1][z_shape::Y_1] = s;
    map[z_shape::X_1][z_shape::Y_2] = s;
}

static VOID add_to_database_Z_SHAPE(int X, int Y) {
    base_z_shape.push_back({ X, Y });
}

VOID change_mode_Z_SHAPE(VOID) {
    z_shape::last_X_1 = z_shape::X_1;
    z_shape::last_Y_1 = z_shape::Y_1;
    add_to_database_Z_SHAPE(z_shape::X, z_shape::Y);
    int y = BEGIN_Y, x = BEGIN_X;
    z_shape::Y = y;
    z_shape::X = x;
    z_shape::X_1 = x + 1;
    z_shape::Y_1 = y + 1;
    z_shape::Y_2 = y + 2;
}

BOOL DRAW_Z_SHAPE() {
    BOOL GAME_OVER = FALSE;
    COORD cursorPos = { 0, 0 };
    SetConsoleCursorPosition(hConsole, cursorPos);

    SetConsoleTextAttribute(hConsole, COLOR_Z_SHAPE);

    for (wint_t i = 0; i < HEIGHT; i++) {
        for (wint_t j = 0; j < WIDTH; j++) {
            putchar(map[i][j]);
        }
        putchar(L'\n');
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    PRINT_PARAMETERS_FOR_MENU();
    printf("x = %i; y = %i\n", z_shape::X, z_shape::Y);
    printf("last_x = %i; last_y = %i\n", z_shape::last_X_1, z_shape::last_Y_1);

    for (wint_t i = 0; i != base_z_shape.size(); i++) {
        if (BEGIN_X == (base_z_shape[i].first) && BEGIN_Y == base_z_shape[i].second) {
            GAME_OVER = TRUE;
            return GAME_OVER;
        }
    }

    static int count_z_shape = 0;
    if (map[z_shape::X + 1][z_shape::Y] == figure || map[z_shape::X_1 + 1][z_shape::Y_1] == figure || map[z_shape::X_1 + 1][z_shape::Y_2] == figure || map[z_shape::X_1 + 1][z_shape::Y_2] == L'#') {
        printf("SIGNAL: STOPPING: %i count", count_z_shape++);
        change_mode_Z_SHAPE();
        return TRUE;
    }
    BOOL is_Conflit = 0;
    if (z_shape::X_1 == HEIGHT - 2) {
        printf("SIGNAL: STOPPING: %i count", count_z_shape++);
        is_Conflit = 1;
        change_mode_Z_SHAPE();
    }

    return is_Conflit;
}

static VOID MOVE_Z_SHAPE() {
    BOOL Conflict = FALSE;
    clock_t CURRENT_TIME = clock();

    if (map[z_shape::X + 1][z_shape::Y] != figure && map[z_shape::X_1 + 1][z_shape::Y_1] != figure && map[z_shape::X_1 + 1][z_shape::Y_2] != figure) {
        if (map[z_shape::X_1 + 1][z_shape::Y_2] != L'#') {
            ADD_ELEMENTS_Z_SHAPE(L' ');
            if (CURRENT_TIME - lastTime >= StopObject) {
                z_shape::X++;
                z_shape::X_1++;
                lastTime = CURRENT_TIME;
            }
            if (_kbhit()) {
                switch (_getch()) {
                case L'a':
                case L'A':
                case L'ф':
                case L'Ф':
                    z_shape::Y--;
                    z_shape::Y_1--;
                    z_shape::Y_2--;
                    break;
                case L'd':
                case L'D':
                case L'в':
                case L'В':
                    z_shape::Y++;
                    z_shape::Y_1++;
                    z_shape::Y_2++;
                    break;
                case L's':
                case L'S':
                case L'Ы':
                case L'ы':
                    if (map[z_shape::X_1 + 1][z_shape::Y_2] != L'#') {
                        z_shape::X++;
                        z_shape::X_1++;
                    }
                    break;
                case 'f':
                    z_shape::X = WIDTH - 2;
                    z_shape::X_1 = WIDTH - 1;
                    break;
                }
            }
        }
    }
    else {
        change_mode_Z_SHAPE();
    }
    ADD_ELEMENTS_Z_SHAPE(figure);
}

namespace s_shape {
    int last_X_1 = -1;
    int last_Y_1 = -1;
    int x_1 = x + 1;
    int y_1 = y + 1;
    int y_2 = y + 2;

    int& X = x;
    int& Y = y;
    int& X_1 = x_1;
    int& Y_1 = y_1;
    int& Y_2 = y_2;
}

VOID ADD_ELEMENTS_S_SHAPE(TCHAR s) {
    map[s_shape::X][s_shape::Y_1] = s;
    map[s_shape::X][s_shape::Y_2] = s;
    map[s_shape::X_1][s_shape::Y] = s;
    map[s_shape::X_1][s_shape::Y_1] = s;
}

static VOID add_to_database_S_SHAPE(int X, int Y) {
    base_s_shape.push_back({ X, Y });
}

VOID change_mode_S_SHAPE(VOID) {
    s_shape::last_X_1 = s_shape::X_1;
    s_shape::last_Y_1 = s_shape::Y_1;
    add_to_database_S_SHAPE(s_shape::X, s_shape::Y);
    int y = BEGIN_Y, x = BEGIN_X;
    s_shape::Y = y;
    s_shape::X = x;
    s_shape::X_1 = x + 1;
    s_shape::Y_1 = y + 1;
    s_shape::Y_2 = y + 2;
}

BOOL DRAW_S_SHAPE() {
    BOOL GAME_OVER = FALSE;
    COORD cursorPos = { 0, 0 };
    SetConsoleCursorPosition(hConsole, cursorPos);

    SetConsoleTextAttribute(hConsole, COLOR_S_SHAPE);

    for (wint_t i = 0; i < HEIGHT; i++) {
        for (wint_t j = 0; j < WIDTH; j++) {
            putchar(map[i][j]);
        }
        putchar(L'\n');
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    PRINT_PARAMETERS_FOR_MENU();
    printf("x = %i; y = %i\n", s_shape::X, s_shape::Y);
    printf("last_x = %i; last_y = %i\n", s_shape::last_X_1, s_shape::last_Y_1);

    for (wint_t i = 0; i != base_s_shape.size(); i++) {
        if (BEGIN_X == (base_s_shape[i].first) && BEGIN_Y == base_s_shape[i].second) {
            GAME_OVER = TRUE;
            return GAME_OVER;
        }
    }

    static int count_s_shape = 0;
    if (map[s_shape::X + 1][s_shape::Y_1] == figure || map[s_shape::X + 1][s_shape::Y_2] == figure || map[s_shape::X_1 + 1][s_shape::Y] == figure || map[s_shape::X_1 + 1][s_shape::Y_1] == L'#') {
        printf("SIGNAL: STOPPING: %i count", count_s_shape++);
        change_mode_S_SHAPE();
        return TRUE;
    }
    BOOL is_Conflit = 0;
    if (s_shape::X_1 == HEIGHT - 2) {
        printf("SIGNAL: STOPPING: %i count", count_s_shape++);
        is_Conflit = 1;
        change_mode_S_SHAPE();
    }

    return is_Conflit;
}

static VOID MOVE_S_SHAPE() {
    BOOL Conflict = FALSE;
    clock_t CURRENT_TIME = clock();

    if (map[s_shape::X + 1][s_shape::Y_1] != figure && map[s_shape::X + 1][s_shape::Y_2] != figure && map[s_shape::X_1 + 1][s_shape::Y] != figure) {
        if (map[s_shape::X_1 + 1][s_shape::Y] != L'#') {
            ADD_ELEMENTS_S_SHAPE(L' ');
            if (CURRENT_TIME - lastTime >= StopObject) {
                s_shape::X++;
                s_shape::X_1++;
                lastTime = CURRENT_TIME;
            }
            if (_kbhit()) {
                switch (_getch()) {
                case L'a':
                case L'A':
                case L'ф':
                case L'Ф':
                    s_shape::Y--;
                    s_shape::Y_1--;
                    s_shape::Y_2--;
                    break;
                case L'd':
                case L'D':
                case L'в':
                case L'В':
                    s_shape::Y++;
                    s_shape::Y_1++;
                    s_shape::Y_2++;
                    break;
                case L's':
                case L'S':
                case L'Ы':
                case L'ы':
                    if (map[s_shape::X_1 + 1][s_shape::Y] != L'#') {
                        s_shape::X++;
                        s_shape::X_1++;
                    }
                    break;
                case 'f':
                    s_shape::X = WIDTH - 2;
                    s_shape::X_1 = WIDTH - 1;
                    break;
                }
            }
        }
    }
    else {
        change_mode_S_SHAPE();
    }
    ADD_ELEMENTS_S_SHAPE(figure);
}

wint_t min = 0;
wint_t max = 6;

VOID CLEAR_PRINT_PARAMETERS_FOR_NEXT();

int main(int argc, TCHAR** argv) {
    CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    ::INIT();

    srand(time(0));

    ::std::vector<int> index_arr;
    ::std::random_device dev;
    ::std::mt19937 rng(dev());
    ::std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
    for (int i = 0; i != 50; i++) {
        rand_arr.push_back(dist6(rng));
    }

    int index = 0;
    int prev_x = 0;
    BOOL is_C = 0;

    wint_t s_circle = 0;
    wint_t s_rectangle = 0;
    wint_t s_square = 0;
    wint_t s_l_shape = 0;
    wint_t s_t_shape = 0;
    wint_t s_z_shape = 0;
    wint_t s_s_shape = 0;

    wint_t index_for_rand_array = 0;
    while (true)
    {
        index_for_rand_array = rand_arr[index];
        next_index_figure = rand_arr[index + 1];
        if (index_for_rand_array == 0) {
            while (true) {
                if (DRAW_CIRCLE() == TRUE) {
                    s_circle++;
                    CLEAR_PRINT_PARAMETERS_FOR_NEXT();
                    break;
                }
                MOVE_CIRCLE();
                Sleep(16);
            }
        }
        else if (index_for_rand_array == 1) {
            while (true) {
                if (DRAW_RECTANGLE() == TRUE) {
                    s_rectangle++;
                    CLEAR_PRINT_PARAMETERS_FOR_NEXT();
                    break;
                }
                MOVE_RECTANGLE();
                Sleep(16);
            }
        }
        else if (index_for_rand_array == 2) {
            while (true) {
                if (DRAW_SQUARE() == TRUE) {
                    s_square++;
                    CLEAR_PRINT_PARAMETERS_FOR_NEXT();
                    break;
                }
                MOVE_SQUARE();
                Sleep(16);
            }
        }
        else if (index_for_rand_array == 3) {
            while (true) {
                if (DRAW_L_SHAPE() == TRUE) {
                    s_l_shape++;
                    CLEAR_PRINT_PARAMETERS_FOR_NEXT();
                    break;
                }
                MOVE_L_SHAPE();
                Sleep(16);
            }
        }
        else if (index_for_rand_array == 4) {
            while (true) {
                if (DRAW_T_SHAPE() == TRUE) {
                    s_t_shape++;
                    CLEAR_PRINT_PARAMETERS_FOR_NEXT();
                    break;
                }
                MOVE_T_SHAPE();
                Sleep(16);
            }
        }
        else if (index_for_rand_array == 5) {
            while (true) {
                if (DRAW_Z_SHAPE() == TRUE) {
                    s_z_shape++;
                    CLEAR_PRINT_PARAMETERS_FOR_NEXT();
                    break;
                }
                MOVE_Z_SHAPE();
                Sleep(16);
            }
        }
        else if (index_for_rand_array == 6) {
            while (true) {
                if (DRAW_S_SHAPE() == TRUE) {
                    s_s_shape++;
                    CLEAR_PRINT_PARAMETERS_FOR_NEXT();
                    break;
                }
                MOVE_S_SHAPE();
                Sleep(16);
            }
        }
        ++index;
        test_counting++;
        index_arr.push_back(index_for_rand_array);
        printf("\ntest_count : %i", test_counting);
    }
    std::system("cls");
    ::std::printf("rand_arr: \n");
    for (wint_t i = 0; i != rand_arr.size(); i++)
        ::std::printf("%i ", rand_arr[i]);
    ::std::printf("\nindex_arr: \n");
    for (wint_t i = 0; i != index_arr.size(); i++)
        printf("%i ", index_arr[i]);
    ::std::printf("\ns_circle (of circle): %i\n", s_circle);
    ::std::printf("\ns_rectangle (of rectangle): %i\n", s_rectangle);
    ::std::printf("\ns_square (of square): %i\n", s_square);
    ::std::printf("\ns_l_shape (of L-shape): %i\n", s_l_shape);
    ::std::printf("\ns_t_shape (of T-shape): %i\n", s_t_shape);
    ::std::printf("\ns_z_shape (of Z-shape): %i\n", s_z_shape);
    ::std::printf("\ns_s_shape (of S-shape): %i\n", s_s_shape);

    int a;
    ::std::printf("\nExit? ");
    ::std::cin >> a;

    return 0;
}

VOID PRINT_PARAMETERS_FOR_MENU(VOID) {
    printf("%s\t", "SPEED");
    printf("%s\t", "SCORE");
    printf("%s", "HI-SCORE");
    printf("   %s\t", "TIME");
    printf("\n %i\t", StopObject);
    printf("  %i\t", 0);
    printf("   %i\t", 0);
    printf("   %li\n", NOWTime);
    printf("%s\n", "---------------------------------");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    if (next_index_figure == 0) {
        SetConsoleTextAttribute(hConsole, COLOR_CIRCLE);
        printf("\t%c\n", figure);
    }
    else if (next_index_figure == 1) {
        SetConsoleTextAttribute(hConsole, COLOR_RECTANGLE);
        printf("\t%c%c\n        %c%c\n        %c%c\n", figure, figure, figure, figure, figure, figure);
    }
    else if (next_index_figure == 2) {
        SetConsoleTextAttribute(hConsole, COLOR_SQUARE);
        printf("\t%c%c\n        %c%c\n", figure, figure, figure, figure);
    }
    else if (next_index_figure == 3) {
        SetConsoleTextAttribute(hConsole, COLOR_L_SHAPE);
        printf("\t%c\n        %c\n        %c%c\n", figure, figure, figure, figure);
    }
    else if (next_index_figure == 4) {
        SetConsoleTextAttribute(hConsole, COLOR_T_SHAPE);
        printf("\t %c\n       %c%c%c\n", figure, figure, figure, figure);
    }
    else if (next_index_figure == 5) {
        SetConsoleTextAttribute(hConsole, COLOR_Z_SHAPE);
        printf("\t%c%c\n         %c%c\n", figure, figure, figure, figure);
    }
    else if (next_index_figure == 6) {
        SetConsoleTextAttribute(hConsole, COLOR_S_SHAPE);
        printf("\t %c%c\n       %c%c\n", figure, figure, figure, figure);
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("\n\n");
}

VOID CLEAR_PRINT_PARAMETERS_FOR_NEXT(VOID) {
    printf("%s\t", "SPEED");
    printf("%s\t", "SCORE");
    printf("%s", "HI-SCORE");
    printf("   %s\t", "TIME");
    printf("\n %i\t", StopObject);
    printf("  %i\t", 0);
    printf("   %i\t", 0);
    printf("   %li\n", NOWTime);
    printf("%s\n", "---------------------------------");
    printf("                           \n");
    printf("                           \n");
    printf("                           \n");
    printf("                           \n");
    printf("                           \n");
    printf("                           \n");
    printf("\n\n");
}
