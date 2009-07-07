#include "mycur.h"

void initCursel(int *row, int *col)
{
	initscr();
	cbreak();
	keypad(stdscr, true);
	
	getmaxyx(stdscr, *row, *col);
}

char *inputstr(char *str)
{
	if (str == NULL)
		printstr(NULL, "inputstr param is NULL.\n");
	else
	{
		getstr(str);
	}	
	
	return str;	
}

void formatprint(WINDOW* win, const char* fmt, ...)
{	
	int maxy, maxx;
	int height, width;
	
	va_list vfmtlist;
	va_start(vfmtlist, fmt);	
	
	if (win == NULL)
		win = stdscr;	
	
	getyx(win, height, width);
	getmaxyx(win, maxy, maxx);	
			
  if (maxy <= height)
  {
  	clear();
  	getyx(win, height, width);
  }
  
  if (width != 0)
		width += 3;
	//mvprintw(row, col, fmt, vfmtlist);
	vwprintw(win, fmt, vfmtlist);
	move(height, width);
	
	
	va_end(vfmtlist);	
	refresh();
}

void printstr(WINDOW* win, const char *fmt, ...)
{
	va_list vfmtlist;
	va_start(vfmtlist, fmt);
	
	if (win == NULL)
		win = stdscr;
		
	vwprintw(stdscr, fmt, vfmtlist);
	refresh();
	
	va_end(vfmtlist);	
}

void exitwin()
{
	endwin();
}