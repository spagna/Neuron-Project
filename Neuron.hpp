#ifndef NEURON_H
#define NEURON_H

#include <iostream>
#include <vector>

class Neuron 
{
public:
	Neuron (double membranePotential = 0.0, int spikes = 0, std::vector<double> time = std::vector<double> (0, 0.0));
	
	double getMembranePotential() const;
	int getNumberSpikes() const;
	double getTimeSpike(size_t i); 
	
	void setMembranePotential(double membranePotential);
	void setNumberSpikes(int spikes);
	void setTimeSpike (size_t i, double t);
	void addSpikeTime(double t); //add a time to the vector
	bool emptyVector () const;
	
	void update();
	
	
	
	~Neuron();
	
	

private:
	double membranePotential_; 
	int spikes_; //number of spikes
	std::vector<double> t_; //times when the spikes occured 
	
};

#endif
