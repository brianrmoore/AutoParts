#include <fstream>
#include <iostream>
#include "Alignment.h"
#include "DualStream.h"
#include "MbRandom.h"
#include "Mcmc.h"
#include "Model.h"
#include "Settings.h"

void printHeader(void);



int main (int argc, char* argv[]) {
    
    // print header
    printHeader();

	// initialize
	Settings mySettings(argc, argv);
	MbRandom myRandom;

    // set up an object for splitting the standard output stream to a file
	std::string logFileName = mySettings.getLogFileName();
	std::ofstream logFstrm;
    logFstrm.open( logFileName.c_str(), std::ios::out );
    DualStream log = DualStream(std::cout, logFstrm);
	
	// read the data
	Alignment myAlignment( mySettings.getDataFilePathName() );
	
	// set up the model
	Model myModel( &mySettings, &myRandom, &myAlignment );
	
	// run the Markov chain Monte Carlo analysis
	Mcmc myMcmc( &mySettings, &myModel, &myRandom );

    logFstrm.close();

    return 0;
}

void printHeader(void) {

    std::cout << "AutoParts v 1.0" << std::endl;
    std::cout << "John P. Huelsenbeck and Brian Moore" << std::endl;
    std::cout << "University of California, Bervis" << std::endl;
}
