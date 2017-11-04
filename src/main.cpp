#include "Neuron.hpp"
#include "Simulation.hpp"
#include <iostream>
#include <cmath>
#include <fstream>

int main()
{
	Simulation sim;
	sim.networkSimulation();
	//sim.oneNeuronSimulation();
	//sim.twoNeruonsSimulation();
	std::cout << "Simulation done" << std::endl;
	return 0;
}

	
