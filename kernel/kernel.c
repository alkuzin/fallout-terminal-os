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
#include <kernel/multiboot.h>
#include <kernel/keyboard.h>
#include <libfterm/login.h>
#include <libfterm/utils.h>
#include <kernel/kernel.h>
#include <kernel/timer.h>
#include <libk/memory.h>
#include <kernel/sstd.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/mm.h>
#include <shell/ksh.h>

login_t    *login_window;
terminal_t *terminal;

static void kboot(multiboot_t *boot_info)
{
    /* initializing Global Descriptor Table */
    gdt_init();
    kprints(" kernel: initialized Global Descriptor Table \n", DEFAULT_DELAY);	
    
    /* initializing Interrupt Descriptor Table */
    idt_init();
    kprints(" kernel: initialized Interrupt Descriptor Table \n", DEFAULT_DELAY);	
    
    /* initializing timer */
    timer_init();
    kprints(" kernel: initialized timer \n", DEFAULT_DELAY);	

    /* initializing memory management */
    memory_init(boot_info);
    kprints(" kernel: initialized memory management\n", DEFAULT_DELAY);	

    is_terminal_set = false;

    keyboard_init();
    kprints(" kernel: initialized keyboard\n", DEFAULT_DELAY);	

    /* TODO: ROBCO boot entry */

}

/* kernel entry point */
extern void kmain(__attribute__((unused)) uint32_t magic, multiboot_t *boot_info)
{
    vga_color_t fg, bg;

    fg = VGA_COLOR_LIGHT_CYAN;
    bg = VGA_COLOR_BLACK;
    __set_default_color(fg, bg);

	__kclear(); /* clear screen */
    kboot(boot_info); /* boot kernel */
    
    // set default settings TODO: make macros for success_msg & failure_msg 
    const char *terminal_title = 
    "==== Institute Central Network ====\n"
    "Advanced Systems Terminal 1A     \n\n";
    
    const char *success_msg = "\nLOGIN AS ADMINISTRATOR\n"
        "PASSWORD: * * * * *\n\nSTATUS: COMPLETE\n\n\n";
    
    const char *failure_msg = "\n\n\n\n\n\t\t\t\t\t\t\t\t TERMINAL LOCKED\n"
        "\t\t\t\t\t\t\t\tPLEASE CONTACT AN\n"
        "\t\t\t\t\t\t\t\t  ADMINISTRATOR";

    // initialize login process
    if (login_init("ROBCO INDUSTRIES (TM) TERMLINK PROTOCOL\n")) {
        login_success(success_msg);
    }
    else {
        login_failure(failure_msg);
    }
	
    __kclear(); /* clear screen */

    // set options
    option_t options[3];
    option_t opt_list_personal_notes[2];

    opt_list_personal_notes[0] = (option_t) {
        .title="Cold Fusion",
        .content=
        "Proposal:\n\n"
        "Divert time/materials from current Phase Three research to exploring the\n"
        "possibility of sustainable nuclear reactions through electrochemical processes at\n"
        "or near room temperatures. Pre-war work on the subject yielded no concrete\n"
        "results; advances in technology could potentially make it possible.\n\n"
        "Approval:\n\n"
        "Rejected - Li.M\n"
        "Notes: Evidence suggests this is, and always will be, a pipe dream.\n",
        .type=TEXT,
        .size=2
    };

    opt_list_personal_notes[1] = (option_t) {
        .title="Surface Work Crew Tracking",
        .content=
        "Group Ident: G1486\n"
        "Assigned Units: R7-81, T3-54, B8-88, J1-74\n"
        "Location: Site Alpha\n"
        "Elapsed Time: 160:33:12\n"
        "Last Report: 1:56:40\n\n"
        "NOTES:\n"
        "Unit B8-88 removed, unit T3-54 added. [ERROR, NO DATA PRESENT]\n",
        .type=TEXT,
        .size=2
    };

    options[0] = (option_t) {
        .title="View Access Logs",
        .content= 
        "Access: Local. Login: Li.M\n"
        "Notes: Review of Phase Three project status\n\n"
        "Access: Local. Login: Li.M\n"
        "Notes: update diagnostic tools for child synth project\n\n"
        "Access: Remote. Login: DIRECTOR\n"
        "Notes: --REDACTED--\n\n"
        "Access: Local. Login: Li.M\n"
        "Notes: Review of all BioScience projects\n\n\n",
        .type=TEXT,
        .size=3
    };

    options[1] = (option_t) {
        .title="Incident Reports",
        .content=
        "   ****Incident Reports****\n\n\n"
        "Incident IN2845t\n\n"
        "Patient: Thompson.E\n"
        "Presenting symptom: First-degree burns, left hand\n"
        "Notes: Patient placed his hand inside power\n"
        "relay without first disengaging. First aid applied.\n",     
        .type=TEXT,
        .size=3
    };

    options[2] = (option_t) {
        .title="Personal Notes", 
        .content=&opt_list_personal_notes,
        .type=DIRECTORY,
        .size=3
    };
    
    terminal->fg = fg;
    terminal->bg = bg;

    // initializing terminal
    terminal_init(terminal_title, DEFAULT_FIELD_WIDTH, options);

	for(;;); /* infinite loop for halting CPU */
}
