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

/*!
     * @class Neuron
     * @details This class represent a single neuron that simulate the Brunel's network.
     * Each network has certain specific members describing its behaviour and what happens when 
     * a spike occured or a spike is received. 
     * Every neuron is connected with other neuron and each neuron receives a total of 1000 excitatory connection
     * and 250 inhibitory connections. The connections are created randomly. 
     * Moreover, each neuron receives some random external input from the rest of the brain. Those random
     * spikes are generated by a poisson generator. 
     * The membrane potential of the neuron is updated at the end of each time step of the simulation.
     * When it cross a certain threshold, the neruon spikes and sends the amplitude of the spike to all
     * its target neurons. 
     * Each neuron enters in refractory time for a certain period after it spikes, and then start incrising
     * its membrane potential again.
     * The neurons are devided in excitatory and inhibitory neurons: the excitatory ones send 
     * positive amplitudes to the targets, the inhibitory ones negative amplitudes. 
     */

class Neuron 
{
public:
	/*!
     * @brief The constructor of the class neuron.
     * @details The constructor initialize the following argument by default:
     * the membrane potential is 0.0 mV, the number of spikes is 0, the time of the last spike is 0.0 msec,
     * the spike state is false because no spikes has occured yet,
     * the local neuron clock is 0 time step, the external input is 0.0 mV.
     * The only argument needed by the neuron at the moment of its creation is the boolean excitatory.
     * It's true if we want to create an excitatory neuron and false for an inhibitory one.
     * The time buffer is initialize in the .cpp file, and all the D+1 cases are inizialized to 0.0 mV.
     * 
     * @param excitatory_neuron : a boolean indicating that the neuron is excitatory
     * @param membranePotential : a double indicating the initial membrane potential
     * @param nb_spikes : a positive integer indicating the initial number of spikes
     * @param t_spike : a double indicating the initial time stored for a spike in milliseconds
     * @param spike : a boolean indicating that no spikes occured before the simulation starts
     * @param neuron_clock : an integer indicating the initial local time of the neuron in terms of time steps
     * @param external_input : a double indicating the initial input percived 
     *
     */
	
	Neuron (bool excitatory_neuron,
			double membranePotential = 0.0, unsigned int nb_spikes = 0, 
			double t_spike = 0.0, bool spike = false,	
			int neuron_clock = 0, double external_input = 0.0);
	
	 /*!
     * @brief Get the membrane potential.
     *
     * @return A double V_membrane_: the membrane potential
     */
	double getV_membrane() const;
	 /*!
     * @brief Get the number of spikes done by a neuron.
     *
     * @return A positive integer nb_spikes_: the number of spikes 
     */
	int getNumberSpikes() const;
	 /*!
     * @brief Get the time in milliseconds of the last spike that has occured.
     *
     * @return A double t_spike_: the time of the last spike occured 
     */
	double getTimeSpike() const;
	 /*!
     * @brief Get the spike state, that is true if a neuron has spiked in a certain time step.
     * @details This function allows to know if the neuron has spiked in the current time step 
     * 
     * @return A boolean spike_: if the spike occured (true) or not (false) 
     */
	bool getSpikeState() const; 
	/*!
     * @brief Get the local time of the neuron, which is different form the time of the simulation
     *		  
     * @return An integer neuron_clock_: the neuron local clock in term of time step
     */
	int getNeuronClock() const;
	/*!
     * @brief Get the external input received by one or two neurons. 
     * @details When the simulation has more than two neurons, the external input is always 0.0 
     * 
     * @return A double external_input_: the external input received external_input_
     */
	double getExternalInput() const;
	/*!
	 * @brief Get the amplitude stored in the time buffer in a specific case
	 * @details The amplitude is stored depending on the current time and the delay.
	 *
	 * @param i : an integer refering to a certain case 
	 * @return A double t_buffer[i]: the amplitude stored in the specific case i
	 */
	double getTimeBuffer (int i) const; 
	/*!
	 * @brief Get one specific neuron chosen between the vector of target neurons
	 *
	 * @param i: an integer refering to the neuron needed
	 * @return A pointer Neuron*: points on the i element of the target's vector
	 */
	Neuron* getTargetNeuron (int i) const;
	/*!
	 * @brief Get the role of the neuron.
	 * @details If the boolean is true, the neuron is an excitatory neuron, 
	 * if false it's an inhibitory neuron.
	 *
	 * @return A boolean excitatory_neuron: true or false depending on the role of the neuron
	 */
	bool getExcitatoryNeuron() const;
	/*!
	 * @brief Get the number of excitatory connections received by a neuron.
	 * @details This getter is needed for tests.
	 *
	 * @return An integer nb_excitatory_connections: the number of excitatory connections
	 */
	int getExcitatoryConnections() const;
	/*!
	 * @brief Get the number of inhibitory connections received by a neuron.
	 * @details This getter is needed for tests.
	 *
	 * @return An integer nb_inhibitory_connections: the number of inhibitory connections
	 */
	int getInhibitoryConnections() const;
	
	
	/*!
	 * @brief Set the membrane potential with the value in parameter
	 *
	 * @param V_membrane : a double containing the value of the membrane potential
	 */
	void setV_membrane(double V_membrane);
	/*!
	 * @brief Set the number of spikes with the value in parameter
	 *
	 * @param nb : a positive integer containing the number of spikes
	 */
	void setNumberSpikes(unsigned int nb);
	/*!
	 * @brief Set the time of the last occured spike in milliseconds
	 *
	 * @param t : a double containing the time of the last spike 
	 */
	void setTimeSpike (double t);
	/*!
	 * @brief Set the spike state to true if a spike has occured
	 *
	 * @param spike : a boolean corresponding to true if the spike has occured
	 */
	void setSpikeState (bool spike);
	/*!
	 * @brief Set the time of the local clock in time step
	 * @details The time step is given by the time in milliseconds multiplied by the h value
	 * that correspon to the time interval between each step.
	 * 
	 * @param clock : an integer refering to a certain time step 
	 */
	void setNeuronClock (int clock);
	/*!
	 * @brief Set the external current received 
	 *
	 * @param external_input : a double indicating the external input received
	 */
	void setExternalInput (double external_input);
	/*!
	 * @brief Set a case of the time buffer 
	 *
	 * @param i : an integer indicating the case that has to be filled
	 * @param val : a double indicating the amplitude of the signal that need to be stored
	 */
	void setTimeBuffer (int i, double val); //set the value val in the case i of the buffer
	/*!
	 * @brief Set a case of the vector of target neurons.
	 *
	 * @param i : an integer indicating the case that has to be filled
	 * @param n : a pointer on the neuron that has to replace another target or an empty case
	 */
	void setTargetNeuron (int i, Neuron* n);
	/*!
	 * @brief Set the role of a neuron
	 * @details The parameter is setted to true when the neuron is defined as an excitatory neuron, 
	 * and false if it's an inhibitory neuron
	 * 
	 * @param n : a boolean indicating the role
	 */
	void setExcitatoryNeuron (bool n);
	/*!
	 * @brief Set the number of the excitatory connections received
	 *
	 * @param nb : an integer indicating the new number of connections
	 */
	void setExcitatoryConnections(int nb);
	/*!
	 * @brief Set the number of the inhibitory connections received
	 *
	 * @param nb : an integer indicating the new number of connections
	 */
	void setInhibitoryConnections(int nb);


	/*!
	 * @brief Fill the time buffer 
	 * @details The time buffer of the current neuron is filled with a certain amplitude 
	 * of a signal it just has received. If another value was already present in the case, 
	 * the new value is summed. To decide the case that has to be filled, 
	 * the neuron takes in account the current local time and the delay. 
	 *
	 * @param i : an integer inidcating the case that has to be filled
	 * @param val : a double indicating the amplitude of the signal received that has to be stored
	 */
	void addTimeBuffer (int i, double val); //this function add a value to a specif case of the time buffer, and if another values different from 0 is alredy present, it increments it
	/*!
	 * @brief Fill the vector of target neurons
	 * @details Each time a neuron is connected to another neuron, it has to store a pointer on the 
	 * target neuron in a vector to remember it. So, when the neuron spikes, it just has to
	 * accede the vector to find all the target and send them the amplitude of the spike.
	 * To add the pointer, because the order is not important, the function push_back is used.
	 *
	 * @param n : a pointer to a neuron that has to be stored as target neuron
	 */
	void addTargetNeuron (Neuron* n);
	/*!
	 * @brief Update the neuron's parameters when the membrane potential cross the threshold and the neuron spikes
	 * @details When a neuron spikes, some attributes have to change. The function stores the time 
	 * of the new spike in milliseconds, increase the number of spikes occured 
	 * and set the state of the spike to true. 
	 * 
	 * @param dt : an integer indicating the time the spike has occured and has to be stored in temrs of time step
	 * @details The parameter corresponds to the local time of the neuron.
	 */
	void updateNeuronState (int dt); //if the potential cross the threshold, this function update all the attributes of the neuron	
	/*!
	 * @brief Update the neuron's target (their parameters) when the neuron spikes
	 * @details When a neuron spikes, the time buffer of the target neurons has to be filled with the
	 * amplitude of the singal sended by the spiking neuron, depending on the role of the neuron. 
	 * The case filled based on the local time of the spiking neuron and the delay of reception.
	 * 		  
	 */
	void updateTargets(); //if there is a spike, the buffers of all the targets of the neuron have to be filled with the corrisponding amplitude of the spike
	/*!
	 * @brief Update the neuron depending on its situation.
	 * @details A neuron has three possible situations: it's membrane potential could cross the threshold
	 * and so the neruon would spike, it could be in his refractory period if a spike occured 
	 * in less than 2 milliseconds or neither of them and so the neuron would just update it's
	 * membrane potential depending on the external input received and the amplitudes stored in its time buffer.
	 * When a neuron is updated, he has to receive the amplitudes of the noise in parameters. This also
	 * allows futher simulations or tests
	 * 
	 * @param noise : a double indicating the random amplitudes recevied by the exterior 
	 * @param dt : an integer indicating the time step. Every time step the neuron has to be checked and updated.
	 * 
	 */
	void update(int dt, double noise); //at every time step, the neuron controle if he received an input or not and updates 	


	/*!
	 * @brief Solve the membrane equation and update the membrane potential
	 * @details During each time step interval, the neuron has to update its membrane potential 
	 * depending on all the external inputs received. 
	 * The equation depends on some external parameters and two constants.
	 * 		 
	 * @param input : a double indicating the external input
	 * @param random : a double indicating the amplitude of the external signal that randomly occured
	 */
	void solveMembraneEquation(double input, double random);
	
	/*!
	 * @brief Add the connections between all neurons in the network
	 * @details Before the simulation starts, the connections have to be setted.
	 * Every neuron receives 1000 excitatory connections and 250 inhibitory connections.
	 * Those connections are choosen randomly. 
	 * 
	 * @param neurons : an array of pointer on neurons correponding to the neurons of the whole network (12500).
	 */
	void addConnections (std::array<Neuron*, total_neurons>  neurons); //add connections between all neurons of the network. 
	
	/*!
	 * @brief Generate random amplitudes from the rest of the brain
	 * @details At each time step, a neuron receives a certain number of amplitudes that come
	 * from the rest of the brain. The number of random spikes received is implemented by a poisson generator.
	 * The external neurons spike randomly and give only excitatory inpulses. 
	 *
	 * @return A double J_e*dis_ext(gen): the total amplitudes.
	 * @details The amplitude given by an excitatory neuron multiplied by the number 
	 * generated randomly by the poisson distribution.
	 */
	double randomSpikes (); //generates the random spikes arrived from outside the brain at every time step
	
	/*!
     * @brief The destructor of the class Neuron
     */
	~Neuron();
	
	

private:
	
	double V_membrane_; //!< Membrane potential in millivolts  
	
	unsigned int nb_spikes_; //!< Number of spikes stored during the simulation  
	
	double t_spike_; //!< Time the last spike occured in milliseconds  
	
	bool spike_; //!< True if a spike has occured during the last update  
	
	int neuron_clock_; //!< Local time of the neuron calculated in time step intervals
	//!< Take the count of the time step interval 
	
	double external_input_; //!< External input received in millivolts  	
	
	std::array <double, D+1> t_buffer_;//!<  Time buffer  
	//!< The time buffer has a size of D+1 to allow to correct add the amplitudes in a case
	//!< that will be read with the correct delay by a target neuron.
	//!< It stores the amplitude/the sum of the amplitudes of the signals received by a neuron 	
	
	std::vector <Neuron*> n_target_; //!< Vector of targets 
	//!< Each neuron has a number of post synaptic neurons that 
	//!< has to update when a spike occurs 
	
	bool excitatory_neuron_; //!< Determine the role of a neuron  
	//!< If this member is true, the neuron will be an excitatory neuron.
	//!< Otherwise it will be an inhibitory neuron.
	//!< An excitatory neuron will give a different amplitude from an inhibitory neuron. 
	
	int nb_excitatory_connections_; //!< Number of excitatory connections received by a neuron 
	
	int nb_inhibitory_connections_; //!< Number of inhibitory connections received by a neuron  
};

#endif


