# Neuron-Project

Create the class Neuron on Neuron.hpp with attributes for the membrane potential, the number of spikes and the time the spikes occured.
Create a Neuron.cpp. Add the setters and getters of the previous attributes.
Implement the function update in main.cpp. The program is not complete and doesn't work.
Another conception of the update function, that has been moved in Neuron.cpp. It compiles.
In main.cpp, a file Datas.txt opens and stores the membrane potential at the end of the simulation. Better to store all the membrane potential during the simulation (have to change it). The main.cpp calls the update function. The program works: the simulation has 5 different spikes with an external input of 1.01. 
New conception: changed all the time (that were double) into int: they've been transformed in steps of the simulation to make comparison better and more precise. The program works and the number of spikes is always 5 with the same external input. 
Change in the main.cpp. All the code relatif to the Datas.txt has been moved in update in Neuron.cpp, so that the membrane potential is stored at each steps of the simulation. The program works, but the number of spikes doesn't match with the solution.
Add a neuron clock for the start and the end (as time constants) of the time interval where the neuron recevies an external input. Add a function that update the state of the neuron in case of spike (better modularisation). The program works, and the number of spikes and the time the spikes occured now match.
Add an attribute (bool spike_) that tells if a spike occured during a time step or not. Nothing changed. 
All the files made so far have been deplaced in a new branch: that was the one neuron program.

The prpgram have to evolve: it has to work for two neurons and one connection.
