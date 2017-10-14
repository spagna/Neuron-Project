#ifndef NEURON_H
#define NEURON_H

#include <iostream>
#include <fstream>

//here times are transformed in steps of simulation, to make comparisons more accurate
//to have the exact value in milliseconds, it has to be multiplied by h 
constexpr int t_start (0); //step simulation starts
constexpr int t_stop (5000); //step simulation stops 
constexpr int tau (200); //time constant (20.0 in milliseconds) * h (note tau = R/C)
constexpr unsigned int C (1); //number of connections from other neurons
constexpr double V_thr (20.0); //firing threshold in microvolts 
//constexpr double V_reset (10.0); //reset potential in microvolts
constexpr double V_refractory (0.0);
constexpr int tau_rp (20); //refractory period (2.0 in milliseconds) * h
constexpr double h (0.1); //equidistant step of time during which the simulation evolves
constexpr int N(1); //step of simulations 


class Neuron 
{
public:
	Neuron (double membranePotential = 0.0, unsigned int nb_spikes = 0, double t_spike = 0.0, bool r_period = false);
	
	double getV_membrane() const;
	int getNumberSpikes() const;
	double getTimeSpike() const;
	bool getRefractory() const; //return true if the neuron is in its refractory period
	
	void setV_membrane(double V_membrane);
	void setNumberSpikes(unsigned int nb);
	void setTimeSpike (double t);
	void setRefractory(bool refractory);
	
	void update(int dt);
	void storeDatas(std::ofstream file);
	
	
	
	
	~Neuron();
	
	

private:
	double V_membrane_; //membrane potential
	unsigned int nb_spikes_; //number of spikes
	double t_spike_; //time when a spike occured
	bool r_period_; //this attribute tell if the neuron is in a refracgtory period or not
	
};

#endif


