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
const char *success_msg = "\nLOGIN AS ADMINISTRATOR\n"
    "PASSWORD: * * * * *\n\nSTATUS: COMPLETE\n\n\n";

const char *failure_msg = "\n\n\n\n\n\t\t\t\t\t\t\t\t TERMINAL LOCKED\n"
    "\t\t\t\t\t\t\t\tPLEASE CONTACT AN\n"
    "\t\t\t\t\t\t\t\t  ADMINISTRATOR";

void institute_terminal_init(multiboot_t *boot_info);
//void bos_terminal_init(multiboot_t *boot_info);
void mass_fusion_terminal_init(multiboot_t *boot_info);

void mass_fusion_terminal_init(multiboot_t *boot_info)
{
    vga_color_t fg, bg;

    fg = VGA_COLOR_LIGHT_GREEN;
    bg = VGA_COLOR_BLACK;
    __set_default_color(fg, bg);

	__kclear(); /* clear screen */
    kboot(boot_info); /* boot kernel */
    
    const char *terminal_title = 
    "Mass FusionOS v4.86\n"
    "Main Reactor - Prototype Engineering\n\n\n";

    // initialize login process
    if (login_init("ROBCO INDUSTRIES (TM) TERMLINK PROTOCOL\n"))
        login_success(success_msg);
    else
        login_failure(failure_msg);
	
    __kclear(); /* clear screen */

    // set options
    static option_t options[3];
    static option_t suboptions[2];

    suboptions[0] = (option_t) {
        .title="Activate",
        .content=
        "STATUS: Unit(s) Charging/Inactive\n"
        "CONNECTED: 1 Unit(s) Connected to this terminal\n\n"
        "Activate Unit(s)\n"
        "\n...Accessing pod...\n"
        "...Initializing unit...\n"
        "...Loading assigned subroutines...\n\n"
        "Please advise any personnel standing near charge pod to make way.\n",
        .type=TEXT,
        .size=2
    };

    suboptions[1] = (option_t) {
        .title="Shutdown",
        .content=
        "STATUS: Unit(s) Charging/Inactive\n"
        "CONNECTED: 1 Unit(s) Connected to this terminal\n\n"
        "Shut-Down Unit(s)\n"
        "\n...Accessing pod...\n"
        "...Pinging Protectron Unit...\n"
        "...Broadcasting shut-down signal...\n\n"
        "Please ensure that Charge Pod is unobstructed.\n",
        .type=TEXT,
        .size=2
    };

    options[0] = (option_t) {
        .title="Research IntraMail 07-30-77",
        .content=
        "To: Reactor Team\n"
        "From: N. Chandrich, Research Lead\n\n"
        "Today we embark on a new frontier. Once the switch on the reactor is thrown,\n"
        "and the Beryllium Agitator cold-starts the fusion reaction, the reality of \n"
        "Cleanpower nuclear energy will be upon us all. As the research lead on the \n"
        "project, I'd like to thank you all for contributing to the cause.\n"
        "Good luck and godspeed!\n",
        .type=TEXT,
        .size=5
    };

    options[1] = (option_t) {
        .title="Personal IntraMail 8-01-77",
        .content=
        "To: D. Wahbash, X. Vincent, C. Collins\n"
        "From: B. Thorpe, Prototype Engineering\n\n"
        "Tamash reaches into his pouch and sprinkles his communing dust in a circle \n"
        "around himself. He closes his eyes and calls upon the Knowledge \n"
        "of Deeligasa. \"Deeligasa, Sage of the Ages, show me what my eyes can't see!\"\n",
        .type=TEXT,
        .size=5
    };

    options[2] = (option_t) {
        .title="Personal IntraMail 8-02-77",
        .content=
        "To: D. Wahbash, X. Vincent, C. Collins\n"
        "From: B. Thorpe, Prototype Engineering\n\n"
        "Tamash stops casting and switches to his Staff of Brilliant Sunlight.\n"
        "He incants the words and raises the staff high over his head to \n"
        "illuminate the room. He proudly yells aloud \n"
        "\"Dark foe, prepare to be revealed!\"\n",
        .type=TEXT,
        .size=5
    };

    options[3] = (option_t) {
        .title="Personal IntraMail 8-03-77",
        .content=
        "To: D. Wahbash, X. Vincent, C. Collins\n"
        "From: B. Thorpe, Prototype Engineering\n\n"

        "Tamash drops the staff, reaches into his left front pocket and \n"
        "pulls out a pinch of sulfur powder. He incants the Flameblaze spell \n"
        "and blows into the pile of powder towards the beast. If it works, \n"
        "the things has to make a -5 fire check.\n",
        .type=TEXT,
        .size=5
    };

    options[4] = (option_t) {
        .title="Protectron Control",
        .content=&suboptions,
        .type=DIRECTORY,
        .size=5
    };

    terminal->fg = fg;
    terminal->bg = bg;

    // initializing terminal
    terminal_init(terminal_title, DEFAULT_FIELD_WIDTH, options);
}
    
void institute_terminal_init(multiboot_t *boot_info)
{
    vga_color_t fg, bg;

    fg = VGA_COLOR_LIGHT_CYAN;
    bg = VGA_COLOR_BLACK;
    __set_default_color(fg, bg);

	__kclear(); /* clear screen */
    kboot(boot_info); /* boot kernel */
    
    const char *terminal_title = 
    "==== Institute Central Network ====\n"
    "SYNTH RETENTION BUREAU TERMINAL 2B\n\n";
    
    // initialize login process
    if (login_init("ROBCO INDUSTRIES (TM) TERMLINK PROTOCOL\n"))
        login_success(success_msg);
    else
        login_failure(failure_msg);
	
    __kclear(); /* clear screen */

    // set options
    static option_t options[3];
    static option_t suboptions[3];

    suboptions[0] = (option_t) {
        .title="Brotherhood of Steel",
        .content=
        "BROTHERHOOD OF STEEL\n\n"
        "Threat level: Primary\n\n"
        "Brotherhood forces recently arrived in strength. believed to \n"
        "be constructing new weapon that could pose direct threat \n"
        "to Institute... Boston Airport now under Brotherhood control... \n"
        "Surface operators should avoid this area.\n",
        .type=TEXT,
        .size=3
    };

    suboptions[1] = (option_t) {
        .title="Railroad",
        .content=
        "RAILROAD\n\n"
        "Threat level: Secondary\n\n"
        "Railroad continues to be elusive. Preventing synth escapes is \n"
        "vital to countering Railroad operations. Possible Railroad agent \n"
        "or sympathizer within Institute. Continue to press informants for \n"
        "new information.\n",
        .type=TEXT,
        .size=3
    };

    suboptions[2] = (option_t) {
        .title="Raider Gangs",
        .content=
        "RAIDER GANGS\n\n"
        "Threat level: Tertiary\n\n"
        "Raider gangs remain disorganized and scattered. major areas of \n"
        "activity include Libtertalia, Corvega, Saugus Ironworks, \n"
        "Beantown Brewery, D.B. Technical High School and Hyde Park. \n"
        "extreme caution is warranted when operating in these areas.\n",
        .type=TEXT,
        .size=3
    };

    options[0] = (option_t) {
        .title="Threat Assessment", 
        .content=&suboptions,
        .type=DIRECTORY,
        .size=3
    };
    
    options[1] = (option_t) {
        .title="Watcher Allocation",
        .content= 
        "CURRENT WATCHER ALLOCATION\n\n"
        "Diamond City: 4 pods\n"
        "Fort Hagen: 2 pods\n"
        "Warwick Homestead: 1 pod\n"
        "Goodneighbor: 3 pods\n"
        "Covenant: 1 pod\n"
        "Vault 81: 1 pod\n"
        "Libertalia: 2 pods\n",
        .type=TEXT,
        .size=3
    };

    options[2] = (option_t) {
        .title="View Access Logs",
        .content=
        "ACCESS LOG\n\n"
        "Access: Local. Login: Secord.A\n"
        "Accessed Watcher Allocation\n\n"
        "Access: Local. Login: Ayo.J\n"
        "Operation: Update Threat Assessment\n\n"
        "Access: Local. Login: X6-88\n"
        "Accessed Threat Assessment\n\n"
        "Access: Local. Login: Ayo.J\n"
        "Operation: Update Watcher Allocation\n\n"
        "Access: Local. Login: Secord.A\n"
        "Accessed Threat Assessment\n",
        .type=TEXT,
        .size=3
    };

    terminal->fg = fg;
    terminal->bg = bg;

    // initializing terminal
    terminal_init(terminal_title, DEFAULT_FIELD_WIDTH, options);
}

void bos_terminal_init(multiboot_t *boot_info)
{
    vga_color_t fg, bg;

    fg = VGA_COLOR_YELLOW;
    bg = VGA_COLOR_BLACK;
    __set_default_color(fg, bg);

	__kclear(); /* clear screen */
    kboot(boot_info); /* boot kernel */
    
    const char *terminal_title = 
    "Prydwen Internal Network\n"
    "General Information System - Official Use Only\n\n";
    
    // initialize login process
    if (login_init("ROBCO INDUSTRIES (TM) TERMLINK PROTOCOL\n"))
        login_success(success_msg);
    else
        login_failure(failure_msg);
	
    __kclear(); /* clear screen */

    // set options
    static option_t options[6];

    options[0] = (option_t) {
        .title="Announcement 001", 
        .content=
        "All recovered technology must be presented immediately to \n"
        "Proctor Quinlan for evaluation. Anyone found in possession of \n"
        "undocumented technologies will be subject to inquiry and punishment.\n",
        .type=TEXT,
        .size=5
    };

    options[1] = (option_t) {
        .title="Announcement 002", 
        .content=
        "Scribe Neriah's Mole Rat subjects are not pets. They are crucial \n"
        "military resources. As such, they are not to be fed by any personnel \n"
        "save Scribe Neriah and her team. Any violation of this policy will \n"
        "result in severe disciplinary action.\n",
        .type=TEXT,
        .size=5
    };

    options[2] = (option_t) {
        .title="Announcement 003", 
        .content=
        "Due to increased Raider activity, all leave requests to off-base sites \n"
        "have been temporarily suspended. Affected parties will have their leave \n"
        "compensated at the next available opportunity.\n",
        .type=TEXT,
        .size=5
    };

    options[3] = (option_t) {
        .title="Announcement 004", 
        .content=
        "This is a notice to all personnel. Please think of your brothers and \n"
        "sisters down below. Thoroughly secure all gear while on the Flight Deck.\n",
        .type=TEXT,
        .size=5
    };

    options[4] = (option_t) {
        .title="Announcement 005", 
        .content=
        "Any personnel who have not completed their annual medical evaluation \n"
        "should report to Knight-Captain Cade immediately to schedule an examination.\n",
        .type=TEXT,
        .size=5
    };

    terminal->fg = fg;
    terminal->bg = bg;

    // initializing terminal
    terminal_init(terminal_title, DEFAULT_FIELD_WIDTH, options);
}

/* kernel entry point */
extern void kmain(__attribute__((unused)) uint32_t magic, multiboot_t *boot_info)
{
    //institute_terminal_init(boot_info);
    //mass_fusion_terminal_init(boot_info);
    bos_terminal_init(boot_info);

	for(;;); /* infinite loop for halting CPU */
}
