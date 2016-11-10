.. G4Sipm cell fire controller

===========================
G4Sipm cell fire controller
===========================

The cell fire controller is a container which holds the points in time of the last breakdown all SiPM cells. 
Thus, the cell fire controller can compute the current gain of the cell and can decide whether the cell can break down or not.

The dead time check can be enabled/disable by a macro command::

   /g4sipm/filter/timing 1

Source
======

.. doxygenclass:: G4SipmCellFireController
   :members: