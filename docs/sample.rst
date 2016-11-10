.. Sample simulation

#################
Sample simulation
#################

This is a simple simulation which is intended to provide a quick-start for Geant4 beginners.
It resides in the *sample* directory of G4Sipm.

Invocation
==========

A new simulation can be started with::
   
   $ ./sample
   
which also creates a GUI window in which you should see a graphical representation of the simulation.

======================
Command line arguments
======================

The following command line arguments are available

help
   Produce help message.
   
mac
   Macro input file which will be processed without creating the GUI.
   
output
   The path to the output directory
   
model
   The model string of the SiPM model to simulate. Can be one of "HamamatsuS1036211100", "HamamatsuS1036233100", "HamamatsuS1036233050", "HamamatsuS10985100" and "HamamatsuS12651050" or a path to a config file.

housing
   The type of housing in which the SiPM is packaged. Can be one of "default", "ceramic" and "smd".
   
========
GUI mode
========

If invoked without the *mac* command line argument, the simulation runs in the GUI mode.
A QT window with a 3D visualization is created.
For more details, please refer to the `Geant4 visulization documentation <http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/ch08.html>`_.

==========
Batch mode
==========

If the *mac* command line argument is supplied, the simulation runs in batch mode::
   
   ./sample --mac run.mac
   
After executing all commands stored in the *macro*-file, the program exits.
For more details on *macro*-files, please refer to the `Geant4 documentation <https://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/ch02s10.html#sect.HowToExec.BatchMacro>`_.
This is particularly useful for executing many simulations consecutively as e.g. a dynamic range simulation.

The most basic example of a mac file is the *run.mac*::
   
   # Generate event.
   /run/beamOn 1
   
Run scripts
===========

The repository of G4Sipm also contains several so-called *run-scripts*.
It is a collection of python scripts which invoke the simulation with temporarily created *macro*-files for the generation of particular plots.
The run scripts should be invoked from the *sample* directory::

   python run/pde.py
   
======
pde.py
======

Varies the photon wavelength for the investigation of the photon detection efficiency as a function of the photon wavelength.
The output is written to *./results/pde*.
A temporary *macro*-file will be created::

   /g4sipm/filter/timing 0
   /g4sipm/digitize/hits 0
   /g4sipm/digitize/trace 0
   /ps/energy/eMin %g eV
   /ps/energy/eMax %g eV
   /ps/nParticles %d
   /run/beamOn 1
   
with the string replacements "%g" and "%d".

===============
relative_pde.py
===============

Similar to the *pde.py* script, this script investigates the photon detection efficiency as a function of the angle of incidence of the light.
The output is written to *./results/relative_pde*.
Instead of only changing the angle of incidence, the light source is rotated around the y-axis whereas the surface normal points to the center of the SiPM. 

Plot scripts
============

G4Sipm also provides several python scripts for plotting simulation data with matplotlib. 
Depending on the activated export method (Json, ROOT or SQlite), the plot scripts can be found in either one of the following directories::

   plots/json
   plots/root
   plots/sqlite
   
The scripts are thought to be invoked from the sample simulation directory::

   python plots/sqlite/pde.py
