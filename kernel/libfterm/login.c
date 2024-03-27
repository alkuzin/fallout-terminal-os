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

#include <libfterm/login.h>
#include <kernel/keyboard.h>
#include <libk/stdint.h>
#include <libk/stddef.h>
#include <libk/memory.h>
#include <libk/string.h>
#include <kernel/sstd.h>
#include <libk/ctype.h>
#include <kernel/tty.h>
 
/* get string input from user*/
static void getinput(char* buffer);

/* show number of attempts left */
static void show_attempts(void);

/* check if password is correct */
static bool is_correct(const char *password);

/* lock out terminal in case of loggin in failure */
static void lock_out_terminal(const char *lock_out_msg);

static char *memory_dump[193] = 
{
"|?_}|[+=}|^!", "]+,,|[]+=||,", 
"&*@_]*[]!$ .", "-,{, | ^=,{ ", 
"&*_]/+|!{/?<", "-/^^{/^}!-^*", 
"-*/|.||[.}=*", ">+.[(^(_,-^^", 
"&*+}=?$+/.[<", ">.{.[//|]-+.", 
"-*.[$[||/}()", "&+[&}/)[^-+*",
")*=,,.|]/&//", "&^^+[/{[_--&",
".*^+{_|([]+(", ">)}_[]]{_-^&", 
".*]{}?$[] ]*", ")?{]+(&[+-^|", 
"^*)^)(|[]+=.", ")[^]?^+](-^_", 
".*{}[,|[.^)_", ">([]/=?]!-./", 
"&*]^?{|=){|-", "..(+/.+_&-._", 
"**[|)&$/}]_&", ".(_+{-[[}-._", 
"[*,[.}|?]/{@", "_/)${,]!+-^&", 
"$*]]].|![{^#", "*=[+(_]/]-^.", 
"$*+&.[|[_[+.", "*? ^+[}{[-^/"
};

bool login_init(const char *title)
{
    char password[PASSW_MAX_LEN + 1];
    const int hexnumber = 0x34DF;
    int temp_hexnumber;

    bzero(login_window, sizeof(login_window));
    bzero(password, sizeof(password));

    strlcpy(login_window->password, SUPER_SECRET_PASSW, SUPER_SECRET_PASSW_SIZE);
    strlcpy(login_window->title, title, TERMINAL_TITLE_SIZE);
    login_window->attempts = MAX_ATTEMPTS;


    while (login_window->attempts >= 0) {

        __kclear();
		show_attempts();
        
        if(login_window->attempts != MAX_ATTEMPTS)
            kprintf("INCORRECT PASSWORD: '%s'\n\n", password);
        
        temp_hexnumber = hexnumber;

        for(int i = 1; i < STRING_ROWS; i++) {
            kprintf("%#x %s %#x %s \n", temp_hexnumber,
					memory_dump[2*(i-1)] , temp_hexnumber + 24, memory_dump[2*i-1]);
            temp_hexnumber += STRING_SIZE;
        }
        
        temp_hexnumber = hexnumber;
        kprintf("%#x .*]{}?$[] ]* %#x -^{/)-user-$ > ", temp_hexnumber, temp_hexnumber + 24);
        
        getinput(password);

		if(is_correct(password))
            return true;
        
        if (login_window->attempts == 0)
            break;

        --login_window->attempts;
    }   

    return false;
}

void login_success(const char *success_msg)
{
    uint8_t cc;

    __kclear();

    kputs(login_window->title);
	kprints(success_msg, DEFAULT_DELAY);

	kputs("[press <enter> to continue]");
    
    do {
        cc = keyboard_getchar();
    } while(cc != '\n');
}

void login_failure(const char *failure_msg)
{
    __kclear();
    lock_out_terminal(failure_msg);
}

static void lock_out_terminal(const char *lock_out_msg)
{
    kprints(lock_out_msg, DEFAULT_DELAY);
    kprint("\n\n\n\t\t\t\t\t\t\t[terminal locked for 10 sec.]\n\n\n\n\n\n\n\n");
    
    ksleep(LOCK_OUT_TIME);
    login_init(login_window->title);
}

static void getinput(char* buffer)
{
    int  i, j;
    char cc;

    bzero(buffer, INPUT_SIZE);
    i = 0;
    j = 0;
    
    do {
        cc = (char)keyboard_getchar();
        if(cc != 0 && cc != '\n') {
            kputchar(cc);
            if(j < INPUT_SIZE) {
                buffer[j] = cc;
                j++;
            }
        }
    } while(cc != '\n');

    buffer[j] = '\n';

    kprintf("buffer: \"%s\"\n", buffer);

    if (buffer) {
        while (buffer[i] != '\n' && buffer[i])
            i++;

        if (buffer[i] == '\n')
            buffer[i] = '\0';
        else {
            cc = keyboard_getchar();

            while (cc != '\n')
                continue;
        }
    }
}

static void show_attempts(void)
{
    kprintf("%sENTER PASSWORD NOW\n\n", login_window->title);
    kprintf("%d Attempt(s) left:", login_window->attempts);

    for (int i = 0; i < login_window->attempts; i++) {
        kputchar(' ');
        kputchar('#');
    }
    kputchar('\n');
    kputchar('\n');
}

static bool is_correct(const char *password)
{
	return ((strncmp(password, login_window->password, PASSW_MAX_LEN) == 0) && 
			(strlen(password) <= PASSW_MAX_LEN));
}
