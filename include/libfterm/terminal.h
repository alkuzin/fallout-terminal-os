/*
MIT License

Copyright (c) 2024 Alexander (@alkuzin)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _LIBFTERM_TERMINAL_H_
#define _LIBFTERM_TERMINAL_H_

#include <libfterm/utils.h>
#include <libk/stddef.h>
#include <libk/stdint.h>
#include <kernel/vga.h>

typedef struct option_s {
    char title[OPTION_TITLE_SIZE];
    int  type;
    int  size;
    void *content;
} option_t;

typedef struct terminal_s {
    char        title[TERMINAL_TITLE_SIZE];
    uint16_t    field_width;
    option_t    *options;
    option_t    *prev_options;
    option_t    *parent_window;
    vga_color_t fg; /* foreground color */
    vga_color_t bg; /* background color */
} terminal_t;

extern terminal_t *terminal;

// selected option index
extern int  selected;
extern bool is_terminal_set;

/* initialize terminal */
void terminal_init(const char *title, uint16_t fw, option_t *opt_list);

/* set terminal foreground & background colors */
void terminal_set_theme(vga_color_t fg, vga_color_t bg);

void terminal_print_options(void);

void terminal_print_content(void);

/* set option content (text, list of options or error) */
void terminal_set_option_content(option_t *option, void* content);

#endif /* _LIBFTERM_TERMINAL_H_ */
