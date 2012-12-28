#include <libgen.h>
#include <stdio.h>
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
	char *layout;
	char *ptr;

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

	layout = XGetAtomName(dpy, kb->names->groups[state.group]);

	if(uflag)
		for(ptr=layout;*ptr;++ptr)
			if (*ptr >= 'a' && *ptr <= 'z')
				*ptr-=32;

	if(!lflag)
		layout[2] = '\0';

	printf("%s\n", layout);

	XkbFreeNames(kb, XkbGroupNamesMask, True);
	XkbFreeKeyboard(kb, 0, True);
	XCloseDisplay(dpy);
	return 0;
}
