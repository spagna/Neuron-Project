#include <iostream>
#include "Neuron.hpp"
#include "gtest/gtest.h"
#include <cmath>
#include <cassert>

//TEST1: Test if the current membrane potential coincide with the equation if the external
//////// input during the time interval [a,b] is 1.0 (so no spikes)

TEST (NeuronTest, PositiveMembranePotential){
	Neuron neuron(true);
	neuron.setExternalInput(1.0); //positif input given
	neuron.update(1, 0.0); //one update without noises
	//check if the membrane potential increases in positif according to the membrane equation
	EXPECT_NEAR(20.0*(1.0-exp(-0.1/20.0)), neuron.getV_membrane(), 0.001);
	
}

//TEST2: Test if the current membrane potential coincide with the equation if the external
//////// input during the time interval [a.b] is -1.0 (so no spikes)
TEST (NeuronTest, NegativeMembranePotential){
	Neuron neuron(true);
	neuron.setExternalInput(-1.0); //negatif input given
	neuron.update(1, 0.0); //one update without noises
	//check if the membrane potential decreases in negatif according to the membrane equation
	EXPECT_NEAR(-20.0*(1.0-exp(-0.1/20.0)), neuron.getV_membrane(), 0.001); 
}

//TEST3: Test if the current membrane potential coincide with the equation if the extenral 
/////// input is 0.0 in the time interval [a,b] too
TEST (NeuronTest, NulMembranePotential){
	Neuron neuron (true);
	neuron.setExternalInput (0.0); //no input is received by the neuron
	neuron.update(1, 0.0); //no noise received
	EXPECT_NEAR(0.0, neuron.getV_membrane(), 0.001); //check if there is no changes in the membrane potential
}



//TEST4: Test if the time of the first spike (whick is 92.4 for an external input of 1.01)
/////// is the same as we expect
TEST (NeuronTest, SpikeTimes){
	Neuron neuron (true); 
	neuron.setExternalInput(1.01); //the external input is setted to 1.01 so that the neuron will cross the threshold
	//neuron is update until one step before it spikes
	do {
		neuron.update(1, 0.0);
	} while (neuron.getNeuronClock() < 924);
	EXPECT_NEAR (neuron.getV_membrane(), 20.0, 0.001); //check if neuron is ready to spike
	neuron.update(1, 0.0); //neuron spikes
	 //check if the values of the time of the last spikes is the same as the values written in the 
	 //terminal during the simulation
	EXPECT_EQ (92.4, neuron.getTimeSpike());
	EXPECT_NEAR (neuron.getV_membrane(), 0.0, 0.001); //check if neuron enters its refractory period
}

//TEST4:Test if the post-synaptic neuron receives the spike with the correct delay
TEST (NeuronTest, Delay){
	Neuron neuron1(true); //the spiking neuron is an excitatory neuron: the amplitude of its signal is 0.1
	Neuron neuron2(true);
	neuron1.addTargetNeuron(&neuron2); //neuron2 is added as a target of neuron1
	neuron1.setExternalInput(1.01); //neuron1 is the only one receiving an external input
	
	//the neurons are updated unitl one step before neuron1 spikes
	do {
		neuron1.update(1, 0.0); 
		neuron2.update(1, 0.0); 	
	} while (neuron1.getNeuronClock() < 924);
	EXPECT_NEAR (neuron1.getV_membrane(), 20.0, 0.001); //chekc if neuron1 is ready to spike
	EXPECT_NEAR (neuron2.getV_membrane(), 0.0, 0.001); //check if neuron2 doesn't receive any stimuli
	neuron1.update(1, 0.0); //neuron1 spikes
	neuron2.update(1, 0.0);
	EXPECT_NEAR (neuron1.getV_membrane(), 0.0, 0.001); //check if neuron1 goes in its refractory period
	EXPECT_NEAR (neuron2.getV_membrane(), 0.0, 0.001); //check if neuron2 doesn't spike
	//neuron2 is update until the delay ends
	for (int i(0); i<=D; ++i){ 
		neuron2.update(1, 0.0); //after D time steps neuron2 should receives the spike
		}
	EXPECT_NEAR (neuron2.getV_membrane(), 0.1, 0.001); //check if neuron2 receives the spike
}




	

	
