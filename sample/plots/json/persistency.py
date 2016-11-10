#!/usr/bin/env python
# -*- coding: utf-8 -*-

try:
    import json
except ImportError:
    # Load fallback module.
    import demjson
    json = None
    
def __loads(str):
    try:
        if json:
            return json.loads(str)
        else:
            return demjson.decode(str)
    except:
        print "persistency::__loads(): could not parse json: ", str[0:120], "..."
    
def get(fp, name):
    """
    Get the first instance of a specific class name
    @param fp - the file pointer.
    @param name - the name.
    """
    fp.seek(0)
    for l in fp.readlines():
        j = __loads(l)
        if j["class"] == name:
            return j
    return None;

def get_multiple(fp, name):
    """
    Get all instances of a specific class name
    @param fp - the file pointer.
    @param name - the name.
    """
    fp.seek(0)
    m = []
    for l in fp.readlines():
        j = __loads(l)
        if j["class"] == name:
            m.append(j)
    return m

def count(fp, name):
    """
    Count the number of occurences of a specific class name
    @param fp - the file pointer.
    @param name - the name.
    """
    fp.seek(0)
    c = 0
    for l in fp.readlines():
        j = __loads(l)
        if j["class"] == name:
            c += 1
    return c
