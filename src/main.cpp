#include <fstream>
#include <iostream>
#include "Alignment.h"
#include "DualStream.h"
#include "MbRandom.h"
#include "Mcmc.h"
#include "Model.h"
#include "Settings.h"

void printHeader(DualStream& log);



int main (int argc, char* argv[]) {
    
	// initialize
	Settings mySettings(argc, argv);
	MbRandom myRandom;

    // set up an object for splitting the standard output stream to a file
	std::string logFileName = mySettings.getLogFileName();
	std::ofstream logFstrm;
    logFstrm.open( logFileName.c_str(), std::ios::out );
    DualStream log = DualStream(std::cout, logFstrm);

    // print header
    printHeader(log);
	
	// read the data
	Alignment myAlignment( mySettings.getDataFilePathName(), &log );
	
	// set up the model
	Model myModel( &mySettings, &myRandom, &myAlignment, &log );
	
	// run the Markov chain Monte Carlo analysis
	Mcmc myMcmc( &mySettings, &myModel, &myRandom, log );

    logFstrm.close();

    return 0;
}

void printHeader(DualStream& log) {

    log << "AutoParts v 1.0\n";
    log << "John P. Huelsenbeck and Brian Moore\n";
    log << "University of California, Bervis\n";
}
