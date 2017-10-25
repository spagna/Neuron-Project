# Neuron-Project

Create the class Neuron on Neuron.hpp with attributes for the membrane potential, the number of spikes and the time the spikes occured.
Create a Neuron.cpp. Add the setters and getters of the previous attributes.
Implement the function update in main.cpp. The program is not complete and doesn't work.
Another conception of the update function, that has been moved in Neuron.cpp. It compiles.
In main.cpp, a file Datas.txt opens and stores the membrane potential at the end of the simulation. Better to store all the membrane potential during the simulation (have to change it). The main.cpp calls the update function. The program works: the simulation has 5 different spikes with an external input of 1.01. 
New conception: changed all the time (that were double) into int: they've been transformed in steps of the simulation to make comparison better and more precise. The program works and the number of spikes is always 5 with the same external input. 
Change in the main.cpp. All the code relatif to the Datas.txt has been moved in update in Neuron.cpp, so that the membrane potential is stored at each steps of the simulation. The program works, but the number of spikes doesn't match with the solution.
Add a neuron clock for the start and the end (as time constants) of the time interval where the neuron recevies an external input. Add a function that update the state of the neuron in case of spike (better modularisation). The program works, and the number of spikes and the time the spikes occured now match.
All the files made so far have been deplaced in a new branch: that was the one neuron program.

The program has to evolve: it has to work for two neurons and one connection.



Add a new attribute: a time buffer array. The array will store the amplitude J recevied from a neuron that has spiked, and it stores that data with a certain a D.
Problems: difficult to know how to fill the ring buffer, and how it has to work exactly. I've tried to use a vector (even if it's more expansive in terms of time). The vector is filled but I don't know how the post-synpatic neuron has to receive it. After trying the function receive with the time buffer vector and the time plus the delay (in step) as argument, the program doesn't work. 
The time buffer returns array. No more changes added.
