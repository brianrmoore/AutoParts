#ifndef Mcmc_H
#define Mcmc_H

#include <fstream>

class MbRandom;
class Model;
class Settings;

class Mcmc {

	public:
                	              Mcmc(Settings* sp, Model* mp, MbRandom* rp);  
								 ~Mcmc(void);
                	              
	private:
	                       void   saveStates(int n, double lnL);
						 Model*   modelPtr;
					  Settings*   settingsPtr;
					  MbRandom*   ranPtr;
				  std::ofstream   lnOut;
                            
};

#endif