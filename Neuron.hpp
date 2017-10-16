#ifndef NEURON_H
#define NEURON_H

#include <iostream>
#include <fstream>
#include <array>

//here times are transformed in steps of simulation, to make comparisons more accurate
//to have the exact value in milliseconds, it has to be multiplied by h 
constexpr int t_start (0); //step simulation starts
constexpr int t_stop (5000); //step simulation stops 
constexpr int n_clock_start (1000); //neuron clock starts at step 1000 (at 100.0 in milliseconds) 
constexpr int n_clock_stop (4000); //neuron clock stops at step 4000 (at 400.0 in milliseconds) 
constexpr int tau (200); //time constant (20.0 in milliseconds) * h (note tau = R/C)
constexpr unsigned int C (1); //number of connections from other neurons
constexpr double V_thr (20.0); //firing threshold in microvolts 
//constexpr double V_reset (10.0); //reset potential in microvolts
constexpr double V_refractory (0.0);
constexpr int tau_rp (20); //refractory period (2.0 in milliseconds) * h
constexpr double h (0.1); //equidistant step of time during which the simulation evolves
constexpr int N(1); //step of simulations 
constexpr int D(15); //delay (how many time after the post-synpatic neuron receives a spike) (=1.0 in milliseconds)
constexpr double J(0.2); //amplitude of the signal 


class Neuron 
{
public:
	Neuron (double membranePotential = 0.0, unsigned int nb_spikes = 0, 
			double t_spike = 0.0, bool r_period = false, bool spike = false,
			double neuron_clock = 0.0, double external_input = 0.0);
	
	double getV_membrane() const;
	int getNumberSpikes() const;
	double getTimeSpike() const;
	bool getRefractory() const; //return true if the neuron is in its refractory period
	bool getSpikeState() const; 
	double getNeuronClock() const;
	double getExternalInput() const;
	double getTimeBuffer (int i) const; //return the i element of the time buffer
	
	void setV_membrane(double V_membrane);
	void setNumberSpikes(unsigned int nb);
	void setTimeSpike (double t);
	void setRefractory(bool refractory);
	void setSpikeState (bool spike);
	void setNeuronClock (double clock);
	void setExternalInput (double external_input);
	void setTimeBuffer (int i, double val); //set the value val in the case i of the buffer
	
	void addTimeBuffer (int i, double val);

		
	void updateNeuronState (int dt); //if the potential cross the threshold, this function update all the attributes of the neuron	
	void update(int dt);	
	void receive (int time, double ampl); //if the neuron is a post-synaptic neuron, and it receives an input from another neuron (at time time and with amplitude J), this function update its state
	
	~Neuron();
	
	

private:
	double V_membrane_; //membrane potential
	unsigned int nb_spikes_; //number of spikes
	double t_spike_; //time when a spike occured
	bool r_period_; //this attribute tell if the neuron is in a refracgtory period or not
	bool spike_; //true if a spike occured during a single time step, false otherwise
	double neuron_clock_;
	double external_input_; //set the arbitrary input received 	
	std::array <double, D+1> t_buffer_;
};

#endif


