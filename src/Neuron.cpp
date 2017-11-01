#include "Neuron.hpp"
#include <iostream>


Neuron::Neuron (double V_membrane, unsigned int nb_spikes,
				double t_spike, 
				bool spike, int neuron_clock, 
				double external_input,
				bool excitatory_neuron)
: V_membrane_ (V_membrane),
  nb_spikes_(nb_spikes), 
  t_spike_(t_spike), spike_(spike), 
  neuron_clock_ (neuron_clock), 
  external_input_(external_input), 
  excitatory_neuron_ (excitatory_neuron)
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

bool Neuron::getSpikeState() const
{
	return spike_;
}

int Neuron::getNeuronClock() const
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

Neuron* Neuron::getTargetNeuron(int i) const
{
	return n_target_[i];
}

bool Neuron::getExcitatoryNeuron() const
{
	return excitatory_neuron_;
}

int Neuron::getExcitatoryConnections() const
{
	return nb_excitatory_connections_;
}

int Neuron::getInhibitoryConnections() const
{
	return nb_inhibitory_connections_;
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

void Neuron::setSpikeState (bool spike)
{
	spike_ = spike;
}

void Neuron::setNeuronClock (int clock)
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

void Neuron::setTargetNeuron(int i, Neuron* n)
{
	n_target_[i] = n;
}

void Neuron::setExcitatoryNeuron (bool n)
{
	excitatory_neuron_ = n;
}

void Neuron::setExcitatoryConnections(int nb)
{
	nb_excitatory_connections_ = nb;
}

void Neuron::setInhibitoryConnections(int nb)
{
	nb_inhibitory_connections_ = nb;
}

void Neuron::addTimeBuffer (int i, double val)
{
	t_buffer_[i] += val;
}

void Neuron::addTargetNeuron(Neuron* n)
{
	n_target_.push_back(n);
}


void Neuron::updateNeuronState (int dt)
{
	t_spike_ = dt*h;//store the time of the spike 
	++nb_spikes_; //the number of spike increases of one
	spike_ = true; //the spike has occured
}

void Neuron::updateTargets()
{
	for (auto n: n_target_){
		assert(n != nullptr); //control that the case of the target vector aren't nullptr
		if (excitatory_neuron_){
			n->addTimeBuffer((neuron_clock_+D)%(D+1), J_e);// the time buffer is fulled with a certain J from an excitatory neuron at a certain time step delay
		} else {
			n->addTimeBuffer((neuron_clock_+D)%(D+1), -J_i);// the time buffer is fulled with a certain J from an inhibitory neuron at a certain time step delay
		}
	}
}

void Neuron::update(int dt)
{	
	spike_ = false; //by default, we don't have any spike
	double sum_amplitudes(0.0); //contains all the amplitude of the arriving spike of excitatory and inhibitory neurons and from the random spiking external neurons
	sum_amplitudes = getTimeBuffer((neuron_clock_)%(D+1)) + randomSpikes();

	
	if (V_membrane_ > V_thr){ // if the membrane potential crosses the threshold
		updateNeuronState(neuron_clock_);
		updateTargets();
	} 
	if (tau_rp > neuron_clock_ - t_spike_/h){ //if the neuron is in its refractory state and it's still in its refractory period
		V_membrane_ = V_refractory; //the membrane potential is zero
	} else {
		V_membrane_= const1*V_membrane_ + external_input_*const2 + sum_amplitudes;
	}
	
	setTimeBuffer(neuron_clock_%(D+1), 0.0);
	neuron_clock_ += dt;	//the simulation advanced of a step dt
}

void Neuron::addConnections(std::array<Neuron*, 12500>  neurons)
{
	std::random_device rd; //algorithme for generating random numbers
	std::mt19937 gen(rd()); //algorithme for generating random numbers
	std::uniform_int_distribution<> dis_e (0, excitatory_neurons-1); //distribution of random integer number
	
	for (int i(0); i<c_e; ++i){ //for every excitatory connections that a neuron can receive, add this neuron to the targets of a random chosen neuron in the pool of neurons
		neurons[dis_e(gen)]->addTargetNeuron(this); //the neuron randomly chosen add the current neuron as target
		++nb_excitatory_connections_; //take the count of the excitatory connections received by a neuron
	}
	
	std::uniform_int_distribution<> dis_i (0, inhibitory_neurons-1); //distribution of random integer number
	for (int i(0); i<c_i; ++i){//for every inhibitory connections that a neuron can receive, add this neuron to the targets of a random chosen neuron in the pool of neurons
		neurons[excitatory_neurons+dis_i(gen)]->addTargetNeuron(this);//the neuron randomly chosen add the current neuron as target
		++ nb_inhibitory_connections_;//take the count of the inhibitory connections received by a neuron
	}
}

double Neuron::randomSpikes()
{
	static std::random_device rd; //algorithme for generating random numbers
	static std::mt19937 gen(rd()); //algorithme for generating random numbers
	static std::poisson_distribution<> dis_ext (c_e*v_ext*h); //rate at which the target of the external connections receive spikes
	return J_e*dis_ext(gen);
}
			
Neuron::~Neuron()
{}
