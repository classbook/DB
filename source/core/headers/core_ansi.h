/**
 * @file core_ansi.h
 * @author Kishore
 * @date March 19, 2016, 11:35 PM 
 */

#include "common/headers/init.h"

void ansi_fg_bg(int fg, int bg);
void ansi_default();

void ansi_dim();
void ansi_bold();
void ansi_uline();
void ansi_reverse();
void ansi_cursor_hide();
void ansi_clear_line();

void ansi_cursor_right(int offset);
void ansi_cursor_left(int offset);
void ansi_cursor_up(int offset);
void ansi_cursor_down(int offset);