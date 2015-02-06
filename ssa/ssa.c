/* 
   ssa - Simple Set of Accounts

   Copyright (C) 2014 mark carter

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  

#include <termios.h>
#include <grp.h>
#include <pwd.h>
*/

#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <argp.h>
#include <stdlib.h>

#include "commands.h"
#include "csv.h"
#include "data.h"
#include "financials.h"
#include "ofx.h"
#include "epics.h"
#include "portfolios.h"
#include "prolog.h"


#define N_(Text) Text
#define PACKAGE "ssa"
//#define VERSION "X" // set in compiling



static error_t parse_opt (int key, char *arg, struct argp_state *state);
static void show_version (FILE *stream, struct argp_state *state);

/* argp option keys */
enum {DUMMY_KEY=129
};

/* Option flags and variables.  These are initialized in parse_opt.  */

int want_interactive;		/* --interactive */
int want_snapshot;
int want_verbose;		/* --verbose */
int want_web; // --web

static struct argp_option options[] =
{
  { "interactive", 'i',           NULL,            0,
    N_("Prompt for confirmation"), 0 },
  { "snapshot", 's', NULL, 0, N_("Print s snapshot of today's performance"), 0},
  { "verbose",     'v',           NULL,            0,
    N_("Print more information"), 0 },
  { "web", 'w', NULL, 0, N_("Download prices from web (Google)"), 0},

  { NULL, 0, NULL, 0, NULL, 0 }
};

/* The argp functions examine these global variables.  */
const char *argp_program_bug_address = "<devnull@markcarter.me.uk>";
void (*argp_program_version_hook) (FILE *, struct argp_state *) = show_version;

static struct argp argp =
{
  options, parse_opt, N_("[FILE...]"),
  N_("Simple Set of Accounts"),
  NULL, NULL, NULL
};






int main (int argc, char **argv)
{
  init_data();
  argp_parse(&argp, argc, argv, 0, NULL, NULL);
  setlocale(LC_NUMERIC, "");

  parse_rc_file();
  init_dirs();
  
  if(want_web || want_snapshot) { download_prices(); }
  // parse_rc_subdir("gofi"); // Do it after download of prices, otherwise they wont be picked up
  derive_data();
  print_etb();
  report_etrans();
  create_csvs();
  create_portfolios();
  report_epics();
  //parse_data_file1("financials.txt");
  create_financials();
  report_returns();
  create_ofx();
  export_prolog();
  if(want_snapshot) { create_snapshot(); }
  dump_data();
  free_resources();
  
  exit (0);
}

/* Parse a single option.  */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  switch (key)
    {
    case ARGP_KEY_INIT:
      /* Set up default values.  */
      want_interactive = 0;
      want_verbose = 0;
      break;

    case 'i':			/* --interactive */
      want_interactive = 1;
      break;

    case 's':
	want_snapshot = 1;
	break;
    case 'v':			/* --verbose */
      want_verbose = 1;
      break;

    case 'w': // web
	want_web = 1;
	break;
    case ARGP_KEY_ARG:		/* [FILE]... */
      /* TODO: Do something with ARG, or remove this case and make
         main give argp_parse a non-NULL fifth argument.  */
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Show the version number and copyright information.  */
static void
show_version (FILE *stream, struct argp_state *state)
{
  (void) state;
  /* Print in small parts whose localizations can hopefully be copied
     from other programs.  */
  //fputs(PACKAGE" "VERSION"\n", stream);
  fprintf(stream, "%s: %s\n", PACKAGE, VERSION);
  fprintf(stream, "Written by %s.\n\n", "mark carter");
  fprintf(stream, "Copyright (C) %s %s\n", "2014", "mark carter");
  fputs("\
This program is free software; you may redistribute it under the terms of\n\
the GNU General Public License.  This program has absolutely no warranty.\n",
	stream);
}
