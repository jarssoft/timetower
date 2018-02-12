#include <stdlib.h>
#include <argp.h>
#include <string>
#include <iostream>
#include <fstream>
#include "xmltv.h"
#include "datetime.h"
#include "sending.h"

const char *argp_program_version =
  "argp-ex3 1.0";
const char *argp_program_bug_address =
  "<bug-gnu-utils@gnu.org>";

/* Program documentation. */
static char doc[] =
  "Argp example #3 -- a program with options and arguments using argp";

/* A description of the arguments we accept. */
static char args_doc[] = "[tvxml-file1 tvxml-file2...]";

/* The options we understand. */
static struct argp_option options[] = {
  {"output",   'o', "FILE",     0, "Output to FILE instead of standard output" },
  {"stdin",    '@', 0,          0, "Read XML filenames from standard input" },
  {"channel",  'c', "CHANNEL" , 0, "Select channel. Use if several" },
  {"timezone", 'z', "TIMEZONE", 0, "Timezone correction in hours" },
  {"week",     'w', "WEEK",     0, "Number of week" },
  {"svg",      's', 0,          0, "Output in svg-format instead of html." },
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
  char *args[7];                /* arg1 & arg2 */
  char *output_file;
  bool namesfromstdin;
  bool svgoutput;
  char *channel;
  int timezone;
  int week;
};

int argcount=0;

/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = (struct arguments *)state->input;

  switch (key)
    {
    case 'o':
      arguments->output_file = arg;
      break;
    case 'c':
      arguments->channel = arg;
      break;
    case 'z':
      arguments->timezone = atoi(arg);
      break;
    case 'w':
      arguments->week = atoi(arg);
      break;
    case 's':
      arguments->svgoutput = true;
      break;
    case '@':
      arguments->namesfromstdin = true;
      break;
    case ARGP_KEY_ARG:
      if (state->arg_num >= 7)
        /* Too many arguments. */
        argp_usage (state);

      arguments->args[state->arg_num] = arg;
      if(state->arg_num+1 > argcount)
          argcount=state->arg_num+1;

      break;

    case ARGP_KEY_END:
      if (state->arg_num < 0)
        /* Not enough arguments. */
        argp_usage (state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };

static void run(struct arguments* arguments){

    setMakeSVG(arguments->svgoutput);
    adjust_timezone(arguments->timezone);
    setChannel(arguments->channel);
    setWeek(arguments->week);

    if(arguments->namesfromstdin){
        std::string line;
        while (std::getline(std::cin, line))
        {
            importXML(line.c_str());
            //printf ("ARG = %s\n", line.c_str());
        }
    }else{
        for(int i=0;i<argcount;i++)
            //printf ("ARG%d = %s\n", i, arguments.args[i]);
            importXML(arguments->args[i]);
    }

    if(arguments->output_file=="-"){
        std::ostream& out = std::cout;
        if(arguments->svgoutput){
            exportSVG(out);
        }else{
            exportHTML(out);
        }
    }else{
        std::ofstream fout;
        fout.open (arguments->output_file);
        if(arguments->svgoutput){
            exportSVG(fout);
        }else{
            exportHTML(fout);
        }
    }
}

int main(int argc, char **argv)
{
  struct arguments arguments;

  /* Default values. */
  arguments.output_file = "-";
  arguments.namesfromstdin = false;
  arguments.svgoutput = false;
  arguments.channel = "";
  arguments.timezone = 0;
  arguments.week = -1;

  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  //for(int i=0;i<argcount;i++)
      //printf ("ARG%d = %s\n", i, arguments.args[i]);

  /*
  printf ("OUTPUT_FILE = %s\nSTDIN = %s\nCHANNEL=%s\nTIMEZONE=%d\nWEEK=%d\n",
          arguments.output_file,
          arguments.namesfromstdin ? "yes" : "no",
          arguments.channel,
          arguments.timezone,
          arguments.week);
          //*/

  run(&arguments);

  exit (0);
}
