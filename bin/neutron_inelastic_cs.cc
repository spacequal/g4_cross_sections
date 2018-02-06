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

//Cross Section Access
#include "G4Material.hh"
#include "G4NeutronInelasticCrossSection.hh"
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
static char doc[]     = "ROOT (CERN) Plots of Cross Sections from GEANT4\n\n"
                        "Author: Branden Allen\n"
                        "Date  : 2018.01.05 (First Commit)\n\n"
                        "Example command: ./cs_plot --energy 0.1 100.0 0.01 -o output_file.png"
								"--clobber Cu W\n\n";
static char args_doc[]= "Maximum Input Elements (100)";

static struct argp_option options[]= {
	{"energy" , 'e', "ENERGY_BINS", 0, "Energy Bins [MeV]"    },
	{"ofile"  , 'o', "OUTPUT_FILE", 0, "Output Image File"    },
	{"clobber",   2, 0            , 0, "Overwrite Old File"   },
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
		case 'e':
			if(a->nelim> 0) argp_usage(state);
			ss_arg= new stringstream(arg);
			*ss_arg>> a->elim[a->nelim++];
			a->last_elim= true;
			delete ss_arg;
			break;
		case 'o':
			a->ofile= arg;
			break;
		case 2:
			a->clobber= 1;
			break;
		case ARGP_KEY_ARG:
			if(a->last_elim) {
				//Remainder of the ELIM Input
				ss_arg= new stringstream(arg);
				*ss_arg>> a->elim[a->nelim++];
				a->last_elim= (a->nelim> 2) ? false : true;
			} else {
				//Standard Argument Retrieval
				if(state->arg_num>= MAX_ARGS) argp_usage(state);
				a->args[a->nargs++]= arg;
			}
			break;
		case ARGP_KEY_END:
			if(state->arg_num<1) argp_usage(state);
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
	a.A            = 6                    ;   //Default C-14
	a.Z            = 14                   ;
	memset(a.elim, 0, 3*sizeof(double));

	//Parse input arguments
	static struct argp argp= {options, parse_opt, args_doc, doc};
	argp_parse(&argp, argc, argv, 0, 0, &a);

	//Neutron Cross Section
	G4NeutronInelasticCrossSection *n_ics= new G4NeutronInelasticCrossSection();

	//Data
	int n_pts= (int) (a.elim[1]-a.elim[0])/a.elim[2];
	double *E, *cs;
	int size= n_pts*sizeof(double);
	E = (double*) malloc(size); memset(E , 0, size);
	cs= (double*) malloc(size); memset(cs, 0, size);
	for(int i= 0; i< n_pts; i++) {
		E[i]= a.elim[0]+ a.elim[2]*i;
		cs[i]= n_ics->GetCrossSection(E[i]*MeV, a.Z, a.A)/millibarn;
	}

	//Report User Settings
	cout << "Energy Bin Definition [keV]: ";
	for(int i= 0; i< 3; i++) { cout << a.elim[i] << " "; }
	cout << endl;
	cout << "Output File: " << a.ofile << endl;
	cout << "Argument List: " << endl;
	if(a.clobber) { cout << "OVERWRITE ACTIVE" << endl; }
	for(int i= 0; i< a.nargs; i++) { cout << "\t" << a.args[i] << endl; }

	//Canvas
	TCanvas *c= new TCanvas("c1", "c1");
	TGraph *g= new TGraph(n_pts, E, cs);
	g->GetXaxis()->SetTitle("Energy [MeV]");
	g->GetYaxis()->SetTitle("Cross Section [mb]");
	g->SetTitle("Neutron Inelastic Scattering Cross Section");
	g->Draw();
//	c->SetLogy();
	c->SaveAs(a.ofile);

	//Exit
	free(E); free(cs);
	return 0;
}

