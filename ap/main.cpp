#include <iostream>
#include "Alignment.h"
#include "MbRandom.h"
#include "Mcmc.h"
#include "Model.h"
#include "Settings.h"



int main (int argc, char* argv[]) {

	/* initialize */
	Settings mySettings(argc, argv);
	MbRandom myRandom;
	
	// read the data
	Alignment myAlignment( mySettings.getDataFilePathName() );
	
	// set up the model
	Model myModel( &mySettings, &myRandom, &myAlignment );
	
	// run the Markov chain Monte Carlo analysis
	Mcmc myMcmc( &mySettings, &myModel, &myRandom );

    return 0;
}
