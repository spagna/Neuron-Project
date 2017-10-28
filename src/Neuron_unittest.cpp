#include <iostream>
#include "Neuron.hpp"
#include "gtest/gtest.h"
#include <cmath>
#include <cassert>

//TEST1: Test if the current membrane potential coincide with the equation if the external
//////// input during the time interval [a,b] is 1.0 (so no spikes)

TEST (NeuronTest, PositiveMembranePotential){
	Neuron neuron;
	neuron.setExternalInput(1.0); 
	neuron.setNeuronClock(n_clock_start);
	neuron.update(1);
	
	EXPECT_EQ(20.0*(1.0-exp(-0.1/20.0)), neuron.getV_membrane());
	
}

//TEST2: Test if the current membrane potential coincide with the equation if the external
//////// input during the time interval [a.b] is -1.0 (so no spikes)
TEST (NeuronTest, NegativeMembranePotential){
	Neuron neuron;
	neuron.setExternalInput(-1.0);
	neuron.setNeuronClock(n_clock_start);
	neuron.update(1);
	
	EXPECT_EQ(-20.0*(1.0-exp(-0.1/20.0)), neuron.getV_membrane());
}

//TEST3: Test if the current membrane potential coincide with the equation if the extenral 
/////// input is 0.0 in the time interval [a,b] too
TEST (NeuronTest, NulMembranePotential){
	Neuron neuron;
	neuron.setExternalInput (0.0);
	neuron.setNeuronClock(n_clock_start);
	neuron.update(1);
	
	EXPECT_EQ(0.0, neuron.getV_membrane());
}



//TEST4: Test if the current membrane potential is 0 outside the time interval
/////// [a.b] even if an external input is applied. The neuron doesn't receive the 
/////// input in this time interval. The test A checks before the interval.

TEST (NeuronTest, OutsideExternalInputA){
	Neuron neuron;
	neuron.setExternalInput(1.01);
	neuron.update(1);
	
	EXPECT_EQ(0.0, neuron.getV_membrane());
}

//TEST5: Test if the current membrane potential is 0 outside the time interval
/////// [a.b] even if an external input is applied. The neuron doesn't receive the 
/////// input in this time interval. The test B checks after the interval.
TEST (NeuronTest, OutsideExternalInputB){
	Neuron neuron;
	neuron.setExternalInput(1.01);
	neuron.setNeuronClock(n_clock_stop+1);
	neuron.update(1);
	
	EXPECT_EQ(0.0, neuron.getV_membrane());
}

//TEST6: Test if the time of the first spike (whick is 192.4 for an external input of 1.01)
/////// is the same as we expect
TEST (NeuronTest, SpikeTimes){
	Neuron neuron; 
	neuron.setExternalInput(1.01);
	neuron.setNeuronClock(n_clock_start);
	do {
		neuron.update(1);
	} while (neuron.getNeuronClock() < 1924);
	EXPECT_NEAR (neuron.getV_membrane(), 20.0, 0.001);
	neuron.update(1);
	EXPECT_EQ (192.4, neuron.getTimeSpike());
	EXPECT_NEAR (neuron.getV_membrane(), 0.0, 0.001);
}

TEST (NeuronTest, Delay){
	Neuron neuron1;
	Neuron neuron2;
	neuron1.addTargetNeuron(&neuron2);
	neuron1.setExternalInput(1.01);
	neuron1.setNeuronClock(n_clock_start);
	neuron2.setExternalInput(0.0);
	neuron2.setNeuronClock(n_clock_start);
	do {
		neuron1.update(1);
		neuron2.update(1);
	} while (neuron1.getNeuronClock() < 1924);
	EXPECT_NEAR (neuron1.getV_membrane(), 20.0, 0.001);
	EXPECT_NEAR (neuron2.getV_membrane(), 0.0, 0.001);
	neuron1.update(1);
	neuron2.update(1);
	EXPECT_NEAR (neuron1.getV_membrane(), 0.0, 0.001);
	EXPECT_NEAR (neuron2.getV_membrane(), 0.0, 0.001);

	for (int i(0); i<=D; ++i){
		neuron1.update(1);
		neuron2.update(1);
		EXPECT_NEAR(neuron1.getV_membrane(), 0.0, 0.001);
		}
	EXPECT_NEAR (neuron2.getV_membrane(), 0.1, 0.001);
	neuron1.update(1);
	//EXPECT_EQ (neuron2.getV_membrane(), 0.1-const1*neuron2.getV_membrane());
	
}
			
	 




	

	