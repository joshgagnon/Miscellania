#!/usr/bin/python
from BeautifulSoup import BeautifulSoup          # For processing HTML
import re
import os
import urllib
import subprocess 


proxies = []
count =0

def load_proxies():
    global proxies
    data = open('./proxies','r').read()
    p = re.compile('([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}:[0-9]{2,5}).+Anonymous')
    res= p.match(data)
    for m in p.finditer(data):
        proxies.append(m.group(1))
    return

def switch_proxy():
    global count
    os.popen("export http_proxy="+proxies[ count % len(proxies)])
    count+=1;
    return 


name_pattern = re.compile('>([a-zA-Z]+) Surname</A>')
for n in range(ord('a'), ord('z')+1):
    for m in range(ord('a'), ord('z')+1):
        
        page = urllib.urlopen('http://www.surnameweb.org/registry/'+chr(n)+chr(m)+'.htm').read()
       
        res = name_pattern.match(page)
        for m in name_pattern.finditer(page):
            print m.group(1)
