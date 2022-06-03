// Author: Evgeny Berendeev
// Budker Institute of Nuclear Physics of Siberian Branch Russian Academy of Sciences
// beren@inp.nsk.su
// (c) 2022, for licensing details see the LICENSE file

#include <iostream>
#include <random>
#include <math.h>
#include "simulation.h"

using namespace std;

//init general physical values for the simulation
void Simulation::init_parameters() {
	double dt  = parameters.at("Dt");
	long maxTimestep = parameters.at("MaxTime") / dt + 1;

	std::stringstream ss;
	for( const auto& par : parameters ) {
        ss << par.first << " " << par.second << "\n";
    }
	std::cout << ss.str();
	std::ofstream out_parameters("simulation_parameters");
	out_parameters << ss.str();

}
