#include "Neuron.hpp"
#include <iostream>
#include <vector>

Neuron::Neuron (double membranePotential, int spikes, std::vector<double> time)
{} 

double Neuron::getMembranePotential() const
{
	return membranePotential_;
}

int Neuron::getNumberSpikes() const
{
	return spikes_;
}

double Neuron::getTimeSpike(size_t i)
{
	return t_[i];
}

void Neuron::setMembranePotential(double membranePotential)
{
	membranePotential_ = membranePotential;
	
}

void Neuron::setNumberSpikes(int spikes)
{
	spikes_ = spikes;
}

void Neuron::setTimeSpike (size_t i, double t)
{
	t_[i] = t;
}

void Neuron::update()
{
	//membranePotential_ = exp(-h
}

void Neuron::addSpikeTime(double t)
{
	t_.push_back(t);
}

bool Neuron::emptyVector() const
{
	return t_.empty();
}

Neuron::~Neuron ()
{}



