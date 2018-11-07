/*************************************************************************************/
//											CE437   																			  
//									  Nikolas Koxenoglou																		
// 						implemented readline and autocomplete functions		   																														 
/*************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "readline_functions.h"

char *commands_list[]= {
	// my commands //
	"ls", "less", "history", "quit", "cube_intersect_2", "supercube_2", 
	"distance_2", "cube_cover_2", "sharp_2", "sharp", "off_f",
	// standard tcl command for TCL8.6//
	"after", "errorInfo", "load", "re_syntax", "tcl_startOfNextWord",
	"append","eval","lrange","read","tcl_startOfPreviousWord",
	"apply","exec","lrepeat","refchan","tcl_traceCompile",
	"argc","exit","lreplace","regexp","tcl_traceExec",
	"argv","expr","lreverse","registry","tcl_version",
	"argv0","fblocked","lsearch","regsub","tcl_wordBreakAfter",
	"array","fconfigure","lset","rename","tcl_wordBreakBefore",
	"auto_execok","fcopy","lsort","return","tcl_wordchars",
	"auto_import","file","mathfunc","safe","tcltest",
	"auto_load","fileevent","mathop","scan","tell",
	"auto_mkindex","filename","memory","seek","throw",
	"auto_path","flush","msgcat","self","time",
	"auto_qualify","for","my","set","tm",
	"auto_reset","foreach","namespace","socket","trace",
	"bgerror","format","next","source","transchan",
	"binary","gets","nextto","split","try",
	"break","glob","oo::class","string","unknown",
	"catch","global","oo::copy","subst","unload",
	"cd","history","oo::define","switch","unset",
	"chan","http","oo::objdefine","tailcall","update",
	"clock","if","oo::object","Tcl","uplevel",
	"close","incr","open","tcl::prefix","upvar",
	"concat","info","package","tcl_endOfWord","variable",
	"continue","interp","parray","tcl_findLibrary","vwait",
	"coroutine","join","pid","tcl_interactive","while",
	"dde","lappend","pkg::create","tcl_library","yield",
	"dict","lassign","pkg_mkIndex","tcl_nonwordchars","yieldto",
	"encoding","lindex","platform","tcl_patchLevel","zlib",
	NULL
};

// initialize readline, set custom completer and history//
void init_readline(){

	// use the default file name completer //
	rl_completion_entry_function = NULL;

	rl_attempted_completion_function = custom_completer;

	rl_completion_append_character = '\0';

	// initialise history function // 
	using_history();
}

// complete commands with target for completion text bounded //
// by start and end  //
char **custom_completer(const char *text, int start, int end)
{
	char **match;

	match = (char **)NULL;

	// detect if the word to be completed is a command ( word at start of line)
	// or it is the name of a file in current directory 
	if (!start) {
		match = rl_completion_matches(text, command_gen);
	}
	// if null then call default readline //
	return match;
}

// generate command for tab completetion, use state to find out if //
// we need to start from scratch or just the top of the list //
char *command_gen(const char *text, int state)
{
	// index is static in order to keep state //
	static int index_list;
	// input text length //
	static int length;

	char *name = NULL;

	// initialize word, if this is a new word to complete, and //
	// set index to 0 //
	if (!state) {
		index_list = 0;
		length = strlen(text);
	}

	// match name from command list until NULL //
	while( (name = commands_list[index_list]) )
	{
		index_list++;

		// should a much be found , return it //
		if ( strncmp(name, text, length) == 0 ) 
		{
			// if found, return string //
			return strdup(name);
		}
	}
	free (name);
	// return NULL when reached the end //
	return NULL;
}

// strip whitespace from the start and end of STRING // 
// return a pointer into STRING. //
char *stripwhite (char *string)
{
	register char *s, *t;

	// scans for whitespace characters //
	for (s = string; isspace (*s); s++);

	// if the string is empty return //
	if (*s == 0)
	{
		return (s);
	}

	// remove whitespace from the start of the string //
	t = s + strlen (s) - 1;
	while (t > s && isspace (*t))
	{
		t--;
	}
	// add \0 to the end //
	*++t = '\0';

   return s;
}