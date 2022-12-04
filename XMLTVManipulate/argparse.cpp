#include <stdlib.h>
#include <argp.h>
#include <string>
#include <iostream>
#include <fstream>
#include "../XMLTVp/access.h"
#include "manipulate.h"

const char *argp_program_version =
  "argp-ex3 1.0";
const char *argp_program_bug_address =
  "<bug-gnu-utils@gnu.org>";

/* Program documentation. */
static char doc[] =
  "Argp example #3 -- a program with options and arguments using argp";

/* A description of the arguments we accept. */
static char args_doc[] = "[tvxml-file manipulate-file...]";

/* The options we understand. */
static struct argp_option options[] = {
  {"output",     'o', "FILE",       0, "Output to FILE instead of standard output" },
  {"manipulate", 'i', "FILE",       0, "Read manipulation rules from file" },
  {"timeoffset", 'z', "TIMEOFFSET", 0, "Timezone correction in hours" },
  {"autodst",    'a', 0,            0, "Is Automatic Daylight Saving used?" },
  {"channel",    'c', "CHANNEL",    0, "Channel" },
  {"startdate",  'd', "STARTDATE",  0, "Date of start" },
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
  char *args[7];                /* arg1 & arg2 */
  char *output_file;
  char *input_file;
  int  timeoffset;

  char* channel;
  char* startdate;
  bool autodst;
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
    case 'i':
      arguments->input_file = arg;
      break;
    case 'z':
      arguments->timeoffset = atoi(arg);
      break;
    case 'a':
        arguments->autodst = true;
        break;
    case 'c':
      arguments->channel = arg;
      break;
    case 'd':
      arguments->startdate = arg;
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

    initTrans();

    //if(argcount==2){
        initManipulate(arguments->timeoffset, arguments->channel, arguments->startdate, arguments->autodst);
        xmltvp::parseXMLTV(arguments->input_file, arguments->output_file, manipulate);
    //}

    /*
    for(int i=0;i<argcount;i++){
        printf ("ARG%d = %s\n", i, arguments->args[i]);
        //importXML(arguments->args[i]);
    }
    */
    //adjust_timeoffset(arguments->timeoffset);

    /*
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

    }else{
        std::ofstream fout;
        fout.open (arguments->output_file);

    }*/
}

int main(int argc, char **argv)
{

  struct arguments arguments;


  /* Default values. */
  arguments.output_file = "-";
  arguments.input_file = "-";
  arguments.timeoffset = 0;
  arguments.autodst = 0;
  arguments.channel = "";
  arguments.startdate = "";
  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  //for(int i=0;i<argcount;i++)
      //printf ("ARG%d = %s\n", i, arguments.args[i]);

  /*
  printf ("OUTPUT_FILE = %s\nSTDIN = %s\nCHANNEL=%s\nTIMEOFFSET=%d\nWEEK=%d\n",
          arguments.output_file,
          arguments.namesfromstdin ? "yes" : "no",
          arguments.channel,
          arguments.timeoffset,
          arguments.week);
          //*/

  run(&arguments);

  exit (0);
}
