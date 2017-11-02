#ifndef NEURON_H
#define NEURON_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <array>
#include <vector>
#include <cassert> 
#include <random>


//here times are transformed in steps of simulation, to make comparisons more accurate
//to have the exact value in milliseconds, it has to be multiplied by h 
constexpr int t_start (0); //step simulation starts
constexpr int t_stop (5000); //step simulation stops 
constexpr int n_clock_start (1000); //neuron clock starts at step 1000 (at 100.0 in milliseconds) 
constexpr int n_clock_stop (4000); //neuron clock stops at step 4000 (at 400.0 in milliseconds) 
constexpr int tau (200); //time constant (20.0 in milliseconds) * h (note tau = R/C)

constexpr double V_thr (20.0); //firing threshold in microvolts 
constexpr double V_refractory (0.0);
constexpr int tau_rp (20); //refractory period (2.0 in milliseconds) * h

constexpr double h (0.1); //equidistant step of time during which the simulation evolves
constexpr int N(1); //step of simulations 
constexpr int D(15); //delay (how many time after the post-synpatic neuron receives a spike) (=1.0 in milliseconds)
constexpr double J(0.1); //amplitude of the signal 

constexpr unsigned int C (1); //number of connections from other neurons
constexpr double R (20.0); //membrane resistance
constexpr double const1 (exp(-h/(tau*h))); //constant 1 of the membrane equation
constexpr double const2 (R*(1 - const1)); //constante 2 of the membrane equation

constexpr int total_neurons (12500); //total number of neurons in the network
constexpr int excitatory_neurons (10000); //number of excitatory neurons
constexpr int inhibitory_neurons (2500); //number of inhibitory neurons
constexpr int c_e (1000); //connections received from excitatory neurons
constexpr int c_i (250); //connections received from inhibitory neurons
constexpr int c_ext (c_e); //connections received from the rest of the brain 
constexpr double J_e (0.1); //amplitude of the signal from an excitatory neuron
constexpr double J_i (0.5); //amplitude of the signal from an inhibitory neuron

constexpr double v_ext (0.02);//(V_thr/(nb_excitatory_c*J_e*tau)); //rate at which every neuron receives additional random input from the rest of the brain
constexpr double poisson_gen(v_ext*c_e*h*J_e); //rate of possion generator 

class Neuron 
{
public:
	Neuron (double membranePotential = 0.0, unsigned int nb_spikes = 0, 
			double t_spike = 0.0, bool spike = false,	
			int neuron_clock = 0, 
			double external_input = 0.0,
			bool excitatory_neuron = true);
	
	 /*!
     * @brief Get the membrane potential 
     *
     * @return the membrane potential
     */
	double getV_membrane() const;
	 /*!
     * @brief Get the number of spikes done by a neuron 
     *
     * @return the number of spikes
     */
	int getNumberSpikes() const;
	 /*!
     * @brief Get the time in milliseconds of the last spike that has occured 
     *
     * @return the time of the last spike occured
     */
	double getTimeSpike() const;
	 /*!
     * @brief Get the spike state, that is true if a neuron has spiked in a certain time step
     *		  This function allows to know if the neuron has spiked in the current time step 
     * @return if the spike occured or not
     */
	bool getSpikeState() const; 
	/*!
     * @brief Get the local time of the neuron, which is different form the time of the simulation
     *		  
     * @return the neuron clock
     */
	int getNeuronClock() const;
	/*!
     * @brief Get the external input received by one or two neurons. 
     *		  When the simulation has more than two neurons, the external input is always 0.0 
     * @return the external input received
     */
	double getExternalInput() const;
	/*!
	 * @brief Get the amplitude stored in the time buffer in a specific case
	 * 		  The amplitude is stored depending on the current time and the delay.
	 *
	 * @param a integer refering to a certain case 
	 * @return the amplitude stored in that specific case
	 */
	double getTimeBuffer (int i) const; 
	/*!
	 * @brief Get one specific neuron chosen between the vector of target neurons
	 *
	 * @param an integer refering to the neuron needed
	 * @return a pointer on the corresponding element 
	 */
	Neuron* getTargetNeuron (int i) const;
	/*!
	 * @brief Get the role of the neuron.
	 * 		  If the boolean is true, the neuron is an excitatory neuron, 
	 * 		  if false it's an inhibitory neuron.
	 *
	 * @return true or false depending on the role of the neuron
	 */
	bool getExcitatoryNeuron() const;
	/*!
	 * @brief Get the number of excitatory connections received by a neuron.
	 * 		  This getter is needed for tests.
	 *
	 * @return the number of excitatory connections
	 */
	int getExcitatoryConnections() const;
	/*!
	 * @brief Get the number of inhibitory connections received by a neuron.
	 * 		  This getter is needed for tests.
	 *
	 * @return the number of inhibitory connections
	 */
	int getInhibitoryConnections() const;
	
	
	/*!
	 * @brief Set the membrane potential with the value in parameter
	 *
	 * @param a double containing the new value of the membrane potential
	 */
	void setV_membrane(double V_membrane);
	/*!
	 * @brief Set the number of spikes with the value in parameter
	 *
	 * @param a positive integer containing the new number of spikes
	 */
	void setNumberSpikes(unsigned int nb);
	/*!
	 * @brief Set the time of the last occured spike in milliseconds
	 *
	 * @param a double containing the new time of the last spike 
	 */
	void setTimeSpike (double t);
	/*!
	 * @brief Set the spike state to true if a spike has occured
	 *
	 * @param a boolean corresponding to true if the spike has occured
	 */
	void setSpikeState (bool spike);
	/*!
	 * @brief Set the time of the local clock in time step
	 *		  The time step is given by the time in milliseconds multiplied by the h value
	 * 		  that correspon to the time interval between each step
	 * @param an integer refering to a certain time step 
	 */
	void setNeuronClock (int clock);
	/*!
	 * @brief Set the external current received 
	 *
	 * @param a double indicating the external input received
	 */
	void setExternalInput (double external_input);
	/*!
	 * @brief Set a case of the time buffer 
	 *
	 * @param an integer indicating the case that has to be filled
	 * @param a double indicating the amplitude of the signal that need to be stored
	 */
	void setTimeBuffer (int i, double val); //set the value val in the case i of the buffer
	/*!
	 * @brief Set a case of the vector of target neurons.
	 *
	 * @param an integer indicating the case that has to be filled
	 * @param a pointer on the neuron that has to replace another target or an empty case
	 */
	void setTargetNeuron (int i, Neuron* n);
	/*!
	 * @brief Set the role of a neuron
	 *		  The parameter is setted to true when the neuron is defined as an excitatory neuron, 
	 * 		  and false if it's an inhibitory neuron
	 * @param a boolean indicating the role
	 */
	void setExcitatoryNeuron (bool n);
	/*!
	 * @brief Set the number of the excitatory connections received
	 *
	 * @param an integer indicating the new number of connections
	 */
	void setExcitatoryConnections(int nb);
	/*!
	 * @brief Set the number of the inhibitory connections received
	 *
	 * @param an integer indicating the new number of connections
	 */
	void setInhibitoryConnections(int nb);


	/*!
	 * @brief Fill the time buffer 
	 * 		  The time buffer of the current neuron is filled with a certain amplitude
	 * 		  of a signal it just has received. If another value was already present in the case, 
	 * 		  the new value is summed.
	 * 		  To decide the case that has to be filled, the neuron takes in account the current local
	 * 		  time and the delay. 
	 *
	 * @param an integer inidcating the case that has to be filled
	 * @param a double indicating the amplitude of the signal received that has to be stored
	 */
	void addTimeBuffer (int i, double val); //this function add a value to a specif case of the time buffer, and if another values different from 0 is alredy present, it increments it
	/*!
	 * @brief Fill the vector of target neurons
	 * 		  Each time a neuron is connected to another neuron, it has to store a pointer on the
	 * 		  target neuron in a vector to remember it. So, when the neuron spikes, it just has to 
	 * 		  accede the vector to find all the target and send them the amplitude of the spike.
	 * 		  To add the pointer, because the order is not important, the function push_back is used.
	 *
	 * @param a pointer to a neuron that has to be stored as target neuron
	 */
	void addTargetNeuron (Neuron* n);
	/*!
	 * @brief Update the neuron's parameters when the membrane potential cross the threshold and the neuron spikes
	 *		  When a neuron spikes, some attributes have to change. The function stores the time of the 
	 * 		  new spike in milliseconds, increase the number of spikes occured and set the state of the 
	 * 		  spike to true. 
	 * @param an integer indicating the time the spike has occured and has to be stored in temrs of time step
	 * 		  The parameter corresponds to the local time of the neuron.
	 */
	void updateNeuronState (int dt); //if the potential cross the threshold, this function update all the attributes of the neuron	
	/*!
	 * @brief Update the neuron's target (their parameters) when the neuron spikes
	 * 		  When a neuron spikes, the time buffer of the target neurons has to be filled with the
	 * 	      amplitude of the singal sended by the spiking neuron, depending on the role of the neuron.
	 * 	      The case filled based on the local time of the spiking neuron and the delay of reception.
	 * 		  
	 */
	void updateTargets(); //if there is a spike, the buffers of all the targets of the neuron have to be filled with the corrisponding amplitude of the spike
	/*!
	 * @brief Update the neuron depending on its situation.
	 * 		  A neuron has three possible situations: it's membrane potential could cross the threshold
	 * 		  and so the neruon would spike, it could be in his refractory period if a spike occured
	 * 		  in less than 2 milliseconds or neither of them and so the neuron would just update it's 
	 * 		  membrane potential depending on the external input received and the amplitudes stored
	 * 		  in its time buffer.
	 * @param an integer indicating the time step. Every time step the neuron has to be checked and updated.
	 */
	void update(int dt); //at every time step, the neuron controle if he received an input or not and updates 	


	/*!
	 * @brief Solve the membrane equation and update the membrane potential
	 * 		  During each time step interval, the neuron has to update its membrane potential
	 * 		  depending on all the external inputs received. 
	 * 		  The equation depends on some external parameters and two constants.
	 * 		 
	 * @param a double indicating the external input
	 * @param a double indicating the amplitude of the external signal that randomly occured
	 */
	void solveMembraneEquation(double input, double random);
	
	/*!
	 * @brief Add the connections between all neurons in the network
	 * 		  Before the simulation starts, the connections have to be setted. 
	 * 		  Every neuron receives 1000 excitatory connections and 250 inhibitory connections.
	 * 		  Those connections are choosen randomly. 
	 * @param an array of pointer on neurons correponding to the neurons of the whole network (12500).
	 */
	void addConnections (std::array<Neuron*, total_neurons>  neurons); //add connections between all neurons of the network. 
	
	/*!
	 * @brief Generate random amplitudes from the rest of the brain
	 * 		  At each time step, a neuron receives a certain number of amplitudes that come 
	 * 		  from the rest of the brain. The number of random spikes received is implemented
	 * 		  by a poisson generator.
	 * 		  The external neurons spike randomly and give only excitatory inpulses. 
	 *
	 * @return the total amplitudes: the amplitude given by an excitatory neuron multiplied by the number
	 * 		   generated by the poisson distribution.
	 */
	double randomSpikes (); //generates the random spikes arrived from outside the brain at every time step
	
	~Neuron();
	
	

private:
	//!< // Membrane potential in millivolts  */
	double V_membrane_; 
	//!< // Number of spikes stored during the simulation  */
	unsigned int nb_spikes_; 
	//!< // Time the last spike occured in milliseconds  */
	double t_spike_; 
	//!< // True if a spike has occured during the last update  */ 
	bool spike_;
	//!< // Local time of the neuron calculated in time step intervals
	/*! Take the count of the time step interval */
	int neuron_clock_; 
	//!< // External input received in millivolts  */		
	double external_input_; 
	//!< // Time buffer  */
	/*! The time buffer has a size of D+1 to allow to correct add the amplitudes in a case
	 * that will be read with the correct delay by a target neuron.
	 * It stores the amplitude/the sum of the amplitudes of the signals received by a neuron */
	std::array <double, D+1> t_buffer_;  
	//!< // Vector of targets  */
	/*! Each neuron has a number of post synaptic neurons that 
	 * has to update when a spike occurs */
	std::vector <Neuron*> n_target_; 
	//!< // Determine the role of a neuron  */
	/*! If this member is true, the neuron will be an excitatory neuron.
	 * Otherwise it will be an inhibitory neuron.
	 * An excitatory neuron will give a different amplitude from an inhibitory neuron. */
	bool excitatory_neuron_; 
	//!< // Number of excitatory connections received by a neuron  */
	int nb_excitatory_connections_;
	//!< // Number of inhibitory connections received by a neuron  */
	int nb_inhibitory_connections_;
};

#endif


