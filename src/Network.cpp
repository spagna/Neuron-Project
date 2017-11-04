#include "Network.hpp"
#include <iostream>


Network::Network ()
{
	Neuron n_excitatory;
	n_excitatory.setExcitatoryNeuron(true);
	Neuron n_inhibitory;
	n_inhibitory.setExcitatoryNeuron(false);

	for (size_t i(0); i<neurons.size(); ++i){
		if (i<excitatory_neurons){
			
			neurons[i] = new Neuron(n_excitatory);
		} else {
			neurons[i] = new Neuron (n_inhibitory);
		}
	}
}

std::array<Neuron*, total_neurons> Network::getNeuron () const
{
	return neurons;
}

	
Neuron* Network::getNeurons(unsigned int i) const
{
	assert(i<neurons.size());
	return neurons[i];
}
	
void Network::setNeurons(unsigned int i, Neuron* n)
{
	neurons[i] = n;
}

Network::~Network()
{}

