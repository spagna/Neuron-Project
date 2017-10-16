#include "Neuron.hpp"
#include <iostream>
#include <cmath>
#include <fstream>



int main()
{
	Neuron n1, n2; //n2 is the post-synaptic neuron	
	int simulation_time = t_start; //the global simulation starts at step t_start
	int t_pre (0); //stock the time step when a spike occured, at time of neuron n1
	double external_input (0.0);
	
	std::cout << "Choose a value for the external input" << std::endl; // rem: the value should be >=0
	std::cin >> external_input;
	n1.setExternalInput(external_input);
	n2.setExternalInput(0.0);
	
	do {
		n1.update(N);
		n2.update(N);
		if (n1.getSpikeState()){ //if there is a spike
			t_pre = n1.getTimeSpike();
			n2.addTimeBuffer((t_pre+D)%(D+1), J); // at the time step of the spike, J is added to the ring buffer at position t+D	
		}
		n2.receive (t_pre + D, n2.getTimeBuffer(t_pre%(D+1))); //n2 receives the signal and update, but doesn't spike
		simulation_time += N; //the simulation time advanced of a time step N
		
	} while (simulation_time < t_stop); //unitl it reaches the end of the global simulation
	std::cout << "Number of spikes: " << n1.getNumberSpikes() << std::endl;
	//n.update(N);	
	return 0;
}
