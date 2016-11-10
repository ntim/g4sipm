.. G4Sipm digi

===========
G4Sipm digi
===========

A :cpp:class:`G4SipmDigi` is created by the :cpp:class:`G4SipmDigitizer`. 
It stores the following properties:

* :cpp:class:`G4Sipm` id
* :cpp:class:`G4SipmCellId`
* The time of the breakdown
* The weight, i.e. the gain of the cell at the breakdown whereas one equals to 1 p.e.
* :cpp:type:`G4SipmDigiType`, can be either *UNDEFINED*, *THERMAL*, *PHOTON*, *CROSSTALK* or *AFTERPULSE*
 
Please also refer to the `Geant4 digitization documentation <http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/ch04s05.html>`_.

G4Sipm digi collection
======================

Each :cpp:class:`G4SipmDigiCollection` is uniquely identified by the :cpp:type:`G4SipmId`.
Thus, given the id is zero::

   g4sipmDigis-0

Source
======

.. doxygenenum:: G4SipmDigiType

.. doxygenclass:: G4SipmDigi
   :members: