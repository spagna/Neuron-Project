#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include "Neuron.hpp"

constexpr int total_neurons (12500); //total number of neurons in the network


class Network
{
	/*!
     * @brief The constructor of the class Network.
     */
	Network();
	
	/*!
     * @brief Get the neuron of a specific case of the neurons vector
     *
     * @param i : a positive integer indicating a neuron of that specific case
     * @return neurons[i] : a pointer on a neuron of the i corresponding case
     */
	Neuron* getNeuron(unsigned int i) const;
	
	/*!
     * @brief Get the whole vector of neurons
     *
     * @return neurons : the vector of the whole network composed of pointer on 12500 neurons
     */
	std::array<Neuron*, total_neurons> getNeurons() const;
	
	/*!
     * @brief Set a pointer on a neuron corresponding to a specific box
     *
     * @param i : a positive integer indicating a neuron of a specific box
     * @param n : a pointer on a neuron we want to insert of that i box
     */
	void setNeurons(unsigned int i, Neuron* n);
	
	/*!
     * @brief The destructor of the class Network
     */
	~Network();
	
	
private:
	std::array <Neuron*, total_neurons> neurons; //!< Vector of neurons populating the network
};

#endif
