// Author: Evgeny Berendeev
// Budker Institute of Nuclear Physics of Siberian Branch Russian Academy of Sciences
// beren@inp.nsk.su
// (c) 2022, for licensing details see the LICENSE file

#include "particles.h"

std::ostream& operator<<(std::ostream& out, const ParticleSimple& particle){
	out << particle.coord[0] << " " << particle.coord[1] << " " << particle.coord[2] << " " << particle.pulse[0] << " " << particle.pulse[1] << " " << particle.pulse[2];
	return out;
} 

ParticlesArray::ParticlesArray(int nx, int ny, int nz, 
        const std::unordered_map<std::string, double> parameters, const std::vector<std::string>& vecStringPartParams):
    particlesData(nx,ny,nz), 
    countInCell(nx,ny,nz){
    _ppc = int_value(parameters.at("NumPartPerCell"));
    _dx = parameters.at("Dx");
    for (const auto& line: vecStringPartParams){
        set_params_from_string(line);
    }

    std::cout << "dfgvs\n";
}

void ParticlesArray::set_params_from_string(const std::string& line){
    std::vector<std::string> strvec;

    strvec = split(line, ' ');

    if(strvec[0]=="Particles") {
        this->_name = strvec[1];
    }

    if(strvec[0]=="Mass"){
        this->_mass = stod(strvec[1]);
    }

    if(strvec[0]=="Charge"){
       this->_charge =  stoi(strvec[1]);
    }
    if(strvec[0]=="Density"){
        this->_density = stod(strvec[1]); 
        this->_mpw = _density * _dx * _dx * _dx / double(_ppc);
    }
}
