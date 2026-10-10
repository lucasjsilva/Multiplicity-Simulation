# Multiplicity-Simulation
Repository for Monte Carlo event generators scripts for simulation regarding multiplicity distributions in high-energy collisions. The following tools were used to perform the simulations:

* [**Pythia8**](https://pythia.org/)
* [**AMPT**](https://myweb.ecu.edu/linz/ampt/) (A Multi-Phase Transport Model for Relativistic Heavy Ion Collisions)

# Pythia-Scripts
Scripts developed in order to simulate data in Pythia, specially data on charged-particle multiplicity distribution.

# Content description
Main directories: 
* **Tutorials** 
* **Multiplicity-Scripts**

### Tutorials
In this directory, one can find a set o simple tutorials in which focus on the learning process of the framework syntaxe. All of them are available in video on [Physics Matter Channel link](https://www.youtube.com/@physics_matters).

* **tutorial1.cc** simulates a proton-proton collision
* **tutorial2.cc** Creates histograms from simulated data
* **tutorial3.cc** Creates ROOT Trees from simulated data

### Multiplicity-Scripts
In this directory, one can locate a examples of simulations developed for the analysis of multiplicity distribution according to energy, $\eta$ and $p_T$ cuts, also other relevant simulations for the study of charged particle production in proton-proton collision or proton-nucleos collision.

The directory is divided in three sections, and its hierarchy is presented bellow:
* **/analysis** - Folder with Python and R scripts that analyse the results from scripts, making graphics and other plots
* **/results** - Folder with some of the results made by the analysis
* **/src** - Folder with scripts for the simulation

# Tips
While running the executables, the following error may appears:
~~~
error while loading shared libraries: libpythia8.so: cannot open shared object file: No such file or directory
~~~

In this case, the path to the correct library is needed. In order to define it, run the command mentioned bellow:

~~~bash
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/home/lucasjfsilva/pythia/pythia8313/lib
~~~

In case something similar appears while executing a Pythia script that contains a specific plugin (e.g. HepMC3), run the following command:

~~~bash
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH 
~~~

## Makefile
In order to make it easy the compilation process, one can create a **makefile** that register the command:
 
 ~~~bash
touch Makefile
 ~~~

The simplest structure for this file is:

~~~Makefile
executable_name : file_to_be_compiled.xx
    g++ $@.cc -I/path/to/include -L/path/to/lib -lpythia8 -o $@
~~~

Once the file is created, one only needs to the execute the following commands in order to compile and run the executable, respectively

~~~bash
make executable_name
./executable_name
~~~