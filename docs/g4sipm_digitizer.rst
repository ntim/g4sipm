.. G4Sipm digitizer

================
G4Sipm digitizer
================

The purpose of the :cpp:class:`G4SipmDigitizer` is to simulate the SiPM on a single cell basis creating a list of cell breakdowns using the :cpp:class:`G4SipmHitCollection` and the properties of the :cpp:class:`G4SipmModel`.
Since the digitization in Geant4 takes place after the completion of the particle tracking, in this case ray tracing, all needed information is available.
Cell breakdowns or cell triggers are represented by the :cpp:class:`G4SipmDigi` class.
All :cpp:class:`G4SipmHit` instances are transformed into :cpp:class:`G4SipmDigi` instances and sorted chronologically into a list.

The digitization can be enabled/disabled with a macro command::

   /g4sipm/digitize/hits 1
   
In case no :cpp:class:`G4SipmHit` was created, the creation of dark noise can be enabled/disabled::

   /g4sipm/noise/ifNoSignal 1

Please also refer to the `Geant4 digitization documentation <http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/ch04s05.html>`_.

-------------
Thermal noise
-------------

Given, the time stamps of the :cpp:class`G4SipmHit`s vary between :math:`t_0` and :math:`t_1`, thermal noise is created.
To ensure a quasi stable state of the SiPM Monte Carlo model, thermal noise is created in a larger time window.
Each cell of the SiPM is allowed to produce a mean number of thermal noise triggers before :math:`t_0` and after :math:`t_1`::

   /g4sipm/noise/preThermal 3
   /g4sipm/noise/postThermal 1
   
The thermal noise creation can be enabled/disabled with a macro command::

   /g4sipm/digitize/hits 1
   
All thermal noise triggers are added to the chronologically sorted list.

----------------
Correlated noise
----------------

Once the thermal noise triggers have been created, the chronologically sorted list is traversed.
For each trigger, correlated noise, i.e. optical crosstalk and afterpulsing, may be created.

To enable/disable correlated noise, use one of the following macro commands::

   /g4sipm/noise/afterpulse 1
   /g4sipm/noise/crosstalk 1

------
Source
------

.. doxygenclass:: G4SipmDigitizer
   :members: