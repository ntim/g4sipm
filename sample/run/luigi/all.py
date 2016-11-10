#!/usr/bin/env python
import luigi
import dynamic_range_simulation
import darknoise_simulation
import pde_simulation
import relative_pde_simulation
import n_pe_simulation
import crosstalk_neighbour_simulation

class All(luigi.WrapperTask):
    def requires(self):
        yield crosstalk_neighbour_simulation.All()
		yield darknoise_simulation.All()
		yield dynamic_range_simulation.All()
        yield n_pe_simulation.All()
		yield pde_simulation.All()
		yield relative_pde_simulation.All()
        
if __name__ == "__main__":
    luigi.run(main_task_cls=All)
