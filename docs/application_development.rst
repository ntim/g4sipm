.. Application development

Application development
=======================

Apart from the G4Sipm sample simulation, G4Sipm can be easily integrated into existing projects.
This guide explains how to incorporate G4Sipm and use it as a library.

It will be assumed that your project also uses Geant4 and the CMake build system with separate source and build directory.

======================
Create a git submodule
======================

Create a directory inside your source directory::
   
   $ mkdir externals/g4sipm
   
Create a git submodule with::

   $ git add submodule ssh://forge.physik.rwth-aachen.de/git/g4sipm.git externals/g4sipm
   
Initialize and update to the latest version::

   $ git submodule init
   $ git submodule update
   
=================================
Add G4Sipm to your CMakeLists.txt
=================================

To enable the building of G4Sipm add the following lines to the *CMakeLists.txt* of your project::

   add_subdirectory(externals/g4sipm)
   include_directories(externals/g4sipm/g4sipm/include)
   include_directories(externals/g4sipm/externals/jansson/src)
   include_directories(externals/g4sipm/externals/gtest/include)
   

If you wish to include the export of the simulation data of the sample simulation in your build process, add the following lines::

   include_directories(externals/g4sipm/sample/include)
   set(LIBS ${LIBS} boost_program_options g4sipm g4sipm_sample boost_date_time jansson) 
   
Now, link the G4Sipm library to your project by extending the *target_link_libraries* command of your executable::

   target_link_libraries(target-name g4sipm boost_date_time jansson [other-libraries])
   
and with the sample simulation library::

   target_link_libraries(target-name g4sipm g4sipm_sample boost_program_options boost_date_time jansson [other-libraries])
   
=================
Creation of SiPMs
=================

New SiPMs can be created and placed in your :cpp:class:`G4VUserDetectorConstruction`:

.. code-block:: c++

   G4Sipm* sipm = new G4Sipm(g4sipmModel);
   
This creates a bare silicon chip without a window. 
The chip can be placed in a package represented by the :cpp:class:`G4SipmHousing`:

.. code-block:: c++

   new G4SipmHousing(sipm);
   
The G4Sipm class automatically creates the :cpp:class`G4SipmSensitiveDetector`, the :cpp:class`G4SipmDigitizer` and the :cpp:class:`G4SipmVoltageTraceDigitizer` and registers the instances to Geant4.

============
Digitization
============

Digitization is not automatically performed by Geant4.
The following snippet can be used to execute all registered digitizers in your :cpp:class:`G4UserEventAction`:

.. code-block:: c++

   G4DCtable* dcTable = digiManager->GetDCtable();
   for (int i = 0; i < dcTable->entries(); i++) {
      G4String dmName = dcTable->GetDMname(i);
      G4VDigitizerModule* dm = digiManager->FindDigitizerModule(dmName);
      if (dm) {
         dm->Digitize();
      }
   }
