import urllib2
import json
import sys
import string
import MySQLdb
import MySQLdb.cursors


class Show:
    def __init__(self, title=""):
        self.title = title
        self.episodes = {}
 

def add_to_db(show, table):
    try:
        query = """select season,episode from %s where name = '%s'""" % (table,show.title)
        print query
        curr.execute(query);
        result = curr.fetchall()
        values=[]
        for episode in show.episodes.keys():
            if (episode['season'],episode['episode']) not in result:
                values.append((show.title,episode[0],episode[1],show.episodes[episode]['title']))
        if len(values) > 0:
            insert = """insert into %s (name,season,episode,title) """% table
            insert+="""values (%s,%s,%s,%s)""" 
            curr.executemany(insert,values)
            #print values
            #conn.execute("""insert into cartoons (name,season,episode,title) values ('hi',1,1,'hi')""")
            curr.close()
            conn.commit()
    except MySQLdb.Error, e:
        print e
 
def search_db(show_id,table):   
    curr.execute("""select * from %s where name =%s""" % (table,'%s'),[show_id])
    results = curr.fetchall()
    if not results:
        results = curr.execute()
        show_id = show_id.translate(string.maketrans(' -!?_*:;\'"','%%%%%%%%%%'))
        results = curr.fetchall()
        if not results:
            return False
    show = Show(results[0]['name'])
    for episode in results:
        show.episodes[(int(episode["season"]), int(episode["episode"]))] = {"title": str(episode["title"])}
    return show

def search_imdbapi(show_id,extras=''):
    """Get the episodes from imdbapi."""
    print "testing: http://imdbapi.poromenos.org/json/?name=%s" % str(show_id+extras).replace(' ','+')
    results = json.loads(urllib2.urlopen("http://imdbapi.poromenos.org/json/?name=%s"
        % str(show_id+extras).replace(' ','+')).read())
    if results == None:
        if show_id[0] == '%' and show_id[-1] == '%':
            return False
        elif show_id[-1] == '%':
            return search_imdb('%'+show_id)
        show_id = show_id.translate(string.maketrans(' -!?_*:;\'"','%%%%%%%%%%'))
        return search_imdbapi(show_id+'%')       
    elif 'shows' in results:
        print 'you must choose...'
        for x,show in enumerate(results['shows']):
            print str(x+1) +"\t",show['name'],show['year']
        selection = int(raw_input('select a number'))-1
        return search_imdbapi(str(results['shows'][selection]['name']),
            extras='&year='+str(results['shows'][selection]['year']));
    show = Show(str(results.keys()[0]))
    for episode in results[show.title]["episodes"]:
        show.episodes[(int(episode["season"]), int(episode["number"]))] = {"title": episode["name"]}
    return show
   

conn = MySQLdb.connect(host = "127.0.0.1",user="root",passwd = "Se7en303",db = "Sesame",
                     cursorclass=MySQLdb.cursors.DictCursor)
curr = conn.cursor();

show = search_db('the simpsons','cartoons')

#show =  search_imdbapi('')
if show:
    #add_to_db(show,'cartoons',conn)
    print show.episodes[(1,5)]



title = 'Sesame'

#from Tkinter import *

#root = Tk()

#scrollbar = Scrollbar(root)
#scrollbar.pack(side=RIGHT, fill=Y)

#listbox = Listbox(root, yscrollcommand=scrollbar.set)
#for i in range(1000):
#    listbox.insert(END, str(i))
#listbox.pack(side=LEFT, fill=BOTH)

#scrollbar.config(command=listbox.yview)

#mainloop()
