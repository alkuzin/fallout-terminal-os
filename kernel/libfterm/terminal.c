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

#include <libfterm/terminal.h>
#include <kernel/sstd.h>
#include <libk/stdint.h>
#include <libk/string.h>
#include <libk/stddef.h>
#include <kernel/tty.h>

// selected option index
int selected;
bool is_terminal_set;

void terminal_init(const char *title, uint16_t fw, option_t *opt_list)
{
    /* set terminal data */
    strlcpy(terminal->title, title, strlen(title));
    terminal->field_width  = fw;
    terminal->options      = opt_list;
    terminal->prev_options = opt_list;
    terminal->parent_window = opt_list;
    selected = 0;
    is_terminal_set = true;

	kprints(terminal->title, DEFAULT_DELAY);
    
    __kclear();
    terminal_print_options();
}

void terminal_set_theme(vga_color_t fg, vga_color_t bg)
{
    terminal->fg = fg;
    terminal->bg = bg;

    __set_default_color(fg, bg);
}

void terminal_set_option_content(option_t* option, void* content)
{
    switch (option->type)
    {
        case TEXT:
            option->content = (char *)content;
            break;
    
        case ERROR:
            option->content = (char *)content;
            break;
    
        case DIRECTORY:
            option->content = (option_t*)content;
            break;

        default:
            option->content = NULL;
            break;
    }        
}

void terminal_print_options(void)
{   
    vga_color_t fg, bg;
    int size, spaces;

    size = terminal->options->size;
    kprint(terminal->title);

    for (int i = 0; i < size; i++) {
        if (i == selected) {
            /* print reversed color text */
            fg = terminal->bg;
            bg = terminal->fg;
        }
        else {
            fg = terminal->fg;
            bg = terminal->bg;
        }

        kprintc("[", fg, bg); 
        kprintc(terminal->options[i].title, fg, bg); 
        kprintc("]", fg, bg); 

        /* number of empty spaces for displaying selected option */
        spaces = (int)(terminal->field_width - strlen(terminal->options[i].title) - 1);

        for (int j = 0; j < spaces; j++)
            kprintc(" ", fg, bg); 
        kprintc("\n", fg, bg); 
    }
}

void terminal_print_content(void)
{
    int temp_selected; 
    option_t *suboptions;

    __kclear();
    kprint(terminal->title);

    /* set missing option */
    if (terminal->options[selected].content == NULL) {
        terminal_set_option_content(&terminal->options[selected], "--ERROR MISSING DATA--");
        terminal->options[selected].type = ERROR;
    }

    switch (terminal->options[selected].type) {
        case TEXT:
            kputs((char*)terminal->options[selected].content);
            break;

        case DIRECTORY:
            __kclear();
            
            temp_selected = selected;
            suboptions    = (option_t*)terminal->options[temp_selected].content;
            
            selected = 0;
            terminal->prev_options = terminal->options;
            terminal->options = suboptions;
            terminal_print_options();
            selected = 0;
            break;

        case ERROR:
            kputs((char*)terminal->options[selected].content);
            break;
        
        default:
            kputs("--UNKNOWN CONTENT TYPE--");
            break;
    }
}
