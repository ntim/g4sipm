.. G4Sipm hit

==========
G4Sipm hit
==========

A :cpp:class:`G4SipmHit` is created by a :cpp:class:`G4SipmSensitiveDetector` when a Geant4 step of a photon goes through it. 
It stores the following properties of the step:

* :cpp:class:`G4Sipm` id
* track id
* kinetic energy
* global time
* weight (normally equals 1 but can be used to track photon bunches)
* local position on the SiPM
* absolute position in the simulation world
* momentum
* start position
* start momentum
 
Please also refer to the `Geant4 hits documentation <http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/ch04s04.html>`_.

G4Sipm hit collection
=====================

Each :cpp:class:`G4SipmHitCollection` is uniquely identified by the :cpp:type:`G4SipmId`.
Thus, given the id is zero::

   g4sipmHits-0
 
Source
======

.. doxygenclass:: G4SipmHit
   :members:
   :protected-members:
   :private-members: