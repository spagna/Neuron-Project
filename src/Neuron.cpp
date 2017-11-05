#include "Neuron.hpp"
#include <iostream>
#include <random>


Neuron::Neuron (bool excitatory_neuron, 
				double V_membrane, unsigned int nb_spikes,
				double t_spike, 
				bool spike, int neuron_clock, 
				double external_input,
				bool r_period)
				
: excitatory_neuron_(excitatory_neuron),
  V_membrane_ (V_membrane),
  nb_spikes_(nb_spikes), 
  t_spike_(t_spike), spike_(spike), 
  neuron_clock_ (neuron_clock), 
  external_input_(external_input),
  r_period_(r_period)
{
	//each box of the buffer is setted to zero at the beginnin
	for (size_t i(0); i<t_buffer_.size(); ++i){ //each box of the buffer is setted to zero at the beginnin
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

bool Neuron::getRefractoryState() const
{
	return r_period_;
}

double Neuron::getTimeBuffer (int i) const
{
	return t_buffer_[i];
}

Neuron* Neuron::getTargetNeuron(int i) const
{
	return n_target_[i];
}

std::vector<Neuron*> Neuron::getTargets() const
{
	return n_target_;
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

void Neuron::setRefractoryState(bool r)
{
	r_period_ = r;
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
	r_period_= true; //the refractory period becomes as soon as a spike occurs
}

void Neuron::updateTargets()
{
	for (auto const& n: n_target_){
		assert(n != nullptr); //control that the targents aren't nullptr
		if (excitatory_neuron_){
			// the time buffer is fulled with a certain J from 
			// an excitatory neuron at a certain time step delay
			n->addTimeBuffer((neuron_clock_+D)%(D+1), J_e);
		} else {
			// the time buffer is fulled with a certain J from 
			// an inhibitory neuron at a certain time step delay
			n->addTimeBuffer((neuron_clock_+D)%(D+1), -J_i);
		}
	}
}

void Neuron::update(int dt, double noise)
{	
	//the value has to be setted at false at the beginning of every update until a spike occurs
	spike_ = false; 
	
	if (V_membrane_ > V_thr){ // if the membrane potential crosses the threshold
		updateNeuronState(neuron_clock_); //some parameters of the neuron have to be updated
		updateTargets(); //the targets have to be updated 
	} 
	//the neuron is in its refractory period if the distance in time between the last spike
	// and the current neuron is less than tau, a constant
	if (r_period_ and tau_rp > neuron_clock_ - t_spike_/h){ 
		V_membrane_ = V_refractory; //the membrane potential is zero
	//if the neuron doesn't spike nor stay in a refractory state, the membrane potential has to be updated
	} else {
		r_period_ = false; //the neuron isn't in its refractory period
		//the membrane potential increases and that depends on three parameters:
		//the external input, the amplitude stored in the time buffer and the random spikes
		//and their relative amplitudes coming from the exterior
		solveMembraneEquation(external_input_, getTimeBuffer((neuron_clock_)%(D+1)), noise); 
	}
	//the box of the current time in the ring buffer has to be resetted to 0 after each update
	// to allow the next spikes to be stored
	setTimeBuffer(neuron_clock_%(D+1), 0.0); 
	neuron_clock_ += dt;	//the simulation advanced of a step dt
}

void Neuron::solveMembraneEquation(double input, double ampl, double random)
{
	//equation of the membrane potential
	V_membrane_ = const1*V_membrane_ + const2*input + ampl + random;
}

void Neuron::addConnections(std::array<Neuron*, total_neurons>  const& neurons)
{
	std::random_device rd; //algorithme for generating random numbers
	std::mt19937 gen(rd()); //algorithme for generating random numbers
	
	//distribution of random integer number to determine which excitatory neuron 
	//is connected to the current neuron. The values vary between 0 and 999.
	//Those values correponds to the index of excitatory neurons in the array of neurons
	std::uniform_int_distribution<> dis_e (0, excitatory_neurons-1); 
	
	
	for (int i(0); i<c_e; ++i){ //for every excitatory connections that a neuron can receive
		//the neuron randomly chosen add the current neuron as target
		neurons[dis_e(gen)]->addTargetNeuron(this); 
		++nb_excitatory_connections_; //take the count of the excitatory connections received by a neuron
	}
	
	//distribution of random integer number to determine which inhibitory neuron 
	//is connected to the current neuron. The values vary between 0 and 249.
	//Those values correponds to the index of excitatory neurons in the array of neurons
	std::uniform_int_distribution<> dis_i (0, inhibitory_neurons-1);
	for (int i(0); i<c_i; ++i){//for every inhibitory connections that a neuron can receive,
		//the neuron randomly chosen add the current neuron as target
		neurons[excitatory_neurons+dis_i(gen)]->addTargetNeuron(this);
		++ nb_inhibitory_connections_;//take the count of the inhibitory connections received by a neuron
	}
}

double Neuron::randomSpikes() const
{
	static std::random_device rd; //algorithme for generating random numbers
	static std::mt19937 gen(rd()); //algorithme for generating random numbers
	static std::poisson_distribution<> dis_ext (poisson_gen); //rate at which the target of the external connections receive spikes
	return J_e*dis_ext(gen); //amplitudes of the random generated spikes
}
			
Neuron::~Neuron()
{}
