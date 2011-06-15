#!/usr/bin/python

import urllib2
import os
import re

list = os.listdir(os.getcwd()+'/tests')

parse = re.compile('"season": ([0-9]+), "name": "([^"]+)", "number": ([0-9]+)}')
 
print " ".join(list)
for show in list[:1]:
    wohs = show.replace('_', '+');
    results = urllib2.urlopen('http://imdbapi.poromenos.org/js/?name=%s'%wohs ).readlines()
    print results[0]
    episodes = re.findall(parse,results[0])
    
    print episodes[0]
    
