#include "Simulation.hpp"
#include "Neuron.hpp"
#include <iostream>


Simulation::Simulation()
{}
void Simulation::oneNeuronSimulation()
{
	Neuron n(true);
	std::ofstream file;
	file.open("Datas.txt");
	assert(not file.fail());
	double input(0.0);
	std::cout << "Chose a value for the external input" << std::endl;
	std::cin>>input;
	n.setExternalInput(input);
	int simulation_time = t_start; //the global simulation starts at step t_start
	do {
		n.update(1, 0.0);
		if (n.getSpikeState()){
			std::cout << "A spike occured at time: " << n.getNeuronClock()*h << std::endl;
		}
		simulation_time += N; //the simulation time advanced of a time step N
		file << "Membrane potential at " << simulation_time*h << " milliseconds: " << n.getV_membrane() << std::endl; //the membrane potential is stored in Datas.txt
	} while (simulation_time < t_stop); //unitl it reaches the end of the global simulation*/
}
	
void Simulation::networkSimulation()
{
	std::ofstream file1, file2; //open a file to store the values of the membrane potentials
	file1.open("Datas.txt");
	assert(not file1.fail()); //check if the fail opens correctly
	file2.open("Spike_time.txt");
	assert (not file2.fail());

	Neuron n_excitatory(true);
	Neuron n_inhibitory (false);
	
	std::array <Neuron*, total_neurons> neurons;
	for (size_t i(0); i<neurons.size(); ++i){
		if (i<excitatory_neurons){
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
		
		for (size_t i(0); i<neurons.size(); ++i){ //update all the neurons present in the network
			assert (neurons[i] != nullptr);
			neurons[i]->update(N, neurons[i]->randomSpikes());
			if (neurons[i]->getSpikeState()){
				file2 << neurons[i]->getTimeSpike()/h << '\t' << i << '\n';
			}
		}
		
		simulation_time += N; //the simulation time advanced of a time step N
		file1 << "Membrane potential at " << simulation_time*h << " milliseconds: " << neurons[0]->getV_membrane() << std::endl; //the membrane potential is stored in Datas.txt
	} while (simulation_time < t_stop); //unitl it reaches the end of the global simulation*/
	for (size_t i(0); i<neurons.size(); ++i){ 
		std::cout << neurons[i]->getNumberSpikes() << std::endl; 
	}
}

