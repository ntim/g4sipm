.. G4Sipm voltage trace model

==========================
G4Sipm voltage trace model
==========================

The voltage trace model is used to mimic the SiPM signals read with an flash ADC or oscilloscope.
A pulse is parameterized as a double exponential (falling and rising edge) function.

Voltage trace properties
========================

The following properties can be set:

* Amplitude of 1 p.e. pulse
* The time bin width, i.e. the time difference between two voltage readings 
* The time constant :math:`\tau_{rise}` of the rising edge
* The time constant :math:`\tau_{fall}` of the falling edge
* The baseline offset :math:`v_{0}` 
* The white noise sigma
* The precision of the voltage trace in bits.

------
Source
------

.. doxygenclass:: G4SipmVoltageTraceModel
   :members:
   