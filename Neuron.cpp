#include "Neuron.hpp"
#include <iostream>
#include <cmath>

Neuron::Neuron (double V_membrane, unsigned int nb_spikes, double t_spike, bool refractory, bool spike)
: V_membrane_ (V_membrane), nb_spikes_(nb_spikes), 
  t_spike_(t_spike), r_period_(refractory), spike_(spike)
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

bool Neuron::getSpikeState() const
{
	return spike_;
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

void Neuron::setSpikeState (bool spike)
{
	spike_ = spike;
}

void Neuron::updateNeuronState (int dt)
{
	t_spike_ = dt*h;//store the time of the spike 
	std::cout << "The spike occurred at: " << t_spike_ << std::endl;
	r_period_ = true; // the neuron enters in its refractory period
	++nb_spikes_; //the number of spike increases of one
	spike_ = true;
}


void Neuron::update(int dt)
{
	std::ofstream file; //open a file to store the values of the membrane potentials
	file.open("Datas.txt");
	if (file.fail()){ //check if the file opens: if not, there is an error message on the terminal
		std::cerr << "Error: file doesn't open" << std::endl;
	}
	
	int simulation_time = t_start; //simulation_time starts at step 0 (0.0 in milliseconds) 
	double R = (tau*h)/C; //membrane resistance
	double input (0.0);
	double external_input (0.0);
	double const1 =  exp(-h/(tau*h));
	double const2 = R*(1 - exp(-h/(tau*h)));
	
	
	std::cout << "Choose a value for the external input" << std::endl; // rem: the value should be >=0
	std::cin >> external_input;
	
	
	do {
		spike_ = false;
		file << "Membrane potential at " << simulation_time*h << " milliseconds: " << V_membrane_ << std::endl; //the membrane potential is stored in Datas.txt
		if (simulation_time >= n_clock_start and simulation_time <= n_clock_stop){	//outside the clock of the neuron, there is no input. When the clock of the neuron start, the received input changes value.
			input = external_input;
		} else {
			input = 0.0;
		}
		if (V_membrane_ > V_thr){ // if the membrane potential crosses the threshold
			updateNeuronState(simulation_time);
		} 
		if (r_period_ and tau_rp > simulation_time - t_spike_/h){ //if the neuron is in its refractory state and it's still in the refractory period
			V_membrane_ = V_refractory; //the membrane potential is zero
		} else {
			V_membrane_ = const1*V_membrane_ + input*const2; //solve the membrane equation
			r_period_ = false; //the neuron is no more in its refractory period
		}
		simulation_time += dt;	//the simulation advanced of a step dt
		/*if (spike_) {
			std::cout << "SPIKE OCCURED" << std::endl;
		}*/
	} while (simulation_time <= t_stop);	
	std::cout << "Number of spikes: " << nb_spikes_ << std::endl;
}



Neuron::~Neuron ()
{}
