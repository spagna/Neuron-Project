#include "Neuron.hpp"
#include <iostream>
#include <cmath>
#include <vector>





int main()
{
	 double t_start (0.0); //in milliseconds
 double t_stop (10.0);
 double tau (20.0); //time constant in milliseconds (note tau = R/C)
 unsigned int C (1); //number of connections from other neurons
 double V_thr (20.0); //firing threshold in microvolts 
 double V_reset (10.0); //reset potential in microvolts
 double V_refractory (0.0);
 double tau_rp (2.0); //refractory period in milliseconds

	Neuron n;
	double simulation_time (0.0);
	double R = tau/C; //membrane resistance
	double input_current (0.0);
	double external_input (20.0);
	double a (2.0);//a and b are a time interval where the external input is different to zero
	double b (8.0);
	size_t i (0); //keep the count of the number of spikes
	double h (1.0); //equidistant step of time during which the simulation evolves
	
	
	
	/*std::cout << "Enter a value for the external input" << std::endl;
	std::cin >> external_input;
	std::cout << "Enter a time interval: the start value and the final value" << std::endl;
	std::cin >> a >> b;*/
	simulation_time = t_start;
	do {
		if (a<=simulation_time and b>simulation_time){
			input_current = external_input;
		} else {
			input_current = 0.0;
		}
		if (not n.emptyVector() and simulation_time <= n.getTimeSpike(i) and simulation_time + tau_rp >= n.getTimeSpike(i)){ //the neuron is refractory
			n.setMembranePotential(V_refractory);
			std::cout << "Oui" << std::endl;
		} else if (n.getMembranePotential() > V_thr){
			n.addSpikeTime(simulation_time);
			++i;
		} 
		double const1 = exp(-h/tau); //constant in the membrane equation
		n.setMembranePotential(const1*n.getMembranePotential() + input_current*R*(1-const1));
		std::cout << n.getMembranePotential() << std::endl;
		simulation_time += h;	
	} while (simulation_time < t_stop);
	//std::cout << n.getTimeSpike(0) << std::endl;
	
	
	return 0;
}
