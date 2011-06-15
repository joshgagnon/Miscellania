#!/usr/bin/python
from BeautifulSoup import BeautifulSoup          # For processing HTML
import re
import os
import sys
import urllib2
import urllib
import subprocess 
import MySQLdb
import random

proxies = []
count =1

def connectdb():
    conn = MySQLdb.connect (host = "localhost",user = "root",
                            passwd = "Se7en303",db = "koob")
    return conn.cursor()

def load_proxies():
    os.popen("export http_proxy=none")
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
    #proxy_handler = urllib2.ProxyHandler({'http': proxies[ count % len(proxies)]})
    #opener = build_opener(proxy_handler, proxy_auth_handler)
    prox = proxies[count % len(proxies)]
    print "Switching to proxy " + proxies[ count % len(proxies)]
    #os.popen("export http_proxy=''")
    count+=1;
    return prox

def load_name_to_db(name):
    try:
        query = """insert into names(name, status) VALUES('%s', 0)""" % name
        curr.execute(query)
    except MySQLdb.Error, e:
        print e;
    return


def load_file_names_to_db(filename):
    lines = open (filename,'r').readlines() 
    for line in lines:
        try:
            line = line.rstrip('\n')
            query = """insert into names(name, status) VALUES('%s', 0)""" % line
            conn.query(query)
        except MySQLdb.Error, e:
            print e;
    return

def open_page(url):
    while(1):
        try:
            prox = switch_proxy()
            proxy_handler = urllib2.ProxyHandler({'http':prox})
            opener = urllib2.build_opener(proxy_handler)
            opener.addheaders = [('User-agent','Mozilla/5.0')]
            urllib2.install_opener(opener)
            req=urllib2.Request(url)
            sock=urllib2.urlopen(req,None,5)
            html = sock.read()
            #print url
            #response = urllib2.urlopen(url,None,5)
            #html = response.read()
            return BeautifulSoup(html)
        except Exception, e:
            print e;
            prox = switch_proxy()
    return

def fetch_random_name_20max():
    curr = connectdb()
    query = """select name from names where status = 0 and hits < 20"""
    curr.execute(query)    
    names = curr.fetchall()
    return names[ int(len(names)*random.random())]

def remove_html_tags(data):
    p = re.compile(r'<.*?>')
    return p.sub('', data)

def query_site(n):
    try:
        name = (n,'')
        url='http://yellow.co.nz/whitepages/search/new+zealand/%s-1.html'% name[0]
        #response = urllib2.urlopen(url)
        soup = open_page("http://yellow.co.nz/whitepages/search/new+zealand/%s-1.html" % name[0])

        spelling = soup.find('ul',attrs={'class':'spelling'})
        menu=soup.find('div',attrs={'class':'resultInfo '})
        p = re.search('([0-9]+)',str(menu));
        spelling = soup.find('ul',attrs={'class':'spelling'})
        if spelling:
            p = re.search('We found (.+) in new zealand',str(spelling))
            print "Spelling error, adding " + p.group(1) + " to database."
            load_name_to_db(p.group(1))
            
        elif p :        
            query = """update names SET status=0, hits=%s where name = '%s' """ % (p.group(0), name[0])#change to name[0]
            print query
            curr.execute(query)
            if(int(p.group(0)) <20):
                scrape_page_simple(soup)
                query= """update names SET status=2 where name = '%s' """ % name[0]
                print query
                curr.execute(query)               
        else:
            print "No %s in New Zealand." % name[0]
            query= """update names SET status=2 where name = '%s' """ % name[0]
            print query
            curr.execute(query)  
    except MySQLdb.Error, e:
        print e;

    return

def scrape_page_simple(soup):
    block = soup.findAll('tbody')
    print len(block)
    for i in range( len(block)):
        name = block[i].find('a',attrs={'class':'detailsLink'})
        addy = block[i].find('dd',attrs={'class':'bizAddr'}) 
        num = block[i].find('span',attrs={'class':'iconSprite phoneStatic'})
        try:
            name = remove_html_tags(str(name)).replace('&amp;','&')     
            num = remove_html_tags(str(num))
            if addy:
                addy =remove_html_tags(str(addy))
                query = """insert into directory(name, address, number) VALUES('%s', '%s', '%s')""" % (name,addy,num)
                print query  
                curr.execute(query) 
            else:
                query = """insert into directory(name, address, number) VALUES('%s', 'NULL', '%s')""" % (name,num)
                print query
        except MySQLdb.Error, e:
            print e
    return

def main(argv):
    random.seed()
    load_proxies()
    
#    load_file_names_to_db("lastnames");
    load_name_to_db(argv[0])
    query_site(argv[0])
    
    return

#proxy_handler = urllib2.ProxyHandler({})
#opener = urllib2.build_opener(proxy_handler)
#urllib2.install_opener(opener)
curr = connectdb()
main(sys.argv[1:])
