// Author: Evgeny Berendeev
// Budker Institute of Nuclear Physics of Siberian Branch Russian Academy of Sciences
// beren@inp.nsk.su
// (c) 2022, for licensing details see the LICENSE file

#include "util.h"

// Utils for read parameters
// function separating the string variable by the delimiter delim
void split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}
std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

/* Read file 'filename' and separate file by the target.
Convert each part to a vector of strings and store as an element of params

'filename' format:
target type_target
var_name value
...
target type_target
...

*/
void read_params_to_string(const std::string& target,
    const std::string& filename,std::vector< std::vector<std::string> > &params)
{
    std::ifstream fParams;
    std::string line;
    std::vector<std::string> strvec;
    long currentParam = -1;
	
	params.clear();

    fParams.open(filename, std::ifstream::in);
    getline(fParams, line);

    while (fParams.good() ){
        strvec = split(line, ' ');
        if(strvec[0] == target) {
            ++currentParam;
            params.resize(currentParam+1);
        }
        if (currentParam >= 0)           
        	params[currentParam].push_back(line);

        getline(fParams,line);
    }

    fParams.close();
}

void split_by_threads(int n, int ithr, int nthr, int& split, int& origin)
{
    split = n / nthr;
    int remainder = n % nthr;

    origin = split*ithr + std::min(ithr,remainder);
    
    if (0 <= ithr && ithr < remainder){
        split +=1;
    }
}