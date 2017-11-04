#include "Simulation.hpp"
#include "Neuron.hpp"
#include <iostream>


Simulation::Simulation()
{}
void Simulation::oneNeuronSimulation()
{
	Neuron n(true); //an excitatory neuron declared
	std::ofstream file; //open a file to write down the values of the membrane potential at each time step
	file.open("Datas.txt");
	assert(not file.fail()); //control if the file opens correctly
	double input(0.0); //the user decide the external input
	std::cout << "Chose a value for the external input" << std::endl;
	std::cin>>input;
	n.setExternalInput(input);
	int simulation_time = t_start; //the global simulation starts at step t_start
	do { //update all the neurons of the simulation
		n.update(1, 0.0); //no noise is added at the update function for one neuron simulation
		if (n.getSpikeState()){ //write in the terminal the time of the spike
			std::cout << "A spike occured at time: " << simulation_time*h << std::endl;
		}
		simulation_time += N; //the simulation time advanced of a time step N
		file << "Membrane potential at " << simulation_time*h << " milliseconds: " << n.getV_membrane() << std::endl; //the membrane potential is stored in Datas.txt
	} while (simulation_time < t_stop); //unitl it reaches the end of the global simulation
}

void Simulation::twoNeruonsSimulation()
{
	Neuron neuron1(true), neuron2(true); //neuron 1 is the spiking neuron, neuron2 is a post-synaptic neuron
	neuron1.addTargetNeuron(&neuron2);
	
	double input(0.0); //the user decide the external input
	std::cout << "Chose a value for the external input" << std::endl;
	std::cin>>input;
	neuron1.setExternalInput(input); //the external input is added only to the spiking neuron
	
	int simulation_time = t_start; //the global simulation starts at step t_start
	do { //update all the neurons of the simulation
		neuron1.update(1, 0.0); //no noise is added at the update function for one neuron simulation
		if (neuron2.getTimeBuffer((simulation_time)%(D+1)) != 0){ //read if there is an amplitude in the time buffer
			//if an amplitude is present, the time the neurons receives the spike is written in the terminal
			std::cout << "The spike is received at time: " << simulation_time*h << std::endl;
		}
		neuron2.update(1, 0.0);
		if (neuron1.getSpikeState()){ //write in the terminal the time of the spike
			std::cout << "A spike occured at time: " << simulation_time*h << std::endl;
		}
		simulation_time += N; //the simulation time advanced of a time step N
	} while (simulation_time < t_stop); //unitl it reaches the end of the global simulation
	
}
	
void Simulation::networkSimulation()
{
	std::ofstream file2; //open a file to store the values of the membrane potentials
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
	} while (simulation_time < t_stop); //unitl it reaches the end of the global simulation*/
	for (size_t i(0); i<neurons.size(); ++i){ 
		std::cout << neurons[i]->getNumberSpikes() << std::endl; 
	}
}

