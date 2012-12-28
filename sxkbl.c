#include <ctype.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/XKBlib.h>

#include "arg.h"
#include "util.h"

/* function declaration */
static void usage();

/* variables */
char *argv0;
static Display *dpy;
static XkbDescRec *kb;
static XkbStateRec state;

void
usage() {
	die("usage: %s [-l long] [-u upper] [-v]\n", basename(argv0));
}

int
main(int argc, char *argv[]) {
	Bool lflag, uflag;
	char layout[80];
	int i;

	lflag = uflag = False;

	ARGBEGIN {
	case 'l':
		lflag = True;
		break;
	case 'u':
		uflag = True;
		break;
	case 'v':
		die("syntog-"VERSION", © 2012 Peter A. Shevtsov"
				", see LICENSE for details.\n");
	default:
		usage();
	} ARGEND;

	if(!(dpy = XOpenDisplay(NULL)))
		die("sxkbl: cannot open display\n");

	if(!(kb = XkbAllocKeyboard()))
		die("sxkbl: cannot allocate keyboard\n");

	if(XkbGetNames(dpy, XkbGroupNamesMask, kb) != Success)
		die("sxkbl: error getting names\n");

	if(XkbGetState(dpy, XkbUseCoreKbd, &state) != Success)
		die("sxkbl: error getting state\n");

	strncpy(layout, XGetAtomName(dpy, kb->names->groups[state.group]),
			sizeof layout);

	if(uflag)
		for(i = 0; layout[i]; i++)
			layout[i] = toupper(layout[i]);

	if(lflag)
		printf("%s\n", layout);
	else
		printf("%.2s\n", layout);

	XkbFreeNames(kb, XkbGroupNamesMask, True);
	XkbFreeKeyboard(kb, 0, True);
	XCloseDisplay(dpy);
	return EXIT_SUCCESS;
}
