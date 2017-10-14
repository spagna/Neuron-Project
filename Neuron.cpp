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

double Neuron::getTimeSpike() const
{
	return t_spike_;
}

bool Neuron::getRefractory() const
{
	return r_period_;
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

void Neuron::setRefractory (bool refractory)
{
	r_period_ = refractory;
}

	

void Neuron::update(int dt)
{
	std::ofstream file; //open a file to store the values of the membrane potential
	file.open("Datas.txt");
	if (file.fail()){ //check if the file opens: if not, there is an error message on the terminal
		std::cerr << "Error: file doesn't open" << std::endl;
	}
	int simulation_time (0); //simulation_time (0.0 in milliseconds) * h
	double R = (tau*h)/C; //membrane resistance
	double external_input (0.0);
	double const1 =  exp(-h/(tau*h));
	std::cout << "Choose a value for the external input" << std::endl; // rem: the value should be >=0
	std::cin >> external_input;
	simulation_time = t_start;
	do {
		file << "Membrane potential at " << simulation_time*h << " milliseconds: " << V_membrane_ << std::endl; //the membrane potential is stored in Datas.txt
		if (V_membrane_ > V_thr){ // if the membrane potential crosses the threshold
			t_spike_ = simulation_time*h;//store the time of the spike 
			std::cout << "The spike occurred at: " << t_spike_ << std::endl;
			r_period_ = true; // the neuron enters in its refractory period
			++nb_spikes_; //the number of spike increases of one
		} 
		if (r_period_ and tau_rp >= simulation_time - t_spike_/h){ //if the neuron is in its refractory state and it's still in the refractory period
			V_membrane_ = V_refractory; //the membrane potential is zero
		} else {
			V_membrane_ = const1*V_membrane_ + external_input*R*(1-const1); //solve the membrane equation
			r_period_ = false; //the neuron is no more in its refractory period
		}
		simulation_time += dt;	//the simulation advanced of a step dt
	} while (simulation_time <= t_stop);	
	std::cout << "Number of spikes: " << nb_spikes_ << std::endl;
}


Neuron::~Neuron ()
{}
