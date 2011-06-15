#!/usr/bin/python
from BeautifulSoup import BeautifulSoup          # For processing HTML
from threading import  Thread 
import re
import os
import urllib2
import urllib
import subprocess 
import MySQLdb
import random
import string
import math
import time
import glob
import logging
import logging.handlers
import hashlib

exit_now=0
proxies = []
domain ='http://yellow.co.nz'
useproxy = 1
my_logger = logging.getLogger('MyLogger')
my_logger.setLevel(logging.DEBUG)


class Connection:
    def __init__(self):
        while 1:
            try:
                if useproxy:
                    self.count=int(len(proxies)*random.random())
                    self.prox=proxies[self.count % len(proxies)]
                self.conn=connectdb()
                self.url=urllib2
                return
            except Exception, e:
                my_logger.debug( e )
                my_logger.debug( "Waiting for connection" )
                time.sleep(30)
    def __del__(self):
        try:
            self.conn.close()
        except Exception, e:
            my_logger.debug( e )
            
def setup_logging():
    LOG_FILENAME = '/home/gonzoisme/Koobenohpelet/log.out'
    handler = logging.handlers.RotatingFileHandler(
              LOG_FILENAME, maxBytes=2000000, backupCount=5)

    my_logger.addHandler(handler)


def connectdb():
    #conn = MySQLdb.connect (host = "127.0.0.1",port=3307,user = "yellow_koob",
    #                        passwd = "enohpelet",db = "yellow_koob")
    conn = MySQLdb.connect (host = "127.0.0.1",user = "root",
                            passwd = "Se7en303",db = "koob")
    return conn.cursor()


def db_update(query,connection):
    print query
    my_logger.debug( query )
    #connection.conn.execute(query)


def load_proxies():
    os.popen("export http_proxy=none")
    global proxies
    data = open('./proxies','r').read()
    p = re.compile('([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}:[0-9]{2,5})')
    res= p.match(data)
    for m in p.finditer(data):
        proxies.append(m.group(1))
    return 

def switch_proxy(proxy):
    proxy.count+=1;
    proxy.count=int(len(proxies)*random.random())
    proxy.prox = proxies[proxy.count % len(proxies)]
    my_logger.debug( "Switching to proxy %s" % proxy.prox)
   
    proxy_handler = proxy.url.ProxyHandler({'http':proxy.prox})
    opener = proxy.url.build_opener(proxy_handler)
    opener.addheaders = [random.choice([('User-agent','Mozilla/4.0'),('User-agent','Mozilla/5.0'),('User-agent','Opera/9.20')])]
    proxy.url.install_opener(opener)
    return

def load_name_to_db(name,connection):
    try:
        query = """insert into names(name, status) VALUES('%s', 0)""" % name
        connection.conn.execute(query)
    except MySQLdb.Error, e:
        my_logger.debug( e)
    return

def open_page(url,connection):
    my_logger.debug( "Opening %s" % url )
    while(1):
        try:
            req=connection.url.Request(url)
            sock=connection.url.urlopen(req,None,10)
            html = sock.read()
            if len(html) > 10000:
                f = open('pages/'+hashlib.sha1(url).hexdigest()+'.html',"w")
                f.write(html)
                f.close
                return BeautifulSoup(html)
        except Exception, e:
            my_logger.debug( e )
            #print e 
            if useproxy:
                switch_proxy(connection)
    return

def fetch_random_name(connection):
    query = """select count(*) from names"""
    connection.conn.execute(query)    
    total = connection.conn.fetchall()
    while 1:
        index = int(total[0][0])*random.random();
        query = """select name,status from names where id=%s && status=0"""% index;
        connection.conn.execute(query)    
        name = connection.conn.fetchall()
        if name:
            return name[0][0]

def fetch_random_name200plus(connection):
    query = """select count(*) from names"""
    connection.conn.execute(query)    
    total = connection.conn.fetchall()
    query = """select name from names where hits > 200""";
    connection.conn.execute(query)    
    name = connection.conn.fetchall()
    return name[int(len(name)*random.random())][0]


def remove_html_tags(data):
    p = re.compile(r'<.*?>')
    return p.sub('', data)

def query_site(connection):
    try:
        if useproxy:
            switch_proxy(connection)
        name = fetch_random_name200plus(connection)
        my_logger.debug( "Querying %s" % name)
        url='http://yellow.co.nz/whitepages/search/new+zealand/%s-1.html'% name
        soup = open_page("http://yellow.co.nz/whitepages/search/new+zealand/%s-1.html" % name.lower(),connection)
        spelling = soup.find('ul',attrs={'class':'spelling'})
        menu=soup.find('div',attrs={'class':'resultInfo '})
        p = re.search('([0-9]+)',str(menu));
        spelling = soup.find('ul',attrs={'class':'spelling'})
        if spelling:
            p = re.search('We found (.+) in new zealand',str(spelling))
            my_logger.debug( "Spelling error, adding %s  to database." %p.group(1))
            load_name_to_db(p.group(1),connection)
        elif p:
            hits = int(p.group(0))
            query = """update names SET status=1, hits=%s where name = '%s' """ % (hits, name)
            db_update(query,connection)
            scrape_page_simple(soup,connection)
            if hits >20 and hits <=200: 
                for i in range(2,int(math.ceil(float(hits)/20))+1):
                        scrape_page_simple(open_page("http://yellow.co.nz/whitepages/search/new+zealand/%s-%s.html" % (name.lower(),i),connection),connection)                             
            elif hits > 200:
                init=soup.find('div', attrs={'class':'initials'})
                links = getlinks(init)
                for i in links:
                    url = domain+i;
                    scrape_refine(url,connection)
        query= """update names SET status=2 where name = '%s' """ % name
        db_update(query,connection)
    except MySQLdb.Error, e:
        my_logger.debug( e )
    return

def scrape_page_simple(soup,connection):
    block = soup.findAll('tbody')
    for i in range( len(block)):
        name = block[i].find('td',attrs={'class':'datarow busNameWhite'})
        addy = block[i].find('dd',attrs={'class':'bizAddr'}) 
        num = block[i].find('span',attrs={'class':'iconSprite phoneStatic'})
        try:
            name = remove_html_tags(str(name)).replace('&amp;','&') 
            #name = name.replace("'","'''")
            num = remove_html_tags(str(num))
            if addy:
                addy =remove_html_tags(str(addy))
                query = """insert into directory(name, address, number) VALUES("%s", "%s", "%s")""" % (name,addy,num)
                db_update(query,connection)
                print "Added %s %s %s" % (name,addy,num)
            else:
                query = """insert into directory(name, address, number) VALUES("%s", "NULL", "%s")""" % (name,num)
                print "Added %s %s" % (name,num)
                db_update(query,connection)
        except Exception, e:
            my_logger.debug( e )

def scrape_refine(url,connection):
    soup = open_page(url,connection)
    menu=soup.find('div',attrs={'class':'resultInfo '})
    p = re.search('([0-9]+)',str(menu));
    if int(p.group(0)) <= 200:
        scrape_next_recursive(soup, connection)
    else:
        #else we refine by region
        region = soup.findAll('div',attrs={'class':'refineCont'})
        region = region[1].findAll('span', attrs={'class':'catRef'})
        links = []
        for i in range(len(region)):
            links.append(region[i].find('a')['href']);
 
        for link in links:
            scrape_refine(domain+link, connection)  
    return

def scrape_next_recursive(soup, connection):   
    scrape_page_simple(soup, connection)
    next=soup.find('a',attrs={'class':'nextLink'})
    if next:
        scrape_next_recursive(open_page(domain+next['href'],connection),connection)

def getlinks(soup):
    list = []
    links = soup.findAll('a')
    for link in links:
        if link['href'] not in list:
            list.append(link['href'])    
    return list

def fixnames(connection):
    query = """select * from names"""
    connection.conn.execute(query)    
    total = connection.conn.fetchall()
    for i in range(0,len(total)):
        if "\r" in total[i][1]:
            try:
                query = """insert into names(name, status, hits) values('%s','%s','%s')""" % (total[i][1].replace("\r",""),total[i][2],total[i][3])
                connection.conn.execute(query)
                print query
            except Exception, e:
                print e
            try:
                query = """DELETE FROM names where name = '%s'""" % total[i][1]
                connection.conn.execute(query)
                print query
            except Exception, e:
                print e

def run(max):
    for i in range(max):
        query_site(Connection()) 
    return

def main():
    setup_logging()
    random.seed()
    load_proxies()
    #fixnames(Connection())
    for i in range(10):
        t = Thread(target = run, args=(1000,))
        t.start()
    return

main()
