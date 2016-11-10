.. G4Sipm sensitive detector

=========================
G4Sipm sensitive detector
=========================

The :cpp:class:`G4SipmSensitiveDetector` is automatically created for each :cpp:class:`G4Sipm` and uniquely identified by the :cpp:type:`G4SipmId`.
Thus, given the id is zero::

   g4sipmSd-0
   
It is attached to the silicon chip representation of the :cpp:class:`G4Sipm`.
The sensitive detector is responsible for the creation of :cpp:class:`G4SipmHit` objects.

To respect the effects of dead space and the photon detection efficiency, each :cpp:class:`G4SipmSensitiveDetector` has a :cpp:class:`G4SipmSensitiveDetectorFilter`.

Source
======

.. doxygenclass:: G4SipmSensitiveDetector
   :members: