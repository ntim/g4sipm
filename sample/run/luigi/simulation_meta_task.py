#!/usr/bin/env python
# -*- coding: utf-8 -*-
import luigi
import sqlite3
import numpy as np
from config_meta_task import *
from run import *
from export_run_settings import *
import ast

class ListParameter(luigi.Parameter):
    def parse(self, arguments):
        return ast.literal_eval(arguments)

class SimulationMetaTask(ConfigMetaTask):
    n_repititions = luigi.IntParameter()
    start_index = luigi.IntParameter(default=0)
    run_kwargs = ListParameter(default="")

    def parse_run_kwargs(self, run_kwargs):
        if type(run_kwargs) is dict:
            return run_kwargs
        if type(run_kwargs) is str:
            return ast.literal_eval(run_kwargs)
        ValueError("Could not determine type of run_kwargs")

    def requires(self):
        run_kwargs = self.run_kwargs[:]
        if len(run_kwargs) == 0:
            run_kwargs = [{}] * self.n_repititions
        return [[self.clone(Run, index=i, **self.parse_run_kwargs(run_kwargs[i - self.start_index])) for i in xrange(self.start_index, self.n_repititions + self.start_index)], 
                self.clone(ExportRunSettings, **self.parse_run_kwargs(self.run_kwargs[0]))]

    def input_from_runs(self):
        return self.input()[0]

    def sqlite_from_runs(self):
        inputs = self.input_from_runs()
        return [input[1] for input in inputs]
        
    def output(self):
        return luigi.LocalTarget("%s/g4sipm.tsv" % (self.path()))
    
class SimulationDynamicMetaWrapperTask(SimulationMetaTask, luigi.WrapperTask):
    pass
    
class SimulationDynamicMetaTask(ConfigMetaTask, luigi.Task):
    n_repititions = luigi.IntParameter(100)
    step = luigi.IntParameter(10)
    run_after_yield = NotImplemented

    def run_kwargs(self):
        return [{}] * self.step

    def run(self):
        self.yielded_inputs = []
        for start in xrange(0, self.n_repititions, self.step):
            dep = self.clone(SimulationDynamicMetaWrapperTask, start_index=start, n_repititions=self.step, run_kwargs=self.run_kwargs())
            yield dep
            self.yielded_inputs.extend(dep.input_from_runs())
        self.run_after_yield()
        
    def input_from_runs(self):
        return self.yielded_inputs

    def sqlite_from_runs(self):
        return [input[1] for input in self.yielded_inputs]
        
    def output(self):
        return luigi.LocalTarget("%s/g4sipm.tsv" % (self.path()))
