#include "Simulation.hpp"
#include "Neuron.hpp"
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>


Simulation::Simulation()
{}
void Simulation::oneNeuronSimulation()
{
	Neuron n(true); //an excitatory neuron declared
	std::ofstream file; //open a file to write down the values of the membrane potential at each time step
	file.open("Datas.txt");
	assert(not file.fail()); //check if the file opens correctly
	double input(0.0); //the user decide the external input
	std::cout << "Chose a value for the external input" << std::endl;
	std::cin>>input;
	n.setExternalInput(input);
	int simulation_time = t_start; //the global simulation starts at step t_start
	do { //update all the neurons of the simulation
		n.update(1, 0.0, 5); //no noise is added at the update function for one neuron simulation
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
		neuron1.update(1, 0.0, 5); //no noise is added at the update function for one neuron simulation
		if (neuron2.getTimeBuffer((simulation_time)%(D+1)) != 0){ //read if there is an amplitude in the time buffer
			//if an amplitude is present, the time the neurons receives the spike is written in the terminal
			std::cout << "The spike is received at time: " << simulation_time*h << std::endl;
		}
		neuron2.update(1, 0.0, 5);
		if (neuron1.getSpikeState()){ //write in the terminal the time of the spike
			std::cout << "A spike occured at time: " << simulation_time*h << std::endl;
		}
		simulation_time += N; //the simulation time advanced of a time step N
	} while (simulation_time < t_stop); //unitl it reaches the end of the global simulation
	
}
	
void Simulation::networkSimulation(double g, double pois)
{
	std::ofstream file2; //open a file to store the time of all the spikes of all neurons
	file2.open("Spike_time.txt");
	assert (not file2.fail());  //check if the file opens correctly

	Neuron n_excitatory(true);
	Neuron n_inhibitory (false);
	 //initiliaze the array representing the network of 12500 neurons
	std::array <Neuron*, total_neurons> neurons;
	for (size_t i(0); i<neurons.size(); ++i){
		if (i<excitatory_neurons){//from 0 to 9999 the neurons are excitatory
			neurons[i] = new Neuron(n_excitatory);
		} else {
			neurons[i] = new Neuron (n_inhibitory);//from 9999 to 12499 the neurons are inhibitory
		}
		assert(neurons[i] != nullptr); //check that no neurons in the end is nullptr
	}
	//add the connections between each neuron
	for (auto const& n : neurons){
		n->addConnections(neurons);	
	}
	
	int simulation_time = t_start; //the global simulation starts at step t_start
	do {
		for (size_t i(0); i<neurons.size(); ++i){ //update all the neurons present in the network
			assert (neurons[i] != nullptr); //check always that the neurons are nullptr
			neurons[i]->update(N, neurons[i]->randomSpikes(pois), g);  //update with the noise given by random spikes
			if (neurons[i]->getSpikeState()){ //when a neuron spikes, write down the time and the index of the neuron
				file2 << neurons[i]->getTimeSpike()/h << '\t' << i << '\n';
			}
		}
		simulation_time += N; //the simulation time advanced of a time step N after all neuron clocks have already advanced
	} while (simulation_time < t_stop); //unitl it reaches the end of the global simulation*/
	for (size_t i(0); i<neurons.size(); ++i){ 
		std::cout << neurons[i]->getNumberSpikes() << std::endl; 
	}
	//open the python script
	/*std::string name ("python ../Graphs.py &");
	system (name.c_str());*/
	for (auto& n : neurons){ //at the end of the simulation the memory has to be desallocated
		n = nullptr;
		delete n;
	}
}

void Simulation::plotGraph_A()
{
	networkSimulation(3,2);
	/*std::string name ("python ../Graphs.py &");
	system (name.c_str());*/
}

void Simulation::plotGraph_B()
{
	networkSimulation(6,4);
	/*std::string name ("python ../Graphs.py &");
	system (name.c_str());*/
}

void Simulation::plotGraph_C()
{
	networkSimulation(5,2);
	/*std::string name ("python ../Graphs.py &");
	system (name.c_str());*/
}

void Simulation::plotGraph_D()
{
	networkSimulation(4.5, 0.9);
	/*std::string name ("python ../Graphs.py &");
	system (name.c_str());*/	
}
	

Simulation::~Simulation()
{}
