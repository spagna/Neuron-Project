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
	}
	
	std::cout << neurons[1]->numberOfTargets() << std::endl;
	std::cout << neurons[1]->getExcitatoryConnections() << std::endl;
	std::cout << neurons[1]->getInhibitoryConnections() << std::endl;
	
	int simulation_time = t_start; //the global simulation starts at step t_start
	
	/*double external_input (0.0); //allow the user to choose the external value from the terminal
	std::cout << "Choose a value for the external input" << std::endl; 
	std::cin >> external_input;
	
	for (auto n: neurons){
		if (n->getExcitatoryNeuron()){
			n->setExternalInput(external_input);
		}
	}
	//neurons[0]->setExternalInput(external_input); //a non post-synaptic neuron has a certain value as external input
	//n2.setExternalInput(0.0); //if it's a post-synaptic neuron, the external potential is 0*/
	int j(0);


	do {
		for (size_t i(0); i<neurons.size(); ++i){
			assert(neurons[i] != nullptr);
			neurons[i]->update(N);
		}
		/*for (auto n : neurons){ //update all the neurons present in the network
			if(n!=nullptr){
				n->update(N);
				if (n->getSpikeState()){
					std::cout << n->getTimeSpike() << std::endl;
				}
			} 
		}*/
		simulation_time += N; //the simulation time advanced of a time step N
		//std::cout << simulation_time << std::endl;
		for (auto n : neurons){ //test that all neurons have updated and that all clocks are equal to the simulation clock
			assert (n->getNeuronClock() == simulation_time);
			
		}
		file << "Membrane potential at " << simulation_time*h << " milliseconds: " << neurons[0]->getV_membrane() << std::endl; //the membrane potential is stored in Datas.txt
	} while (simulation_time < t_stop); //unitl it reaches the end of the global simulation
	for (size_t i(0); i<neurons.size(); ++i){
		std::cout << "Neuron " << i+1 << " has " << neurons[i]->getNumberSpikes() << std::endl;
		j+= neurons[i]->getNumberSpikes();
	}
	std::cout << j << std::endl;
	//std::cout << "Number of spikes: " << neurons[0]->getNumberSpikes() << std::endl;
	/*std::cout << "Neuron clock: " << n1.getNeuronClock()*h << std::endl;
	std::cout << "Neuron clock 2: " << n2.getNeuronClock()*h << std::endl;
	std::cout << "Simulation time " << simulation_time*h << std::endl;*/
	
	
	return 0;
}

	
