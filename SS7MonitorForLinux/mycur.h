#include <ncurses.h>
#include <stdarg.h>
#ifndef _MYCUR_H
#define _MYCUR_H

void initCursel(int *row, int *col);
char *inputstr(char *str);
void formatprint(WINDOW* win, const char* fmt, ...);
void printstr(WINDOW* win, const char *fmt, ...);
void exitwin();
#endif