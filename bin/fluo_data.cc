/*-------------------------------------------------------------------
Extract data from GEANT4 and plot the cross section to an output file
using ROOT (CERN).

Author: Branden Allen
Date  : 2018.01.05
-------------------------------------------------------------------*/
//Standard 
#include <iostream>
#include <string>
#include <cstring>

//Argument Parsing
#include <argp.h>

//System Access
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

//GEANT4
#include "G4Material.hh"
#include "G4FluoData.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//ROOT for plotting 
#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"

//Local Cross Section Library Definitions
#include <cs_defs.hh>

using namespace std;

//Input Option Documentation
static char doc[]     = "GEANT4 Fluorescence Data Check\n\n"
                        "Author: Branden Allen\n"
                        "Date  : 2018.01.06 (First Commit)\n\n"
                        "Example command: ./fluo_data -Z 20"
								"\n\n";
static char args_doc[]= "Maximum Input Elements (100)";

static struct argp_option options[]= {
	{"energy"        , 'e', "ENERGY_BINS", 0, "Energy Bins [MeV]"    },
	{"ofile"         , 'o', "OUTPUT_FILE", 0, "Output Image File"    },
	{"Atomic Number" , 'Z', "ATOMIC_NUM" , 0, "Atomic Number"        },
	{"clobber"       ,   2, 0            , 0, "Overwrite Old File"   },
	{0}
};

struct args {
	char *args[MAX_ARGS];
	int nargs;

	double elim[3];            //Input energy limits
	int nelim;                 //Number of read elements
	bool last_elim;            //Flag giving the last elim for error detection

	G4int Z, A;                //Target Z,A

	char *ofile;
	unsigned int clobber;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
	struct args *a= (args*) state->input;
	stringstream *ss_arg;

	switch(key) {
		case 'Z':
			a->Z= atof(arg);
			break;
		case ARGP_KEY_ARG:
			//Standard Argument Retrieval
			if(state->arg_num>= MAX_ARGS) argp_usage(state);
			a->args[a->nargs++]= arg;
			break;
		case ARGP_KEY_END:
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
};

int main(int argc, char **argv) {
	//Default Argument Values
	struct args a;
	for(int i= 0; i< MAX_ARGS; i++) { a.args[i]= 0x0; }
	a.nargs        = 0                    ;
	a.nelim        = 0                    ;
	a.clobber      = 0                    ;
	a.last_elim    = false                ;
	a.ofile        = NULL                 ;
	a.A            = 14                   ;   //Default C-14
	a.Z            = 6                    ;
	memset(a.elim, 0, 3*sizeof(double));

	//Parse input arguments
	static struct argp argp= {options, parse_opt, args_doc, doc};
	argp_parse(&argp, argc, argv, 0, 0, &a);

	//Transition Data
	cout << "Z: " << a.Z << endl; 
	G4FluoData *fl_data= new G4FluoData();
	fl_data->LoadData(a.Z);
	fl_data->PrintData();

	return 0;
}

