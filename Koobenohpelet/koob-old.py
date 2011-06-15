#!/usr/bin/python
from BeautifulSoup import BeautifulSoup          # For processing HTML
import re
import os
import urllib2
import urllib
import subprocess 
import MySQLdb
import random
import string
import math

proxies = []
count =1

def connectdb():
    conn = MySQLdb.connect (host = "173.203.198.53",user = "yellow_koob",
                            passwd = "enohpelet",db = "yellow_koob")
    return conn.cursor()

def load_proxies():
    os.popen("export http_proxy=none")
    global proxies
    data = open('./proxies','r').read()
    p = re.compile('([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}:[0-9]{2,5})')
    res= p.match(data)
    for m in p.finditer(data):
        proxies.append(m.group(1))
    return

def switch_proxy():
    global count
    os.popen("export http_proxy="+proxies[ count % len(proxies)])
    prox = proxies[count % len(proxies)]
    print "Switching to proxy " + proxies[ count % len(proxies)]
    count+=1;
    return prox

def load_name_to_db(name):
    try:
        query = """insert into names(name, status) VALUES('%s', 0)""" % name
        curr.execute(query)
    except MySQLdb.Error, e:
        print e;
    return

def open_page(url):
    prox = switch_proxy()
    print "Opening ", url
    while(1):
        try:
            proxy_handler = urllib2.ProxyHandler({'http':prox})
            opener = urllib2.build_opener(proxy_handler)
            opener.addheaders = [('User-agent','Mozilla/5.0')]
            urllib2.install_opener(opener)
            req=urllib2.Request(url)
            sock=urllib2.urlopen(req,None,5)
            html = sock.read()
            if len(html) > 10000:
                return BeautifulSoup(html)
        except Exception, e:
            print e;
            proxies.remove(proxies[ count % len(proxies)])
            prox = switch_proxy()
    return

def fetch_random_name():
    curr = connectdb()
    query = """select count(*) from names"""
    curr.execute(query)    
    total = curr.fetchall()
    index = int(total[0][0])*random.random();
    query = """select name from names where id=%s"""% index;
    curr.execute(query)    
    name = curr.fetchall()
    return name[0][0]

def fetch_random_name_200max():
    curr = connectdb()
    query = """select count(*) from names"""
    curr.execute(query)    
    total = curr.fetchall()
    index = int(total[0][0])*random.random();
    query = """select name from names where hits > 20 and hits <= 200""";
    curr.execute(query)    
    name = curr.fetchall()
    print name[0][0]
    return name[0][0]



def remove_html_tags(data):
    p = re.compile(r'<.*?>')
    return p.sub('', data)

def query_site():
    try:
        name = fetch_random_name()
        #name = "aho"
        print "Querying ", name
        url='http://yellow.co.nz/whitepages/search/new+zealand/%s-1.html'% name
        soup = open_page("http://yellow.co.nz/whitepages/search/new+zealand/%s-1.html" % name.lower())

        spelling = soup.find('ul',attrs={'class':'spelling'})
        menu=soup.find('div',attrs={'class':'resultInfo '})
        p = re.search('([0-9]+)',str(menu));
        spelling = soup.find('ul',attrs={'class':'spelling'})
        if spelling:
            p = re.search('We found (.+) in new zealand',str(spelling))
            print "Spelling error, adding " + p.group(1) + " to database."
            load_name_to_db(p.group(1))
            
        elif p :        
            query = """update names SET status=0, hits=%s where name = '%s' """ % (p.group(0), name)
            print query
            curr.execute(query)
            if(int(p.group(0)) <=20):
                query = """update names SET status=1 where name = '%s' """ % name
                curr.execute(query)
                scrape_page_simple(soup)
                query= """update names SET status=2 where name = '%s' """ % name
                print query
                curr.execute(query)
            elif int(p.group(0)) >20 and int(p.group(0)) <=200: 
                query = """update names SET status=1 where name = '%s' """ % name
                curr.execute(query)
                scrape_page_simple(soup)
                for i in range(2,int(math.ceil(float(p.group(0))/20))+1):
                        scrape_page_simple(open_page("http://yellow.co.nz/whitepages/search/new+zealand/%s-%s.html" % (name.lower(),i)))
                query= """update names SET status=2 where name = '%s' """ % name
                print query               
        else:
            print "No %s in New Zealand." % name
            query= """update names SET status=2 where name = '%s' """ % name
            print query
            curr.execute(query)  
    except MySQLdb.Error, e:
        print e;

    return

def scrape_page_simple(soup):
    block = soup.findAll('tbody')
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
        except Exception, e:
            print e
    return

def main():
    random.seed()
    load_proxies()
    while 1:
        query_site()
    return

curr = connectdb()
main()
