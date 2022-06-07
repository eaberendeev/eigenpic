CXX = g++
#User configurable paths
EIGEN_INCLUDE_PATH=-isystem ../../../eigen/
source_dirs := . srcParticles

#User configurable compile flags
override compile_flags += -Wall -pipe -std=c++11 -O3 

search_wildcards       := $(addsuffix /*.cpp,$(source_dirs)) 

epic: $(notdir $(patsubst %.cpp,%.o,$(wildcard $(search_wildcards))))
	$(CXX) $^ $(compile_flags) -o $@

VPATH := $(source_dirs)
     
%.o: %.cpp
	$(CXX) -c -MD $(compile_flags) $(EIGEN_INCLUDE_PATH) $(addprefix -I,$(source_dirs))  $^

clean:
	rm -f epic *.o *.d
