.. G4Sipm gain map model

=====================
G4Sipm gain map model
=====================

Due to imperfections and electrical noise, the gain is not homogeneous over the whole SiPM surface.
Two implementations are available: :cpp:class:`G4SipmRandomGainMapModel` and :cpp:class:`G4G4SipmGaussianGainMapModel`.

Random gain map model
=====================

Picks the gain out of a Gaussian distribution with mean :math:`\mu = 1` and standard deviation :math:`\sigma` equal to the :cpp:func:`G4SipmModel::getGainVariation()`.
The individual gains stay the same over the whole simulation time.

Gaussian gain map model
=======================

Some SiPM models have a lower gain to the edges of the chip.
The gain is a 2D Gaussian function :math:`f(x,y)` with :math:`f(0,0)=1+3\sigma` and at in the corners :math:`f(d,d)=1-3\sigma` with the standard deviation :math:`\sigma` equal to the :cpp:func:`G4SipmModel::getGainVariation()`.

Source
======

.. doxygenclass:: G4SipmGainMapModel
   :members:
   