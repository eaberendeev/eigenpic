// Author: Evgeny Berendeev
// Budker Institute of Nuclear Physics of Siberian Branch Russian Academy of Sciences
// beren@inp.nsk.su
// (c) 2022, for licensing details see the LICENSE file

#include <algorithm>

#include "simulation.h"
#include <functional>
using namespace std;

//main loop just steps through each timestep and outputs Newton diagnostics at the end
void Simulation::make_all() {
	for( long t = 0; t < maxTimestep; t++) {
		step();
	}
}

//one complete simulation step
void Simulation::step() {
	
}

//constructor inits basics stuff
Simulation::Simulation(int argc, char* argv[]) :
	parameters(load_parameters("./SysParams.cfg")),

	numCPUs(int_value(parameters.at("NumCPUs"))),
	globalSize(
		int_value(parameters.at("CellsX")),
		int_value(parameters.at("CellsY")),
		int_value(parameters.at("CellsZ"))
	),
	ghostCells( //enough for TSC particles
		globalSize[0] > 1 ? 2 : 0,
		globalSize[1] > 1 ? 2 : 0,
		globalSize[2] > 1 ? 2 : 0
	),
	comm(argc, argv, globalSize, ghostCells, numCPUs), //MPI

	cellsX(comm.get_local_size()[0]),
	cellsY(comm.get_local_size()[1]),
	cellsZ(comm.get_local_size()[2]),
	maxTime(parameters.at("MaxTime")),

	innerSize(cellsX, cellsY, cellsZ),
	totalSize(innerSize+2*ghostCells),

	timestep(0),
	time(0.),
	dx(0.),
	cellVol(0.),
	dt(0.),

	identity          (totalSize.prod()*3, totalSize.prod()*3),

	chargeDensity (Field::Zero(totalSize.prod()  )),
	currentDensity(Field::Zero(totalSize.prod()*3)),

	E    (Field::Zero(totalSize.prod()*3)),
	B    (Field::Zero(totalSize.prod()*3))
{

	init_parameters();
	init_particles("./PartParams.cfg");
	stencil_identity();
	std::cout << species[0].name() << " " << species[0].mpw() << " \n";
	std:: cout << cellsX << " " << cellsY << " " << cellsZ << "\n";
	std::cout << species[0].countInCell(2,2,2) << " "<< species[0].countInCell.size().transpose() << "\n";
	Particle pp; 
	pp.coord = Vector3d(0.06,0.06,0.06); //Vector3d(0.3,0.6,0.6)));
	species[0].add_particle_scatter(pp);
		species[0].particlesData(4,5,6).emplace_back(Particle(Vector3d(0.07,0.06,0.06),Vector3d(1.,3,5)));
		species[0].particlesData(1,1,1).emplace_back(Particle(Vector3d(0.07,0.06,0.06),Vector3d(1.,3,5)));
	std::cout<< species[0].particlesData(1,1,1)(0) << "\n";
	std::cout<< species[0].particlesData(1,1,1)(1) << "\n";
	std::cout<< species[0].particlesData(4,5,6)(0) << "\n";


}

void Simulation::init_particles(const std::string& fParamsName){
	std::vector< std::vector<std::string> > stringParticlesParams;
///// Make particles
	read_params_to_string("Particles",fParamsName,stringParticlesParams);
	for( const auto &vecString  : stringParticlesParams){
		std::cout << vecString[1] <<"\n"; 
	    species.emplace_back(cellsX,cellsY,cellsZ,parameters,vecString);
	}
}

//read in config file, stow key-value pairs in hash table
std::unordered_map<std::string, double> Simulation::load_parameters(const std::string& fParamsName) const {
	std::unordered_map<std::string, double> parameters;
    std::vector<string> strvec;	
	ifstream in(fParamsName);
    string line;

	string bufKey, bufVal;

	while(in.good()) {
		getline(in, line);

		if(in.eof()) break;
		// only parameters may be set
		if(line.size() == 0) continue;

        strvec = split(line, ' ');

    	std::vector<string>::iterator it = remove_if(strvec.begin(), strvec.end(),mem_fun_ref(&std::string::empty));
    	strvec.erase(it,strvec.end());	

		if(strvec.size() < 2 ) continue;
		if(strvec[0][0] == '#' ) continue;
		// get name of papameter
		bufKey = strvec[0];
		// get value of papameter
		bufVal = strvec[1];
		// construct 
		parameters.insert({bufKey, stod(bufVal)});
	}
	return(parameters);
}
