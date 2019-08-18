#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

// START: Functionality related to the game

void game_exit ( void );

#define CHARACTER '+'

// END

// START: Functionality related to curses
WINDOW *create_newwin(int height, int width, int starty, int startx);

int check_up_boundaries(int y_pos,int max_height);
int check_down_boundaries(int y_pos, int max_height);
int check_left_boundaries(int x_pos, int max_width);
int check_right_boundaries(int x_pos, int max_width);

void destroy_win(WINDOW *win);
// END

int main ( void )
{
  WINDOW *win, *border;
  int width, height, max_width, max_height, starty, startx;
  int ch;
  
  // Startup init

  initscr();
  keypad(stdscr, TRUE); 
  noecho();
  raw();
  curs_set(0);
  
  max_height = LINES;
  max_width  = COLS;
  
  height = 1;
  width  = 1;

  starty = max_height/2;
  startx = max_width/2;

  border = newwin(max_height, max_width, 0, 0);
  win = newwin(height, width, starty, startx);

  refresh();

  // Draw the main-border
  box(border, 0, 0);
  wrefresh(border);
  
  while ((ch = getch()) != 113) // 'q' is escape key
    {
      switch(ch)
	{
	case KEY_UP:
	  if (!check_up_boundaries(starty, max_height)) {
	    destroy_win(win);
	    win = create_newwin(height, width, --starty, startx);
	  }
	  break;
	case KEY_DOWN:
	  if (!check_down_boundaries(starty, max_height)) {
	    destroy_win(win);
	    win = create_newwin(height, width, ++starty, startx);
	  }
	  break;
	case KEY_LEFT:
	  if (!check_left_boundaries(startx, max_width)) {
	    destroy_win(win);
	    win = create_newwin(height, width, starty, --startx);
	  }
	  break;
	case KEY_RIGHT:
	  if (!check_right_boundaries(startx, max_width)) {
	    destroy_win(win);
	    win = create_newwin(height, width, starty, ++startx);
	  }
	  break;
	}
    }

  // Cleanup time
  delwin(border);
  endwin();
  game_exit();
  
  return 0;
}

void game_exit( void )
{
  //puts("Goodbye");
  return;
}

// Curses-related functions

// check_*_boundaries
// Return values:
// 1 => Collision
// 0 => No Collision
int check_up_boundaries(int y_pos, int max_height)
{
  if (y_pos <= 1)  {
    return 1;
  }

  return 0;
}

int check_down_boundaries(int y_pos, int max_height)
{
  // printw("cur_y: %i cur_x: %i\nmax_h: %i max_w: %i", y_pos, x_pos, max_height, max_width);

  if (++y_pos >= max_height-1)  {
    return 1;
  }

  return 0;
}

int check_left_boundaries(int x_pos, int max_width)
{
  if (x_pos <= 1) {
    return 1;
  }
  
  return 0;
}

int check_right_boundaries(int x_pos, int max_width)
{
  if (++x_pos >= max_width-1) {
    return 1;
  }
  
  return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
  WINDOW *win;

  win = newwin(height, width, starty, startx);

  wborder(win, CHARACTER, CHARACTER, CHARACTER, CHARACTER, CHARACTER, CHARACTER, CHARACTER, CHARACTER);
  
  wrefresh(win);

  return win;
}

void destroy_win(WINDOW *win)
{
  wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');

  wrefresh(win);

  delwin(win);
  
  return;
}
