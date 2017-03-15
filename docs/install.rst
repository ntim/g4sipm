.. Install

###############
Getting started
###############

Introduction
============

G4Sipm uses the CMake to configure a build system for compiling. 
For more details on Geant4 with CMake itself, the `Geant4 documentation <http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/InstallationGuide/html/ch01.html>`_ should be consulted.

==============================
Supported and Tested Platforms
==============================

`Scientific Linux <https://www.scientificlinux.org/>`_ CERN 6 with gcc 4.7.X (also 64bit).

=================
Required packages
=================

The following packages are required for a successful build of G4Sipm.

* `Geant <http://geant4.web.cern.ch>`_ ~4.10.0
* `Boost <http://www.boost.org>`_ ~1.41.0 (required for the reading of properties files)

The tilde signifies that G4Sipm is tested with this version, higher versions should work as well.

Install Geant4 according to the official `Building and Installation Guide <http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/InstallationGuide/html/ch02.html>`_ with CLHEP and Qt if possible.

--------
Optional
--------

Optional dependencies may be installed beforehand to enable certain features.

* `ROOT <http://root.cern.ch>`_ ~5.34 (export of simulation data)
* `SQlite3 <http://www.sqlite.org>`_ ~3.6.20 (export of simulation data)
* `Pyhton <http://www.python.org>`_ ~2.7.6 (batch invocation of the simulation)
* `Matplotlib <http://matplotlib.org>`_ ~1.2.1 (creation of plots)
* `Sphinx <http://sphinx-doc.org>`_ ~1.2.2 (building of this documentation)
* `Breathe <http://breathe.readthedocs.org/en/latest>`_ (inclusion of C++ source code in the documentation)
* `Doxygen <http://www.stack.nl/~dimitri/doxygen>`_ ~1.6.1 (creation of the C++ source code documentation)

-----------------
Included packages
-----------------

Several packages are included in the G4Sipm source tree.

* `GoogleTest <http://code.google.com/p/googletest>`_ 1.7.0 (automated testing)
* `GCovr <http://gcovr.com>`_ 2.5-prerelease (generating test coverage reports)
* `Jansson <http://www.digip.org/jansson>`_ 2.6 (export of simulation data)

Building G4Sipm
===============

For illustration only, this guide will assume G4Sipm resides in a directory named */path/to*.

The next step is to create a directory in which we clone the G4Sipm repository::

   $ cd /path/to
   $ mkdir g4sipm
   $ cd g4sipm
   
================
Clone repository
================

Clone the repository to a directory *source*::

   $ git clone https://github.com/ntim/g4sipm.git

Now, create a directory to configure and run the build and store the build products. This directory should **not** be the same as, or inside, the source directory. 
In this guide, we create this build directory alongside our source directory::

   $ mkdir build

=====================
Configuring the build
=====================

To configure the build, i.e. the creation of Unix Makefiles with CMake, change into the build directory and run CMake::

   $ cd build
   $ cmake ../source

====================
G4Sipm build options
====================

By running::

   $ ccmake ../source
   
the CMake build variables can be inspected and modified.
The relevant build variables and their purpose are stated in the following:

CMAKE_BUILD_TYPE
   It is adviced to set this variable to *RelWithDebInfo* to enable debugging with low performance impact which is useful for application developement.

CMAKE_INSTALL_PREFIX
   Not used.

WITH_COVERAGE_ANALYSIS
   Enables the coverage analysis.
   
WITH_ROOT
   Enable *ROOT* support for the export of simulation data.
   
WITH_SQLITE
   Enable *SQLite* support for the export of simulation data.
   
===================
Compiling of G4Sipm
===================

After the configuration has run, CMake will have generated Unix Makefiles for compiling G4Sipm. To run the build, simply execute make in the build directory::

   $ make -jN

where N is the number of parallel jobs you require (e.g. if your machine has a dual core processor, you could set N to 2).
The build will now run, and will output information on the progress of the build and current operations. 
If you need more output to help resolve issues or simply for information, run make as::

   $ make -jN VERBOSE=1
   
-----------------------
Additional Make targets
-----------------------

The following additional Make targets are available

help
   Print all Make targets

runG4sipmTest
   Runs the GoogleTest tests of the G4Sipm library.

runSampleTest
   Runs the GoogleTest tests of the sample simulation.

doc
   Builds this documentation
   
doc_doxygen
   Builds a doxygen documentation  

==========================
G4Sipm directory structure
==========================

After finishing the compilation of G4Sipm, the following directories (besides the CMake directories) should appear in your build directory

* doc
* externals
* g4sipm
* sample

whereas the *g4sipm* directory contains the G4Sipm library and the *sample* directory contains a simple simulation which is intended to provide a quick-start for Geant4 beginners.

Developing of G4Sipm using Eclipse
==================================

G4Sipm has been developed using `Eclipse IDE for C/C++ Developers <http://eclipse.org>`_ which I **strongly** recommend for everyone which thinks of developing a serious application.

CMake provides a special Makefile generator::

   $ cmake ../source -G"Eclipse CDT4 - Unix Makefiles" <other-options>
   
which automatically creates a *.project* and a *.cproject* file in the build directory which will be understood by Eclipse. 
Import G4Sipm into Eclipse via *File* -> *Import...* -> *Existing Projects into Workspace*.
