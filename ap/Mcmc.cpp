#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "Mcmc.h"
#include "Model.h"
#include "Restaurant.h"
#include "Settings.h"



Mcmc::Mcmc(Settings* sp, Model* mp, MbRandom* rp) {

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
			std::cout << std::setw(6) << n << " -- " << std::fixed << std::setprecision(3) << newLnL << " ";
			for (int i=0; i<modelPtr->getNumRestaurants(); i++)
				std::cout << std::setw(2) << modelPtr->getRestaurant(i)->getNumTables() << " ";
			std::cout << rest->getName();
			std::cout << std::endl;
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
		std::cout << i << " -- " << std::fixed << std::setprecision(4) << mean[i] / settingsPtr->getChainLength() << std::endl;
	
	/* print information on acceptances */
	int longestName = 0;
	for (int i=0; i<modelPtr->getNumRestaurants(); i++)
		{
		std::string restaurantName = modelPtr->getRestaurant(i)->getName();
		if (restaurantName.size() > longestName)
			longestName = restaurantName.size();
		}
	std::cout << std::endl;
	std::cout << "Parm";
	for (int j=0; j<longestName - 4; j++)
		std::cout << " ";
	std::cout << "  ";
	std::cout << std::setw(5) << "Tries" << " ";
	std::cout << std::setw(5) << "Accep" << " ";
	std::cout << std::setw(5) << "Rate" << std::endl;
	for (int i=0; i<modelPtr->getNumRestaurants(); i++)
		{
		std::string restaurantName = modelPtr->getRestaurant(i)->getName();
		std::cout << restaurantName;
		for (int j=0; j<longestName - restaurantName.size(); j++)
			std::cout << " ";
		std::cout << "  ";
		std::cout << std::setw(5) << numAttempted[i] << " ";
		std::cout << std::setw(5) << numAccepted[i] << " ";
		if (numAttempted[i] > 0)
			std::cout << std::fixed << std::setprecision(2) << std::setw(5) << (double)numAccepted[i] / numAttempted[i];
		else
			std::cout << std::setw(5) << "N/A";
		std::cout << std::endl;
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