#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "DualStream.h"
#include "Mcmc.h"
#include "Model.h"
#include "Restaurant.h"
#include "Settings.h"



Mcmc::Mcmc(Settings* sp, Model* mp, MbRandom* rp, DualStream& log) {

	// remember some pointers that we will need
	settingsPtr = sp;
	modelPtr    = mp;
	ranPtr      = rp;
	
	/* Open a file for printing the log likelihood. The parameters are saved through
	   the Restaurant objects. */
	std::string lnFileName = settingsPtr->getOutPutFileName() + ".ln";
	lnOut.open( lnFileName.c_str(), std::ios::out );
	
	/* allocate information for the acceptance rates */
	int *numAccepted = new int[2 * modelPtr->getNumRestaurants()];
	int *numAttempted = numAccepted + modelPtr->getNumRestaurants();
	for (int i=0; i<modelPtr->getNumRestaurants(); i++)
		numAccepted[i] = numAttempted[i] = 0;
        
    // initialize the log likelihoods
    modelPtr->lnLikelihood(true);
	
	/* run the Markov chain */
	std::vector<double> mean(modelPtr->getNumRestaurants(), 0.0);
	for (int n=1; n<=settingsPtr->getChainLength(); n++)
		{
		/* pick a parameter to change */
		Restaurant* rest = modelPtr->getRestaurantToChange();
		int restaurantId = modelPtr->getRestaurantId(rest);
		
		/* update the parameter */
		bool wasAccepted = rest->change();
        
        double newLnL = modelPtr->lnLikelihood();
		
		/* print information to the screen */
		if ( n % settingsPtr->getPrintFrequency() == 0 )
			{
			log << std::setw(6) << n << " -- " << std::fixed << std::setprecision(3) << newLnL << " ";
			for (int i=0; i<modelPtr->getNumRestaurants(); i++)
				log << std::setw(2) << modelPtr->getRestaurant(i)->getNumTables() << " ";
			log << rest->getName();
			log << '\n';
			}
		for (int i=0; i<modelPtr->getNumRestaurants(); i++)
			mean[i] += modelPtr->getRestaurant(i)->getNumTables();
			
		/* remember the state of the chain */
		if ( n % settingsPtr->getSampleFrequency() == 0)
			saveStates( n, 0.0 );
			
		/* update information on acceptances */
		numAttempted[restaurantId]++;
		if (wasAccepted == true)
			numAccepted[restaurantId]++;
		
		}
	
	for (int i=0; i<modelPtr->getNumRestaurants(); i++)
		log << i << " -- " << std::fixed << std::setprecision(4) << mean[i] / settingsPtr->getChainLength() << '\n';
	
	/* print information on acceptances */
	int longestName = 0;
	for (int i=0; i<modelPtr->getNumRestaurants(); i++)
		{
		std::string restaurantName = modelPtr->getRestaurant(i)->getName();
		if (restaurantName.size() > longestName)
			longestName = (int)restaurantName.size();
		}
	log << '\n';
	log << "Parm";
	for (int j=0; j<longestName - 4; j++)
		log << " ";
	log << "  ";
	log << std::setw(5) << "Tries" << " ";
	log << std::setw(5) << "Accep" << " ";
	log << std::setw(5) << "Rate" << '\n';
	for (int i=0; i<modelPtr->getNumRestaurants(); i++)
		{
		std::string restaurantName = modelPtr->getRestaurant(i)->getName();
		log << restaurantName;
		for (int j=0; j<longestName - restaurantName.size(); j++)
			log << " ";
		log << "  ";
		log << std::setw(5) << numAttempted[i] << " ";
		log << std::setw(5) << numAccepted[i] << " ";
		if (numAttempted[i] > 0)
			log << std::fixed << std::setprecision(2) << std::setw(5) << (double)numAccepted[i] / numAttempted[i];
		else
			log << std::setw(5) << "N/A";
		log << '\n';
		}

	delete [] numAccepted;
}

Mcmc::~Mcmc(void) {

	lnOut.close();
}

void Mcmc::saveStates(int n, double lnL) {

	if (n == 0)
		lnOut << "Gen" << '\t' << "lnL" << std::endl;
	lnOut << n << '\t' << lnL << std::endl;
	
	for (int i=0; i<modelPtr->getNumRestaurants(); i++)
		{
		modelPtr->getRestaurant(i)->saveState(n);
		}
		
}