#include "Neuron.hpp"
#include <iostream>
#include <cmath>
#include <fstream>


constexpr unsigned int n (5.0); //number of steps of the simulation
//constexpr double h (0.1); //equidistant step of time during which the simulation evolves


int main()
{
	Neuron n;
	std::ofstream file;
	
	file.open("Datas.txt");
	if (file.fail()){
		std::cerr << "Error: file doesn't open" << std::endl;
	} else {
		//for (unsigned int i(0); i<n; ++i){
		n.update(h);
		file << "Membrane potential: " << n.getV_membrane() << std::endl;
	}
		
		
		
	
	
	return 0;
}
