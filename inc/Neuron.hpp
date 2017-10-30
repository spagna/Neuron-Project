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
constexpr int t_stop (10000); //step simulation stops 
constexpr int n_clock_start (0); //neuron clock starts at step 1000 (at 100.0 in milliseconds) 
constexpr int n_clock_stop (10000); //neuron clock stops at step 4000 (at 400.0 in milliseconds) 
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
			double t_spike = 0.0, bool r_period = false,
			bool spike = false,	int neuron_clock = 0, 
			double external_input = 0.0,
			bool excitatory_neuron = true);
	
	double getV_membrane() const;
	int getNumberSpikes() const;
	double getTimeSpike() const;
	bool getRefractory() const; //return true if the neuron is in its refractory period
	bool getSpikeState() const; 
	int getNeuronClock() const;
	double getExternalInput() const;
	double getTimeBuffer (int i) const; //return the i element of the time buffer
	Neuron* getTargetNeuron (int i) const;
	bool getExcitatoryNeuron() const;
	int getExcitatoryConnections() const;
	int getInhibitoryConnections() const;
	
	void setV_membrane(double V_membrane);
	void setNumberSpikes(unsigned int nb);
	void setTimeSpike (double t);
	void setRefractory(bool refractory);
	void setSpikeState (bool spike);
	void setNeuronClock (int clock);
	void setExternalInput (double external_input);
	void setTimeBuffer (int i, double val); //set the value val in the case i of the buffer
	void setTargetNeuron (int i, Neuron* n);
	void setExcitatoryNeuron (bool n);
	void setExcitatoryConnections(int nb);
	void setInhibitoryConnections(int nb);
	
	size_t numberOfTargets() const;
	
	void addTimeBuffer (int i, double val); //this function add a value to a specif case of the time buffer, and if another values different from 0 is alredy present, it increments it
	void addTargetNeuron (Neuron* n);

		
	void updateNeuronState (int dt); //if the potential cross the threshold, this function update all the attributes of the neuron	
	void update(int dt); //at every time step, the neuron controle if he received an input or not and updates 	
	void receive (int time, double ampl); //if the neuron is a post-synaptic neuron, and it receives an input from another neuron (at time time and with amplitude J), this function update its state
	
	void addConnections (std::array<Neuron*, 12500>  neurons); //add connections between all neurons of the network. 
	int randomSpikes (); //generates the random spikes arrived from outside the brain at every time step
	
	~Neuron();
	
	

private:
	double V_membrane_; //membrane potential
	unsigned int nb_spikes_; //number of spikes
	double t_spike_; //time when a spike occured
	bool r_period_; //this attribute tell if the neuron is in a refracgtory period or not
	bool spike_; //true if a spike occured during a single time step, false otherwise
	int neuron_clock_; //takes the count of the time step passed
	double external_input_; //set the arbitrary input received 	
	std::array <double, D+1> t_buffer_; //stock the value J every time that a spike occured but with a certain delay
	std::vector <Neuron*> n_target_; //this vector contains all the post synaptic neurons (the targets for the current neuron)
	bool excitatory_neuron_; //tell if a neuron is excitatory or inhibitory: true if exctitatory and false if inhibitory
	int nb_excitatory_connections;
	int nb_inhibitory_connections;
};

#endif


