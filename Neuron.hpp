#ifndef NEURON_H
#define NEURON_H

#include <iostream>
#include <fstream>

constexpr double t_start (0.0); //in milliseconds
constexpr double t_stop (500.0);
constexpr double tau (20.0); //time constant in milliseconds (note tau = R/C)
constexpr unsigned int C (1); //number of connections from other neurons
constexpr double V_thr (20.0); //firing threshold in microvolts 
constexpr double V_reset (10.0); //reset potential in microvolts
constexpr double V_refractory (0.0);
constexpr double tau_rp (2.0); //refractory period in milliseconds
constexpr double h (0.1); //equidistant step of time during which the simulation evolves


class Neuron 
{
public:
	Neuron (double membranePotential = 0.0, unsigned int nb_spikes = 0, double t_spike = 0.0, bool r_period = false);
	
	double getV_membrane() const;
	int getNumberSpikes() const;
	double getTimeSpike(); 
	
	void setV_membrane(double V_membrane);
	void setNumberSpikes(unsigned int nb);
	void setTimeSpike (double t);
	
	void update(double dt);
	void storeDatas(std::ofstream file);
	
	
	
	~Neuron();
	
	

private:
	double V_membrane_; 
	unsigned int nb_spikes_; //number of spikes
	double t_spike_; //time when a spike occured
	bool r_period_; //if true the neuron is in its refractory period
	
};

#endif



