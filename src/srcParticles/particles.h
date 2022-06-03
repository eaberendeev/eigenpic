// Author: Evgeny Berendeev
// Budker Institute of Nuclear Physics of Siberian Branch Russian Academy of Sciences
// beren@inp.nsk.su
// (c) 2022, for licensing details see the LICENSE file

#ifndef PARTICLES_H_
#define PARTICLES_H_
#include "array.h"
#include <functional>
#include <assert.h>
#include "util.h"
#include <map>
#include <unordered_map>

struct ParticleSimple{
	Vector3d coord;
    Vector3d pulse;
	friend std::ostream& operator<<(std::ostream& out, const ParticleSimple &particle);
        ParticleSimple(){
    }
    ParticleSimple(const Vector3d& r,const Vector3d& u):
        coord(r),pulse(u) {
    }
};

typedef ParticleSimple Particle;

class ParticlesArray{

public:
    Array3D<Array<Particle> > particlesData;
    Array3D<int> countInCell;

    void add_particle_scatter(const Particle& particle){
        auto i = int(particle.coord[0] / _dx);
        auto j = int(particle.coord[1] / _dx);
        auto k = int(particle.coord[2] / _dx);
        //if(counter % _world.MPIconf.size_depth() == _world.MPIconf.rank_depth() ){
        particlesData(i,j,k).push_back(particle);
        
        //}
        //++counter;
    }
    void delete_particle_runtime(int i, int j, int k, int n){
        countInCell(i,j,k)--;
        int old_count = countInCell(i,j,k);
        particlesData(i,j,k)(n) = particlesData(i,j,k)(old_count);
        if(old_count == static_cast<int>(particlesData(i,j,k).size()) - 1 ){
             particlesData(i,j,k).pop_back();
        }
        else{
            particlesData(i,j,k).del(old_count);
        }
    }
    void update_count_in_cell(){
        for (auto i = 0;i< countInCell.size()[0];i++){
        for (auto j = 0; j< countInCell.size()[1];j++){
        for (auto k = 0; k< countInCell.size()[2];k++){
            countInCell(i,j,k) = particlesData(i,j,k).size();
        }
        }
        }   
    }
    int size(){
        int s = 0;
        for (auto i = 0;i< countInCell.size()[0];i++){
        for (auto j = 0; j< countInCell.size()[1];j++){
        for (auto k = 0; k< countInCell.size()[2];k++){
            s+= particlesData(i,j,k).size();
        }
        }
        }
        return s;
    }
    ParticlesArray(int Nx, int Ny, int Nz, const std::unordered_map<std::string, double> parameters, const std::vector<std::string>& vecStringPartParams);

    double mass() const{
          return _mass;
    }
    double mpw() const{
          return _mpw;
    }
    std::string name() const{
          return _name;
    }

protected:

    int _charge;
    double _density;
    std::string _name;
    int _ppc; // Particles per cell;
    double _dx;
    double _mass;
    double _mpw; /*macroparticle weight*/

    void set_params_from_string(const std::string& line);
};

#endif 
