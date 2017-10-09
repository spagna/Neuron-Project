#include "Neuron.hpp"
#include <iostream>
#include <cmath>

Neuron::Neuron (double V_membrane, unsigned int nb_spikes, double t_spike, bool refractory)
: V_membrane_ (V_membrane), nb_spikes_(nb_spikes), t_spike_(t_spike), r_period_(refractory)
{} 

double Neuron::getV_membrane() const
{
	return V_membrane_;
}

int Neuron::getNumberSpikes() const
{
	return nb_spikes_;
}

double Neuron::getTimeSpike()
{
	return t_spike_;
}

void Neuron::setV_membrane(double V_membrane)
{
	V_membrane_ = V_membrane;
	
}

void Neuron::setNumberSpikes(unsigned int nb)
{
	nb_spikes_ = nb;
}

void Neuron::setTimeSpike (double t)
{
	t_spike_ = t;
}

void Neuron::update(double dt)
{
	double simulation_time (0.0);
	double R = tau/C; //membrane resistance
	double external_input (0.0);
	double const1 =  exp(-h/tau);
	std::cout << "Choose a value for the external input" << std::endl; // rem: the value should be >=0
	std::cin >> external_input;
	simulation_time = t_start;
	//nb_spikes_ = 0;
	do {
		if (V_membrane_ > V_thr){ // if the membrane potential crosses the threshold
			t_spike_ = simulation_time; //store the time of the spike
			std::cout << "The spike occurred at: " << t_spike_ << std::endl;
			r_period_ = true; // the neuron enters in its refractory period
			++nb_spikes_; //the number of spike increases of one
		} 
		if (r_period_ and tau_rp >= simulation_time - t_spike_){ //if the neuron is in its refractory state and it's still in the refractory period
			V_membrane_ = V_refractory; //the membrane potential is zero
		} else {
			V_membrane_ = const1*V_membrane_ + external_input*R*(1-const1); //solve the membrane equation
		}
		if (simulation_time - t_spike_ >= tau){ //when the refractory period has finished, the neuron is no more in this state
			r_period_ = false;
		}
		simulation_time += dt;	//the simulation advanced of 
	} while (simulation_time < t_stop);	
	std::cout << "Number of spikes: " << nb_spikes_ << std::endl;
}


Neuron::~Neuron ()
{}
