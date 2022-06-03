// Author: Evgeny Berendeev
// Budker Institute of Nuclear Physics of Siberian Branch Russian Academy of Sciences
// beren@inp.nsk.su
// (c) 2022, for licensing details see the LICENSE file

#include "simulation.h"

//Main function simply hands off control to the Simulation class
int main(int argc, char* argv[]) {
	Simulation simulation(argc, argv);

	simulation.make_all();

	return(0);
}