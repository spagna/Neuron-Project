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
In the main, a vector of pointer on Neuron has been added. For now, two neurons are on this vector.
The second neuron is the post-synpatic neuron. New attribute in class Neruon: a vector of pointer on Neuron, which will store all the target neurons for a given neuron. New funciton: addTargetNeuron, which will add a new target to the vector. In the main, neuron 2 is added as a target for neuron 1 with this function.
All the post-synaptic neuron will have a external input of 0.0, wheras all the other neurons will have an external input choosen by the user.
New loop in the main: in this loop, all neurons that are part of the vector Neurons will be updated. All the neuron's clocks have to been incremented of one time step before the simulation time increases, so the simulation time will be updated only at the end of the loop and only if all the neurons have been updated. Add an assert function to check that all neurons are updated before incrementing the simulation time. 
Add a new attribute: a time buffer array. The array will store the amplitude J recevied from a neuron that has spiked, and it stores that data with a certain a D. The size of the table is D+1, so that the amplitude will always be stored in the right case that take count of the delay. 
New functon addTimeBuffer, that adds the amplitude in the correct place in the table. If another amplitude is already presents, the two will sum up. 
Now, if the neuron spikes, the amplitude J has to been stored in the time buffer at place (neuron_clock_+D)%(D+1). The modulo allows to place the amplitude in the right case without having an "infinite" size. 
If a target is present, the post-synpatic neuron will updated. New function: updatePostSynpaticNeuron (at time step dt).At every time step, if the case of the time buffer is not empty (not equal to 0), the post-synpatic neuron receives a new input. New functionr receive, which calcule the new membrane potential of the post-synpatic neuron. In receive, add assert function to check if the change in the memrbane potential is equal to the amplitude received. 
Remember to always put the case of the time buffer at 0 every time the amplitude stored has been used. 
When tested, the porgram works. The post-synpatic neurons receives the input with the correct delay.
