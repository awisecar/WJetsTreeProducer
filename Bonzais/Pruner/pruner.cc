// Pruner application, a utility to copy and skim ROOT ntuple files
// Author: Ph. Gras CEA/IRFU Saclay
// Jan. 3, 11. Cloned Squeezer application from AnaNaS framework.
// Jul. 24, 15

#include "Pruner.h"

#include <iostream>
#include <fstream>
#include <getopt.h>

using namespace std;

void help(){
  cout << "\n"
    "Usage: pruner [OPTIONS] -o OUTPUT_FILE -b BRANCH_FILE INPUT_FILE_1 \n"
    "              [INPUT_FILE_2 ...]\n"
    "       pruner [OPTIONS] -o OUTPUT_FILE -b BRANCH_FILE -c CATALOG\n"
    "\n"
    "Decription: Pruner is the file skimmer of the Shears framework. It selects event\n"
    "and branches of an ntuple, typically a Boabab, and produces a new ntuple,\n"
    "typically a Bonzai.\n"
    "\n"
    "Input files are either provided on the command line or read from a Shears\n"
    "catalog file specified with the -c or --catalog option. The ROOT tree branches\n"
    "to include in the event copies must be specified with the option -b or \n"
    "--branches-from. The BRANCH_FILE must be a text file listing the branch names,\n"
    "one name per line. A template can be generated with the --make-branch-list\n"
    "option which will produce a file including all available branches. Options to \n"
    "select events are described in the following section.\n"
    "\n"
    "SELECTION\n"
    "\n"
    "Event can be selected either by providing a list of events identified by their\n"
    "run number and event id or by an event filter. The list of registered filter can\n"
    "be obtained with the --list-selections. The command line options related to\n"
    "event seleciton are listed below.\n"
    "\n"
    "--list-selections            list available selections and subselections\n"
    "--selection CLASS_NAME       specifies the name of the class to use for the\n"
    "                             event selection.\n"
    "--subselection SUBSELECTION\n"
    "                             a selection class can support several selection\n"
    "                             flavours chosen by this parameter\n"
    "--event-list-from FILE       read list of events to copy from file FILE. Each\n"
    "                             line must contain a run number followed by an event\n"
    "                             number. Line starting with a '#' sign are \n"
    "                             considered as comments and are ignored.\n"
    "\n"
    "\n"
    "GENERAL OPTIONS\n"
    "\n"
    "-v                           increase verbosity. Add more v for more verbosity, \n"
    "                             e.g. -vv.\n"
    "--catalog CATALOG            input files are taken from a Shears catalog file\n"
    "-c        CATALOG\n"
    "--max-events NEVENTS         limit processing to the first NEVENTS events of the\n"
    "                             INPUT_FILE file.\n"
    "--skip-events NEVENTS        skip NEVENTS first events.\n"
    "--max-files NFILES           limit processing to NFILES of the CATALOG.\n"
    "--skip-files NFILES          skip NFILES from the CATALOG.\n"
    "--branches-from FILE         includes the branches listed in FILE in the ouput\n"
    "-b              FILE         tree. Format is one branch per line. Line starting\n"
    "                             with a '#' sign are considered as comments and are\n"
    "                             ignored.\n"
    "--ouput-file FILE            Specifies the file name to write the ouput to. If \n"
    "-o           FILE            the output is text (see make-*-list options), then\n"
    "                             - sign can be used in place of a filename to \n"
    "                             display the result on screeen.\n"
    //    "--primary-dataset PRIMARY_DATASET\n"
    //    "                             provides the primary dataset name of the data contained\n"
    //"                             in the input file. This information is pass to the selection\n"
    //"                             class, which can use it to adapt the selections to the dataset."
       << std::endl;
}

struct Options{
  Options(): verbose(0), help(0), list_events(false), output_file(0),
	     make_event_list(false), make_branch_list(false),
	     event_list_from(0),  branches_from(0), selection(0),
	     subselection(0), primary_dataset(0), list_selections(false), catalog(0),
	     max_events(-1), skip_events(0), max_files(-1), skip_files(0){}
  int verbose;
  int help;
  bool list_events;
  char* output_file;
  bool make_event_list;
  bool make_branch_list;
  const char* event_list_from;
  const char* branches_from;
  const char* selection;
  const char* subselection;
  const char* primary_dataset;
  bool list_selections;
  const char* catalog;
  int max_events;
  int skip_events;
  int max_files;
  int skip_files;
};

int parse_cmd_line(Options& cat, int argc, char* argv[]);

int main(int argc, char* argv[]){
  int rc = 0;

  Options o;
  
  int i = parse_cmd_line(o, argc, argv);
  argc -= i;
  argv += i;

  if(o.help){
    help();
    exit(0);
  }

  //In case of make list event options,
  //forces the use of the default Prune class
  if(o.make_event_list){
    o.selection = 0;
  }

  Pruner* cat = Pruner::create(o.selection, o.subselection, o.primary_dataset);

  if(!cat){
    std::cerr << "Failed to create pruner for selection " << o.selection
	      << ", subselection " << o.subselection 
	      << ", and output file " << o.primary_dataset
	      << ". Available selections can be listed with the --selection option.\n";
    exit(1);
  }

  cat->setVerbosity(o.verbose);

  if(o.make_event_list){
    if((argc < 1 && o.catalog == 0) || (o.catalog != 0 && argc > 0)){
      cerr << "Wrong command usage. See pruner --help." << endl;
      return 1;
    }
    if(o.output_file == 0 || strcmp(o.output_file, "-")==0){
      if(o.catalog) cat->listEvents(cout, o.catalog);
      else cat->listEvents(cout, argc, argv);
    } else{
      ofstream out(o.output_file);
      if(o.catalog) cat->listEvents(out, o.catalog);
      else cat->listEvents(out, argc, argv);
    }
    return 0;
  }

  if(o.make_branch_list){
    if((argc < 1 && o.catalog == 0) || (o.catalog != 0 && argc > 0)){
      cerr << "Wrong command usage. See copyAnatuple --help." << endl;
      return 1;
    }
    
    if(o.output_file == 0 || strcmp(o.output_file, "-")==0){
      cout << argv[0] << endl;
      if(o.catalog) cat->listBranchesFromCat(cout, o.catalog);
      else cat->listBranches(cout, argv[0]);
    } else{
      ofstream out(o.output_file);
      if(o.catalog) cat->listBranchesFromCat(out, o.catalog);
      else cat->listBranches(out, argv[0]);
    }
    return 0;
  }

  if(o.list_selections){
    if(o.output_file == 0  || strcmp(o.output_file, "-")==0){
      cat->listSelections(cout);
      cout << std::flush;
    } else{
      ofstream out(o.output_file);
      cat->listSelections(out);
    }
    return 0;
  }


  if(o.event_list_from){
    cat->readEventList(o.event_list_from);
  }

  if(o.branches_from){
    cat->readBranchList(o.branches_from);
  }

  if((argc < 1 && o.catalog == 0) || (o.catalog != 0 && argc > 0)){
    cerr << "Wrong command usage. See pruner --help." << endl;
    return 1;
  }

  if(o.output_file == 0){
    cerr << "An output file must be specified using the option -o." << endl;
    return 1;
  }

  if(o.catalog){
    rc = cat->run(o.catalog, o.output_file, o.max_events, o.skip_events, o.max_files, o.skip_files);
  } else{
    rc = cat->run(argc, argv, o.output_file, o.max_events, o.skip_events);
  }

  return rc;
}


int parse_cmd_line(Options& o, int argc, char* argv[]){
  typedef enum {no_arg=0, required_arg, optional_arg} has_arg_t;
  enum { make_event_list = 300, make_branch_list, event_list_from,
	 selection, subselection, max_events, skip_events, max_files, skip_files
  };
  static struct option options[] = {
    {"verbose", no_arg, NULL, 'v'},
    {"output", required_arg, NULL, 'o'}, 
    {"help", no_arg, NULL, 'h'},
    {"max-events", required_arg, NULL, 'n'},
    {"skip-events", required_arg, NULL, skip_events},
    {"max-files", required_arg, NULL, max_files},
    {"skip-files", required_arg, NULL, skip_files}, 
    {"make-event-list", no_arg, NULL, make_event_list},
    {"make-branch-list", no_arg, NULL, make_branch_list},
    {"event-list-from", required_arg, NULL, event_list_from},
    {"branches-from", required_arg, NULL, 'b'},
    {"selection", required_arg, NULL, selection},
    {"subselection", required_arg, NULL, subselection},
    {"primary-dataset", required_arg, NULL, 'd'},
    {"list-selections", no_arg, NULL, 'l'},
    {"catalog", required_arg, NULL, 'c'},
    {0, 0, 0, 0}
  };
  
  const int noptions = sizeof(options)/sizeof(options[0])-1;
  char short_options[3*noptions+1];
   
  //for each long option, use as short option equivalent the option.val
  //character
  int pos = 0;
  for(int ioption=0; ioption<noptions; ++ioption){
    if(options[ioption].val < 0xFF){
      short_options[pos++] = options[ioption].val;
      switch(options[ioption].has_arg){
      case required_argument: //a required arg. is indicated with a colon
	short_options[pos++] = ':';
	break;
      case optional_argument://an optional arg is indicated with two colons
	short_options[pos++] = ':';
	short_options[pos++] = ':';
	break;
      }
    }
  }
  short_options[pos] = '\0';

  //   cout << "short option description: " << short_options << endl;

  int c = 0;
  while((c=getopt_long(argc, argv, short_options,
		       options, NULL))!=-1){
    switch (c){
    case 'h'://-h or --help
      o.help = 1;
      break;
    case 'v'://-v or --verbose
      ++o.verbose;
      break;
    case 'o':
      o.output_file = optarg;
      break;
    case 'n':
      o.max_events = strtol(optarg, 0, 0);
      break;
    case skip_events:
      o.skip_events = strtol(optarg, 0, 0);
      break;
    case max_files:
      o.max_files = strtol(optarg, 0, 0);
      break;
    case skip_files:
      o.skip_files = strtol(optarg, 0, 0);
      break;
    case 'd':
      o.primary_dataset = optarg;
      break;
    case 'c':
      o.catalog = optarg;
      break;
    case make_event_list:
      o.make_event_list = true;
      break;
    case make_branch_list:
      o.make_branch_list = true;
      break;
    case event_list_from:
      o.event_list_from = optarg;
      break;
    case 'b':
      o.branches_from = optarg;
      break;
    case selection:
      o.selection = optarg;
      break;
    case subselection:
      o.subselection = optarg;
      break;
    case 'l':
      o.list_selections = true;
      break;
    default:
      help();
      exit(EXIT_FAILURE);
    }
  }
  return optind;
}

