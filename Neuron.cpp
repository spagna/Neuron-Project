#include "Neuron.hpp"
#include <iostream>


Neuron::Neuron (double V_membrane, unsigned int nb_spikes,
				double t_spike, double t_received_spike,
				bool refractory, bool spike, bool received, 
				int neuron_clock, double external_input,
				bool updated, bool n_post_synaptic)
: V_membrane_ (V_membrane), nb_spikes_(nb_spikes), 
  t_spike_(t_spike), t_received_spike_(t_received_spike),
  r_period_(refractory), spike_(spike), received_(received),
  neuron_clock_ (neuron_clock), external_input_(external_input), 
  updated_(updated), n_post_synaptic_ (n_post_synaptic)
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

double Neuron::getTimeReceivedSpike() const
{
	return t_received_spike_;
}

bool Neuron::getRefractory() const
{
	return r_period_;
}

bool Neuron::getSpikeState() const
{
	return spike_;
}

bool Neuron::getReceivedState() const
{
	return received_;
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

bool Neuron::getUpdateState() const
{
	return updated_;
}

Neuron* Neuron::getTargetNeuron (int i) const
{
	return n_target_[i];
}

bool Neuron::getPostSynpatic() const
{
	return n_post_synaptic_;
}

bool Neuron::getExcitatoryNeuron() const
{
	return excitatory_neuron_;
}

bool Neuron::getInhibitoryNeuron() const
{
	return inhibitory_neuron_;
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

void Neuron::setTimeReceivedSpike (double t)
{
	t_received_spike_ = t;
}

void Neuron::setRefractory (bool refractory)
{
	r_period_ = refractory;
}

void Neuron::setSpikeState (bool spike)
{
	spike_ = spike;
}

void Neuron::setReceivedState (bool received)
{
	received_ = received;
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

void Neuron::setPostSynaptic(bool p)
{
	n_post_synaptic_ = p;
}

void Neuron::setExcitatoryNeuron (bool exc)
{
	excitatory_neuron_ = exc;
}

void Neuron::setInhibitoryNeuron (bool inhib)
{
	inhibitory_neuron_ = inhib;
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
	std::cout << "The spike occurred at: " << t_spike_ << std::endl;
	r_period_ = true; // the neuron enters in its refractory period
	++nb_spikes_; //the number of spike increases of one
	spike_ = true; //the spike has occured
}

void Neuron::updatePostSynapticNeuron(int dt)
{
	for (auto n : n_target_){ //for every post-synpatic neuron target by the present neuron
		if (n != nullptr and getTimeBuffer(dt) != 0.0){ //check if the pointer target is not nullptr and if there is an impulse in the ring buffer at this time step
			n->setPostSynaptic(true);
			n->receive(getTimeBuffer(dt)); //the post-synaptic neuron receives the signal and update, but doesn't spike
			setTimeBuffer(dt, 0.0); //the used case of the time buffer is reinitialized
		}
	}
}
	
void Neuron::update(int dt)
{
	double input (0.0);

	spike_ = false; //by default, we don't have any spike
	received_ = false; //false if no signal is received
	updated_ = false; //false until update finish
	
	if (neuron_clock_ >= n_clock_start and neuron_clock_ <= n_clock_stop){	//outside the clock of the neuron, there is no input. When the clock of the neuron start, the received input changes value.
		input = external_input_;
	} else {
		input = 0.0;
	}
	if (V_membrane_ > V_thr){ // if the membrane potential crosses the threshold
		updateNeuronState(neuron_clock_);
	} 
	if (spike_) { //if there is a spike, the time buffer is fulled with a certain J at a certain time step delay
		t_received_spike_ = t_spike_ + D*h;
		addTimeBuffer((neuron_clock_+D)%(D+1), J);
	}
	if (not n_target_.empty()){ //if there is a target, we check and update the state of the post-synpatic neuron
		updatePostSynapticNeuron((neuron_clock_)%(D+1));
	}	
	if (r_period_ and tau_rp > neuron_clock_ - t_spike_/h){ //if the neuron is in its refractory state and it's still in its refractory period
		V_membrane_ = V_refractory; //the membrane potential is zero
	} else {
		V_membrane_ = const1*V_membrane_ + input*const2; //solve the membrane equation
		r_period_ = false; //the neuron is no more in its refractory period
	}
	neuron_clock_ += dt;	//the simulation advanced of a step dt
	updated_ = true; //true when the update has finished
}

void Neuron::receive(double ampl)
{
	received_ = true;
	t_received_spike_ = neuron_clock_*h;
	V_membrane_ = const1*V_membrane_ + external_input_*const2 + ampl; //the membrane potential is calculated based on the new input from neuron 1
	std::cout << "The spike is received at time " << t_received_spike_ << " and the membrane potential is " << V_membrane_ << std::endl;
}
	
Neuron::~Neuron()
{}
