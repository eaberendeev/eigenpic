// Author: Evgeny Berendeev
// Budker Institute of Nuclear Physics of Siberian Branch Russian Academy of Sciences
// beren@inp.nsk.su
// (c) 2022, for licensing details see the LICENSE file

#pragma once

#ifndef SIMULATION_H
#define SIMULATION_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "util.h"
#include "constants.h"
#include "communicator.h"
#include "particles.h"
#include "array.h"
#include <memory>

//Main simulation class
class Simulation {
	private:
		const std::unordered_map<std::string, double> parameters; //key = value hash table holding config file information

		int numCPUs; //how many ranks (CPUs) along each axis
		Vector3i globalSize; //size of the global simulation (in cells)
		Vector3i ghostCells; //how many ghost cells in each direction
		Communicator comm; //our MPI communicator Object

		int cellsX, cellsY, cellsZ; //size of local simulation, xyz
		double maxTime; //number of timesteps to perform

		Vector3i innerSize; //local simulation size - ghost_cells
		Vector3i totalSize; //local simulation size + ghost_cells

		long timestep; //current timestep
		long maxTimestep;
		double time; //current time

		double dx; //cell size
		double cellVol; //cell volume = dx^3
		double dt; //duration of single timestep

		Operator identity;

		//Sources and fields on the grid
		Field chargeDensity;
		Field currentDensity;

		Field E;
		Field B;

		std::vector<ParticlesArray> species;
		
		inline int vindg(int x, int y, int z, int c) const {
			return(ind(x + ghostCells[0], y + ghostCells[1], z + ghostCells[2], c, totalSize[0], totalSize[1], totalSize[2], 3));
		}
		inline int vind(int x, int y, int z, int c) const {
			return(ind(x, y, z, c, totalSize[0], totalSize[1], totalSize[2], 3));
		}
	public:
		Simulation(int argc, char* argv[]);
		void make_all();
		void step();
		void stencil_identity();
		void init_particles(const std::string& fParamsName);
		void init_parameters();
		void push(Vector3d& coord, Vector3d& puls, long q, double mass, double mpw, \
		   const Field& fieldE, const Field& fieldB, Field& currentDensity, double _dx, double _dt);
	private:
		std::unordered_map<std::string, double> load_parameters(const std::string& fParamsName) const;
};

#endif
