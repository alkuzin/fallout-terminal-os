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

#ifndef _LIBFTERM_LOGIN_H_
#define _LIBFTERM_LOGIN_H_

#include <libfterm/utils.h>
#include <libk/stddef.h>
 
#define MAX_ATTEMPTS   5
#define INPUT_SIZE     10
#define PASSW_MAX_LEN  16
#define STRING_ROWS    12
#define STRING_SIZE    16
#define LOCK_OUT_TIME  10 /* 10 sec. */

/* the most secret password ever */
#define SUPER_SECRET_PASSW      "12345"
#define SUPER_SECRET_PASSW_SIZE 5

typedef struct login_s {
    char title[TERMINAL_TITLE_SIZE];
    char password[PASSW_MAX_LEN + 1];
    int  attempts;
} login_t;

extern login_t *login_window;

/* initialize user authorisation */
bool login_init(const char *title);

/* successful user authorisation window*/ 
void login_success(const char *success_msg);

/* failed user authorisation window */
void login_failure(const char *failure_msg);

#endif /* _LIBFTERM_LOGIN_H_ */ 
