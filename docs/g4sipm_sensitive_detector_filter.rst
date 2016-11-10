.. G4Sipm sensitive detector filter

================================
G4Sipm sensitive detector filter
================================

The :cpp:class:`G4SipmSensitiveDetectorFilter` implements the virtual method::

   G4bool Accept(const G4Step*)

of its base class :cpp:class:`G4VSDFilter` which returns true if the photon should be scored by the :cpp:class:`G4SipmSensitiveDetector` while considering the effects of dead space and the photon detection efficiency.

Dead space
==========

The photon should not hit the dead space on the Sipm. 
This is checked by the :cpp:func:`G4SensitiveDetectorFilter::acceptGeometry` function.

In G4Sipm, the cells of a Sipm are modeled as squares surrounded by a dead space border.

The dead space check can be enabled/disabled by a macro command::

   /g4sipm/filter/geometry 1
   

Photon detection efficiency
===========================

This is handled by the :cpp:func:`G4SensitiveDetectorFilter::acceptPde` function.
The photon detection efficiency (PDE) is a function of the photon wavelength.
A random number between zero and one is picked from a uniform distribution.
If the random number is smaller than the PDE for the photon wavelength, the method returns true.

To account for the reflectivity of the entrance window of the SiPM, the PDE is divided by its transmittance. 
The transmittance is calculated from a Fresnel-equation for three-layer transmittance whereas for inclined light the transmittance is calculated for unpolarized light.
For perpendicular incidence, the transmittance is calculated for parallel polarization following Jackson's convention.

The PDE check can be enabled/disabled by a macro::

   /g4sipm/filter/pde 1

Source
======

.. doxygenclass:: G4SipmSensitiveDetectorFilter
   :members:
   :protected-members:
   :undoc-members:
   