#include "Neuron.hpp"
#include <iostream>
#include <cmath>
#include <fstream>

int main()
{
	
	std::ofstream file; //open a file to store the values of the membrane potentials
	file.open("Datas.txt");
	assert(not file.fail()); //check if the fail opens correctly
	

	Neuron n_excitatory;
	n_excitatory.setExcitatoryNeuron(true);
	Neuron n_inhibitory;
	n_inhibitory.setExcitatoryNeuron(false);
	
	std::array <Neuron*, 12500> neurons;
	for (size_t i(0); i<neurons.size(); ++i){
		if (i<10000){
			neurons[i] = new Neuron(n_excitatory);
		} else {
			neurons[i] = new Neuron (n_inhibitory);
		}
	}
	for (auto n: neurons){
		assert(n!=nullptr);
	}
	
	for (auto n : neurons){
		n->addConnections(neurons);	
		assert (n->getExcitatoryConnections() == 1000);
		assert (n->getInhibitoryConnections() == 250);
	}
	
	int simulation_time = t_start; //the global simulation starts at step t_start

	do {
		
		for (auto n : neurons){ //update all the neurons present in the network
			assert (n != nullptr);
			n->update(N);
		}
		simulation_time += N; //the simulation time advanced of a time step N
		file << "Membrane potential at " << simulation_time*h << " milliseconds: " << neurons[0]->getV_membrane() << std::endl; //the membrane potential is stored in Datas.txt
	} while (simulation_time < t_stop); //unitl it reaches the end of the global simulation
	/*for (size_t i(0); i<neurons.size(); ++i){
		std::cout << "Neuron " << i+1 << " had " << neurons[i]->getNumberSpikes() << " spikes " << std::endl;
	}*/
	return 0;
}

	
