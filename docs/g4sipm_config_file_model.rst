.. G4Sipm config file model

========================
G4Sipm config file model
========================

The :cpp:class:`G4SipmModel` is one of the most important classes of G4Sipm. 
It stores all properties of the SiPM.

Working conditions
==================

The model stores the working conditions of the SiPM: environmental temperature and bias voltage.

The environmental temperature can be set via the :cpp:func:`G4SipmModel::setTemperature()` function, the bias voltage with :cpp:func:`G4SipmModel::setBiasVoltage()`.
The over-voltage of the SiPM is the difference between the bias voltage and the breakdown voltage

SiPM properties
===============

The model also stores the following properties of SiPMs 

* breakdown voltage
* number of cells
* cell pitch
* geometrical fill factor
* thermal noise rate
* effective dead time of the cells during the avalanche breakdown
* recovery time of the cells
* optical crosstalk probability
* time constants and probabilities of the long and the short afterpulsing component
* variance of the gain
* photon detection efficiency as a function of the photon wavelength
* thickness of the entrance window

It also holds a reference to :cpp:class:`G4SipmGainMapModel` and :cpp:class:`G4SipmVoltageTraceModel`.

Config file example
===================

.. literalinclude:: ../g4sipm/resources/hamamatsu-s10362-11-100c.properties
   :lines: 1-80
   :append: ...

Source
======

Inherited by :cpp:class:`G4SipmConfigFileModel`, :cpp:class:`G4SipmGenericSipmModel`, :cpp:class:`HamamatsuS1036211100`, :cpp:class:`HamamatsuS1036233050`, :cpp:class:`HamamatsuS1036233100`, :cpp:class:`HamamatsuS10985100` and :cpp:class:`HamamatsuS12651050`.  

.. doxygenclass:: G4SipmModel
   :members:
