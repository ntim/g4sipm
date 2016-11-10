.. G4Sipm voltage trace digitizer

==============================
G4Sipm voltage trace digitizer
==============================

The voltage trace digitizer uses the :cpp:class:`G4SipmDigiCollection` and the :cpp:class:`G4SipmVoltageTraceModel` to create the voltage trace of the SiPM.

The digitizaton can be enabled/disabled by a macro command::

   /g4sipm/digitize/trace 1

The digitization of the hits has to be enabled.

Please also refer to the `Geant4 digitization documentation <http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/ch04s05.html>`_.

G4Sipm voltage trace digi collection
====================================

The voltage traces are saved as :cpp:class:`G4SipmVoltageTraceDigi` in a :cpp:class:`G4SipmVoltageTraceDigiCollection`.
Each :cpp:class:`G4SipmVoltageTraceDigiCollection` is uniquely identified by the :cpp:type:`G4SipmId`.
Thus, given the id is zero::

   g4sipmVoltageTraceDigis-0

------
Source
------

.. doxygenclass:: G4SipmVoltageTraceDigitizer
   :members: