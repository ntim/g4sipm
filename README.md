# Welcome to G4SiPM

[![Docs](https://readthedocs.org/projects/g4sipm/badge/)](http://g4sipm.readthedocs.io/en/latest/index.html)

A Geant4 (https://github.com/Geant4/geant4) simulation toolkit for silicon photomultipliers (SiPMs).

Further information can be found here: http://dx.doi.org/10.1016/j.nima.2015.01.067

The code has been developed to
 * simulate SiPMs in (existing) Geant4 detector simulations
 * provide an easy to setup phenomenological model
 * be driven by input parameters which can be obtained from datasheets or in the laboratory
 * make reliable predictions of the signal of the SiPM over the complete dynamic range
 
Requirements
 * Geant4 (4.10 or newer, https://github.com/Geant4/geant4)
 * Boost (1.50.0 or newer, http://www.boost.org/)

Optional
 * Sqlite3 (https://sqlite.org/)
 * ROOT (5.34 or newer, https://root.cern.ch)
 
The build process has been tested with Ubuntu 15.04 and Scientific Linux 6.

## Getting started

Clone the code

    git clone https://github.com/ntim/g4sipm.git source
    cd source

Initialize submodules Googletest and Jansson    
    
    git submodule init
    git submodule update
    
Configure the build with CMake

    cd ../
    mkdir build
    cd build
    cmake ../source
    
Finally, start the build process

    make
    
## Documentation

The documentation is available at http://g4sipm.readthedocs.io/en/latest/index.html.

To build the documentation, invoke

    make docs
