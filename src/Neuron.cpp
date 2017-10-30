#include "Neuron.hpp"
#include <iostream>


Neuron::Neuron (double V_membrane, unsigned int nb_spikes,
				double t_spike,bool refractory, 
				bool spike, int neuron_clock, 
				double external_input,
				bool excitatory_neuron)
: V_membrane_ (V_membrane), nb_spikes_(nb_spikes), 
  t_spike_(t_spike), r_period_(refractory), 
  spike_(spike), neuron_clock_ (neuron_clock), 
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

bool Neuron::getRefractory() const
{
	return r_period_;
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
	return nb_excitatory_connections;
}

int Neuron::getInhibitoryConnections() const
{
	return nb_inhibitory_connections;
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
	nb_excitatory_connections = nb;
}

void Neuron::setInhibitoryConnections(int nb)
{
	nb_inhibitory_connections = nb;
}

size_t Neuron::numberOfTargets() const
{
	return n_target_.size();
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
	//std::cout << "The spike occurred at: " << t_spike_ << std::endl;
	r_period_ = true; // the neuron enters in its refractory period
	++nb_spikes_; //the number of spike increases of one
	spike_ = true; //the spike has occured
}

void Neuron::update(int dt)
{
	double input (0.0);

	spike_ = false; //by default, we don't have any spike
	double random_spikes(0);
	
	if (neuron_clock_ >= n_clock_start and neuron_clock_ <= n_clock_stop){	//outside the clock of the neuron, there is no input. When the clock of the neuron start, the received input changes value.
		input = external_input_;
		random_spikes = J_e*randomSpikes(); //generate random external spikes from the rest of the brain
	} else {
		input = 0.0;
	}
	if (V_membrane_ > V_thr){ // if the membrane potential crosses the threshold
		updateNeuronState(neuron_clock_);
	} 
	if (spike_ and not n_target_.empty()) { //if there is a spike and the current neurons has some targets
		for (auto n: n_target_){
			assert(n != nullptr); //control that the case of the target vector aren't nullptr
			if (excitatory_neuron_){
				n->addTimeBuffer((neuron_clock_+D)%(D+1), J_e);// the time buffer is fulled with a certain J from an excitatory neuron at a certain time step delay
			} else {
				n->addTimeBuffer((neuron_clock_+D)%(D+1), -J_i);// the time buffer is fulled with a certain J from an inhibitory neuron at a certain time step delay
			}
		}
	}
	if (r_period_ and tau_rp > neuron_clock_ - t_spike_/h){ //if the neuron is in its refractory state and it's still in its refractory period
		V_membrane_ = V_refractory; //the membrane potential is zero
	} else {
		if (getTimeBuffer((neuron_clock_)%(D+1)) != 0.0){ // if there is an impulse in the ring buffer at this time step
			receive(neuron_clock_, getTimeBuffer((neuron_clock_)%(D+1)) + random_spikes); // it means a neuron has spiked and the current neuron receives the spike after a certain time step delay
		} else {
			V_membrane_ = const1*V_membrane_ + input*const2 + random_spikes; //solve the membrane equation
			r_period_ = false; //the neuron is no more in its refractory period
		}
	}

	neuron_clock_ += dt;	//the simulation advanced of a step dt
}

void Neuron::receive(int time, double ampl)
{
	V_membrane_ = const1*V_membrane_ + external_input_*const2 + ampl; //the membrane potential is calculated based on the new input from neuron 1
	setTimeBuffer(time%(D+1), 0.0); //the used case of the time buffer is reinitialized to 0 to allow further insertions
}

void Neuron::addConnections(std::array<Neuron*, 12500>  neurons)
{
	std::random_device rd; //algorithme for generating random numbers
	std::mt19937 gen(rd()); //algorithme for generating random numbers
	std::uniform_int_distribution<> dis_e (0, excitatory_neurons); //distribution of random integer number
	
	for (int i(0); i<c_e; ++i){ //for every excitatory connections that a neuron can receive, add this neuron to the targets of a random chosen neuron in the pool of neurons
		neurons[dis_e(gen)]->addTargetNeuron(this); //the neuron randomly chosen add the current neuron as target
		++nb_excitatory_connections; //take the count of the excitatory connections received by a neuron
	}
	
	std::uniform_int_distribution<> dis_i (0, inhibitory_neurons); //distribution of random integer number
	for (int i(0); i<c_i; ++i){//for every inhibitory connections that a neuron can receive, add this neuron to the targets of a random chosen neuron in the pool of neurons
		neurons[9999+dis_i(gen)]->addTargetNeuron(this);//the neuron randomly chosen add the current neuron as target
		++ nb_inhibitory_connections;//take the count of the inhibitory connections received by a neuron
	}
}

int Neuron::randomSpikes()
{
	static std::random_device rd; //algorithme for generating random numbers
	static std::mt19937 gen(rd()); //algorithme for generating random numbers
	static std::poisson_distribution<> dis_ext (c_e*v_ext*h); //rate at which the target of the external connections receive spikes
	return dis_ext(gen);
}
			
Neuron::~Neuron()
{}
