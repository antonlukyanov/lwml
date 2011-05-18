http://conio.sourceforge.net/
http://sourceforge.net/projects/conio/
http://conio.cvs.sourceforge.net/viewvc/conio/conio2/

--

CONIO introduction

What is CONIO library good for?

The library is designed for simple output in text console. Imagine that you want to write to console window at position (10, 10) text "Hello" in a yellow color on a blue background. With CONIO you can do this with the C code: 

gotoxy(10, 10); 
textcolor(YELLOW); 
textbackground(BLUE); 
cputs("Hello"); 
or even simpler with C++ equivalent: 

cout << setxy(10, 10) << setclr(YELLOW) << setbk(BLUE) << "Hello";

--

CONIO Documentation

Borland-style CONIO implementation for MinGW/Dev-C++.

Send any improvements to this library to me, I'll do new release of this devpak.

For the example of use, look at example in the Examples\conio\conio_test.c subdirectory 
of your Dev-C++ directory. It's simple: 

Include conio2.h. 

Link with libconio.a (add -lconio parameter to linker). 

Functions defined already in MinGW's conio.h

conio2.h automatically includes conio.h. It also provides several #defines so you can 
use all these functions without underscores. 

char* _cgets (char*);
int _cprintf (const char*, ...);
int _cputs (const char*);
int _cscanf (char*, ...);

int _getch (void);
int _getche (void);
int _kbhit (void);
int _putch (int);
int _ungetch (int);

int getch (void);
int getche (void);
int kbhit (void);
int putch (int);
int ungetch (int);

Author:

Hongli Lai <hongli@telekabel.nl> 

tkorrovi <tkorrovi@altavista.net> on 2002/02/26. 

Andrew Westcott <ajwestco@users.sourceforge.net> 

Michal Molhanec <michal@molhanec.net>

Version: 2.0
