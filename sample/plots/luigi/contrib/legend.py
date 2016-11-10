#!/usr/bin/env python
# -*- coding: utf-8 -*-
from matplotlib.table import Table
from matplotlib.artist import allow_rasterization
from matplotlib.text import Text
import sig

class LegendTable(Table):
    def __init__(self, ax, loc=None, bbox=None, title=None, offset=None):
        Table.__init__(self, ax, loc, bbox)
        self.title = title
        self.offset = offset
        if self.offset is None:
            self.offset = [0, 0]
        if self.title:
            self.offset[1] -= 0.075
    
    @allow_rasterization
    def draw(self, renderer):
        Table.draw(self, renderer)
        # Add title 
        if self.title != None:
            # Position using loc
            (BEST, UR, UL, LL, LR, CL, CR, LC, UC, C,
             TR, TL, BL, BR, R, L, T, B) = range(len(self.codes))
            # Get table bounds.
            bounds = self._get_grid_bbox(self._cachedRenderer).bounds
            y = bounds[1] + bounds[3] + self._approx_text_height() / 2
            # Default for left
            x = bounds[0]
            ha = 'left'
            # Right
            if self._loc in (BEST, UR, LR, R, CR):
                x = bounds[0] + bounds[2]
                ha = 'right'
            # Center
            if self._loc in (LC, UC, C):
                x = bounds[0] + bounds[2] / 2.0
                ha = 'center'
            # Create text object
            title = Text(x, y, text=self.title, ha=ha, va="bottom")
            title.set_figure(self.figure)
            title.set_transform(self.get_transform())
            title.draw(self._cachedRenderer)
    
    def _offset(self, ox, oy):
        Table._offset(self, ox + self.offset[0], oy + self.offset[1])

def add(ax, labels, values, errors=None, units=None, loc="bottom", bbox=None, title=None, offset=None):
    '''
    Creates a ROOT (root.cern.ch) style fit statistics legend taking variable names (labels), 
    their values and uncertainties (errors) as well as their unit.
    Formats everything with 3 significant digits, or up to two significant digits for values with
    uncertainty given using Latex.
    '''
    table = LegendTable(ax, loc, bbox, title, offset)
    height = table._approx_text_height() * 1.5
    if errors is None:
        errors = [None for i in xrange(len(values))]
    if units is None:
        units = [None for i in xrange(len(values))]
    if len(labels) != len(values) or len(values) != len(errors) or len(values) != len(units):
        raise RuntimeError("labels, values and errors must have the same length")
    # Add rows to the Table
    row = 0
    for l, v, e, u in zip(labels, values, errors, units):
        table.add_cell(row, 0, width=1. / 3., height=height, text=l, loc='right', edgecolor='w')
        table.add_cell(row, 1, width=1. / 3., height=height, text='=', loc='center', edgecolor='w')
        text = sig.format(v, sig=3, exp=4)
        if e != None:
            text = sig.format_std(v, e, sig=1, exp=5)
        if u != None:
            text += ' ' + u
        table.add_cell(row, 2, width=1. / 3., height=height, text=text, loc='left', edgecolor='w')
        #
        row += 1
    # Auto size
    for i in xrange(3):
        table.auto_set_column_width(i)
    # Add to axes.
    ax.add_table(table)
