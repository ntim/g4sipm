.. G4Sipm properties files

================
Properties files
================

The class *Properties* provides the parsing of simple properties files.

------------------------
Definition of a property
------------------------

A simple property can be defined by::

   key: value
   
or with a unit (the CLHEP system of units is used)::
   
   key: value * unit
   
Also available as a unit is the "%" sign.

--------------------------------
Definition of a properties table
--------------------------------

You can think of many properties which have to be supplied as a function of a certain variable. 
For this, a table can be defined.
The header reads::

   table-name: tabular
      field1   field2   field3   ....
      
Or again with units::
   table-name: tabular
      field1 / unit  field2 / unit  field3 / unit  ...
      
whereas the columns should be separated by tabulators to increased readability.
Additionally, the properties could be edited with a spreadsheet software when importing / exporting as CSV file.

^^^^^^^
Example
^^^^^^^

For SiPMs, the photon detection efficiency is a function of the photon wavelength::

   photonDetectionEfficiency: tabular                             
      entry wavelength / nm   efficiency / %
      1  319.490  9.18187959596
      1  320.882  9.79401373737
      1  322.274  10.2530812121
      1  323.666  12.7016177778
      1  326.450  14.3849535354
      1  327.842  14.9970876768
      1  329.234  16.2213559596
      1  330.626  16.833490101
      ...

------
Source
------

.. doxygenclass:: Properties
   :members: