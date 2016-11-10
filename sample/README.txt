<h1>Sample Simulation</h1>

<p>This Geant4 simulation features a fairly simple setup with only one SiPM illuminated by a quadratic light beam.</p>
<p>The SiPM model can be configured by name or using a properties file. The outcome of the simulation is written into plain text file with JSON syntax 
	which is basically supported by the majority of programming languages</p>
	
	
<h2>Run Scripts</h2>
<p>The directory "run" contains several Python scripts which invoke the simulation to test several features of the SiPM 
	e.g. photon detection efficiency as function of the photon wavelength or as function of the incident angle. Invoke the script by e.g. <code>python run/pde.py</code>. 
	When the simulations are finished, the corresponding plot can be produced by <code>python plots/pde.py</code></p>
	
<h2>Plot Scripts</h2>

<p>The plots supplied with this example are generated using <a href="http://matplotlib.org/">Matplotlib</a> for Python.</p> 