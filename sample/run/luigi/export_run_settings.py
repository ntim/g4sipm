#!/usr/bin/env python
import luigi
import sqlite3
import pickle
from config_meta_task import *
from run import *

class ExportRunSettings(RunConfigMetaTask, luigi.Task):
    def requires(self):
        return self.clone(Run, index=0)

    def run(self):
        tables = ['particleSourceMessenger', 'g4sipmUiMessenger', 'sipmModel', 'sipmVoltageTraceModel']
        if self.input()[1].exists():
            input = self.input()[1].fn
            print input
            # Open database.
            con = sqlite3.connect(input)
            con.row_factory = sqlite3.Row
            cur = con.cursor()
            # Select first entry of all tables.
            settings = dict()
            for table in tables:
                row = cur.execute("SELECT * FROM `%s`;" % table).fetchone()
                # Iterate all columns.
                entry = dict()
                for key, value in zip(row.keys(), row):
                    entry[key] = value
                # Add to settings dictionary
                settings[table] = entry
            # Pickle settings
            with self.output().open("w") as o:
                pickle.dump(settings, o)

    def output(self):
        return luigi.LocalTarget("%s/g4sipm.pkl" % (self.path()))
