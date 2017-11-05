#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>

/*!
     * @class Simulation
     */
     
class Simulation
{
public:

	/*!
     * @brief Constructor of the Simulation class
     */
	Simulation();
	/*!
     * @brief Simulate the brunel's network with one neuron
     * @details The neuron in the network has to spikes when its membrane potential cross the threshold. 
     * The membrane potential is increased by the external input received. If the input is between 0 and 1, nothing happens.
     * The membrane potential increases by the threshold is not crossed. If the input is more than 1, the neuron will spikes.
     * If we test the model with a input of 1.01, we will see five spikes and their relative time in the terminal. 
     * The user decides the value of the external input.
     * For this model there is a file that stores the values of the memrbane potential at each time steps.
     * There are no noises coming from external random spikes.
     */
	void oneNeuronSimulation();
	/*!
     * @brief Simulate the brunel's network with two neurons
     * @details One neuron is the spiking neuron, the other one is the post-synaptic neuron.
     * The second one will be added to the target of the first one and he will receives the spikes. 
     * An external input is added for the spiking neuron but not for the post-synaptic neuron. 
     * When the first neuron spikes, the time will be stored and the second neuron will recevies the signal 
     * after a certain delay. On the terminal there are the corresponding values of spikes and received spikes.
     * The use decides the value of the external input.
     * There are no noises coming from external random spikes
     * 
     */
	void twoNeruonsSimulation();
	/*!
     * @brief Simulate the brunel's network with the whole network of 12500 neurons.
     * @details An array of neurons will be created to store all the inhibitory and excitatory neurons. After initializing it,
     * the conncetions are created between all the neurons. Finally all the neurons are updated. When a neuron spikes it sends 
     * the corresponding amplitude to all the post-synaptic neurons which will see their membrane potential increase, and will
     * spike too, and so on for the whole simulation.
     * The noises from the rest of the brain are presents and contribute increasing the memrane potential at each time step.
     * There is a file that stores all the time a neuron spikes and its index. This will allow creating the 4 graphs of the
     * brunel's network.
     * 
     * @param g :  an integer indicating the rate between inhibitory connections and excitatory connections 
     * @param pois : an integer indicating the rate between inhibitory connections and excitatory connections 
     * 
     * 
     */
	void networkSimulation(int g, int pois);
	
	void plotGraph_A();
	
	
	
	/*!
     * @brief Destructor of the class Simulation 
     */
	~Simulation();

private:
	
};

#endif


