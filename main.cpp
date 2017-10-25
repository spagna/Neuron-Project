#include "Neuron.hpp"
#include <iostream>
#include <cmath>
#include <fstream>

int main()
{
	
	std::ofstream file; //open a file to store the values of the membrane potentials
	file.open("Datas.txt");
	assert(not file.fail()); //check if the fail opens correctly
	
	Neuron n1, n2; //n2 is the post-synaptic neuron	
	std::vector <Neuron*> neurons; //vector of all the neurons in the network
	
	neurons.push_back (&n1);
	neurons.push_back (&n2);
	n1.addTargetNeuron(&n2);
	int simulation_time = t_start; //the global simulation starts at step t_start
	
	double external_input (0.0); //allow the user to choose the external value from the terminal
	std::cout << "Choose a value for the external input" << std::endl; 
	std::cin >> external_input;
	//assert (external_input >= 0.0); //the external input should be equal or biggert than 0
	n1.setExternalInput(external_input); //a non post-synaptic neuron has a certain value as external input
	n2.setExternalInput(0.0); //if it's a post-synaptic neuron, the external potential is 0

	do {
		for (auto n : neurons){ //update all the neurons present in the network
			if(n!=nullptr){
				n->update(N);
			} 
		}
		simulation_time += N; //the simulation time advanced of a time step N
		for (auto n : neurons){ //test that all neurons have updated and that all clocks are equal to the simulation clock
			assert (n->getNeuronClock() == simulation_time);
		}
		file << "Membrane potential at " << simulation_time*h << " milliseconds: " << n1.getV_membrane() << std::endl; //the membrane potential is stored in Datas.txt
	} while (simulation_time < t_stop); //unitl it reaches the end of the global simulation
	std::cout << "Number of spikes: " << n1.getNumberSpikes() << std::endl;
	std::cout << "Neuron clock: " << n1.getNeuronClock()*h << std::endl;
	std::cout << "Neuron clock 2: " << n2.getNeuronClock()*h << std::endl;
	std::cout << "Simulation time " << simulation_time*h << std::endl;
	
	return 0;
}

	
