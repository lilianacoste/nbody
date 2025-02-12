# nbody simulates how objects move when they pull on each other with gravity
In c++ we will create a particle with initializing its position,velocity,gravity,and mass. 
# running the simulation
In order to run the simulation to generate random particles we write:
> ./nbody <number of particles> <time step> <number of iterations> <dump frequency>
In order to run the simulation based off predetermined particles (sun,earth,moon):
> ./nbody predefined <time step> <number of iterations> <dump frequency>
In order to run  the simulation based off a file we write:
> ./nbody <filename> <time step> <number of iterations> <dump frequency>
