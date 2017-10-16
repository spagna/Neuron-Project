#include "Neuron.hpp"
#include <iostream>
#include <cmath>

Neuron::Neuron (double V_membrane, unsigned int nb_spikes,
				double t_spike, bool refractory, bool spike,
				double neuron_clock, double external_input)
: V_membrane_ (V_membrane), nb_spikes_(nb_spikes), 
  t_spike_(t_spike), r_period_(refractory), spike_(spike), 
  neuron_clock_ (neuron_clock), external_input_(external_input)
{
	for (size_t i(0); i<t_buffer_.size(); ++i){
		t_buffer_[i] = 0.0;
	}
} 

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

double Neuron::getNeuronClock() const
{
	return neuron_clock_;
}

double Neuron::getExternalInput() const
{
	return external_input_;
}

double Neuron::getTimeBuffer (int i) const
{
	return t_buffer_[i];
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

void Neuron::setNeuronClock (double clock)
{
	neuron_clock_ = clock;
}

void Neuron::setExternalInput (double external_input)
{
	external_input_ = external_input;
}

void Neuron::setTimeBuffer (int i, double val)
{
	t_buffer_[i] = val;
}

void Neuron::addTimeBuffer (int i, double val)
{
	t_buffer_[i] += val;
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
	double R = (tau*h)/C; //membrane resistance
	double input (0.0);
	double const1 =  exp(-h/(tau*h));
	double const2 = R*(1 - exp(-h/(tau*h)));


	spike_ = false;
	file << "Membrane potential at " << neuron_clock_*h << " milliseconds: " << V_membrane_ << std::endl; //the membrane potential is stored in Datas.txt
	if (neuron_clock_ >= n_clock_start and neuron_clock_ <= n_clock_stop){	//outside the clock of the neuron, there is no input. When the clock of the neuron start, the received input changes value.
		input = external_input_;
	} else {
		input = 0.0;
	}
	if (V_membrane_ > V_thr){ // if the membrane potential crosses the threshold
		updateNeuronState(neuron_clock_);
	} 
	if (r_period_ and tau_rp > neuron_clock_ - t_spike_/h){ //if the neuron is in its refractory state and it's still in the refractory period
		V_membrane_ = V_refractory; //the membrane potential is zero
	} else {
		V_membrane_ = const1*V_membrane_ + input*const2; //solve the membrane equation
		r_period_ = false; //the neuron is no more in its refractory period
	}
	neuron_clock_ += dt;	//the simulation advanced of a step dt
}

void Neuron::receive(int time, double ampl)
{
	double R = (tau*h)/C; //membrane resistance
	double const1 =  exp(-h/(tau*h));
	double const2 = R*(1 - exp(-h/(tau*h)));
	if (neuron_clock_ != time) {
		ampl = 0.0;
		V_membrane_ = const1*V_membrane_ + external_input_*const2 + ampl; //the membrane potential is calculated based on the new input from neuron 1
	} else {
		V_membrane_ = const1*V_membrane_ + external_input_*const2 + ampl; //the membrane potential is calculated based on the new input from neuron 1
		std::cout << "The spike has been received at time " << time << " and the membrane potential is " << V_membrane_ << std::endl;
		t_buffer_[time] = 0.0;
	}
}
	



Neuron::~Neuron()
{}
