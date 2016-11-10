#!/usr/bin/env python
# -*- coding: utf-8 -*-
import luigi
import sqlite3
import glob
import numpy as np
from darknoise_simulation import *

class CrosstalkNeighboursConfigMetaTask(ConfigMetaTask):
    name = luigi.Parameter("crosstalk-neighbours-simulation")
    model = luigi.Parameter("../sample/resources/hamamatsu-s10362-11-100c.properties")
    noise_crosstalk_neighbours = luigi.IntParameter()

class CrosstalkNeighboursDarknoiseSimulation(CrosstalkNeighboursConfigMetaTask, DarknoiseSimulation):
    @property
    def run_kwargs(self):
        d = DarknoiseSimulation.run_kwargs.fget(self)
        return [clone(di, noise_crosstalk_neighbours=self.noise_crosstalk_neighbours) for di in d]
    
class CrosstalkNeighboursDeltaTDistribution(CrosstalkNeighboursConfigMetaTask, DeltaTDistribution):
    def requires(self):
        return self.clone(CrosstalkNeighboursDarknoiseSimulation)

class CrosstalkNeighboursPeDistribution(CrosstalkNeighboursConfigMetaTask, PeDistribution):
    def requires(self):
        return self.clone(CrosstalkNeighboursDarknoiseSimulation)

class All(luigi.WrapperTask):
    def requires(self):
        crosstalk_neighbours = [4, 8, 24, 48]
        return [[CrosstalkNeighboursDeltaTDistribution(noise_crosstalk_neighbours=c, path_spec=str(c)),
                 CrosstalkNeighboursPeDistribution(noise_crosstalk_neighbours=c, path_spec=str(c))] for c in crosstalk_neighbours]

if __name__ == "__main__":
    luigi.run(main_task_cls=All)
